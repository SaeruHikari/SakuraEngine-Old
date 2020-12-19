#pragma once
#include "TaskSystem/TaskSystem.h"
#include "RuntimeCore/RuntimeCore.h"
#include "Codebase/Codebase.h"

#define forloop(i, z, n) for(auto i = std::decay_t<decltype(n)>(z); i<(n); ++i)
#define def static constexpr auto

namespace sakura
{
	namespace ecs = core::codebase;
}

// task_system support.
namespace sakura::task_system::ecs
{
	struct custom_pass : core::codebase::custom_pass
	{
		task_system::Event event{ task_system::Event::Mode::Manual };
		void wait_for_dependencies()
		{
			forloop(i, 0, dependencyCount)
			{
				auto& dep = dependencies[i];
				if (auto ptr = std::static_pointer_cast<custom_pass>(dep.lock()))
					ptr->event.wait();
			}
			release_dependencies();
		}
	};
	struct pass : core::codebase::pass_t<custom_pass> {};
	struct pipeline final : public core::codebase::pipeline
	{
		using base_t = core::codebase::pipeline;
		pipeline(sakura::ecs::world& ctx) :base_t(ctx) 
		{
			allpasses.reserve(10000);
		};
		template<class T>
		std::shared_ptr<pass> create_pass(const sakura::ecs::filters& v, T paramList, gsl::span<core::codebase::shared_entry> sharedEntries = {})
		{
			auto p = base_t::create_pass<pass>(v, paramList, sharedEntries);
			allpasses.push_back(p->event);
			return p;
		}
		std::shared_ptr<custom_pass> create_custom_pass(gsl::span<core::codebase::shared_entry> sharedEntries = {})
		{
			auto p = base_t::create_custom_pass<custom_pass>(sharedEntries);
			allpasses.push_back(p->event);
			return p;
		}
		void wait()
		{
			forloop(i, 0u, allpasses.size())
				allpasses[i].wait();
			allpasses.clear();
		}
		void forget()
		{
			allpasses.clear();
		}
		std::vector<task_system::Event> allpasses;
		bool force_no_parallel = false;
	};
	template<class F>
	FORCEINLINE void schedule_custom(pipeline& pipeline, std::shared_ptr<custom_pass> p, F&& t, std::vector<task_system::Event> externalDependencies = {})
	{
		task_system::schedule([&, p, t, externalDependencies = std::move(externalDependencies)]() mutable
		{
			defer(p->event.signal());
			p->wait_for_dependencies();
			for (auto& ed : externalDependencies)
				ed.wait();
			t();
		});
	}

	template<bool ForceParallel = false, bool ForceNoParallel = false, class F>
	FORCEINLINE void schedule(
		pipeline& pipeline, std::shared_ptr<pass> p, F&& t, int maxSlice = -1, std::vector<task_system::Event> externalDependencies = {})
	{
		static_assert(std::is_invocable_v<std::decay_t<F>, const task_system::ecs::pipeline&, const pass&, const sakura::ecs::task&>,
			"F must be an invokable of void(const ecs::pipeline&, const ecs::pass&, const ecs::task&)>");
		static_assert(!(ForceParallel & ForceNoParallel),
			"A schedule can not force both parallel and not parallel!");
		//if (pass.archetypeCount == 0)
		//{
		//	task_system::Event e{ task_system::Event::Mode::Auto };
		//	e.signal();
		//	return e;
		//}
		task_system::schedule([&, p, maxSlice, t, externalDependencies = std::move(externalDependencies)]() mutable
		{
			defer(p->event.signal());
			//defer(tasks.reset());
			p->wait_for_dependencies();
			for (auto& ed : externalDependencies)
				ed.wait();
			auto tasks = pipeline.create_tasks(*p, maxSlice);

			constexpr auto MinParallelTask = 10u;
			const bool recommandParallel = !p->hasRandomWrite && tasks.size > MinParallelTask;
			if (pipeline.force_no_parallel)
				goto FORCE_NO_PARALLEL;
			if ((recommandParallel & !ForceNoParallel) || ForceParallel)
			{
				task_system::WaitGroup tasksGroup((uint32_t)tasks.size);
				forloop(tsk, 0, tasks.size)
				{
					auto& tk = tasks[tsk];
					task_system::schedule([&, tasksGroup] {
						// Decrement the WaitGroup counter when the task has finished.
						defer(tasksGroup.done());
						t(pipeline, *p, tk);
						});
				}
				tasksGroup.wait();
			}
			else
			{
			FORCE_NO_PARALLEL:
				std::for_each(
					tasks.begin(), tasks.end(), [&, t](auto& tk) mutable
					{
						t(pipeline, *p, tk);
					});
			}
		});
	}
}

#undef forloop
#undef def

class ECSAPI ECSModule : public sakura::IModule
{
public:
	static ECSModule* create();
	bool StartUp() override;
	bool Terminate() override;
};