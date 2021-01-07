#pragma once
#include "TaskSystem/TaskSystem.h"
#include "RuntimeCore/RuntimeCore.h"
#include "Codebase.h"

#define forloop(i, z, n) for(auto i = std::decay_t<decltype(n)>(z); i<(n); ++i)
#define def static constexpr auto

namespace sakura
{
	namespace ecs
	{
		using namespace core::codebase;
	}
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
		}
	};
	struct pass : core::codebase::pass_t<custom_pass> {};
	struct pipeline final : public core::codebase::pipeline
	{
		using base_t = core::codebase::pipeline;
		pipeline(sakura::ecs::world&& ctx) :base_t(std::move(ctx)) 
		{
			allpasses.reserve(10000);
		};
		template<class T>
		std::shared_ptr<pass> create_pass(const sakura::ecs::filters& v, T paramList, gsl::span<core::codebase::shared_entry> sharedEntries = {})
		{
			auto p = base_t::create_pass<pass>(v, paramList, sharedEntries);
			allpasses.push_back(std::static_pointer_cast<custom_pass>(p));
			return p;
		}
		std::shared_ptr<custom_pass> create_custom_pass(gsl::span<core::codebase::shared_entry> sharedEntries = {})
		{
			auto p = base_t::create_custom_pass<custom_pass>(sharedEntries);
			allpasses.push_back(p);
			return p;
		}
		void wait() const
		{
			forloop(i, 0u, allpasses.size())
				if(auto pass = allpasses[i].lock())
					pass->event.wait();
			allpasses.clear();
		}
		void forget()
		{
			allpasses.erase(remove_if(allpasses.begin(), allpasses.end(), [](auto& n) {return n.expired(); }), allpasses.end());
		}
		void sync_dependencies(gsl::span<std::weak_ptr<core::codebase::custom_pass>> dependencies) const override
		{
			for (auto dpr : dependencies)
				if(auto dp = dpr.lock())
					((custom_pass*)dp.get())->event.wait();
		}
		void sync_all() const
		{
			wait();
		}
		mutable std::vector<std::weak_ptr<custom_pass>> allpasses;
		bool force_no_parallel = false;
	};
#ifndef ZoneScopedN
#define ZoneScopedN(...)
#endif
	template<class F>
	FORCEINLINE void schedule_custom(pipeline& pipeline, std::shared_ptr<custom_pass> p, F&& t, std::vector<task_system::Event> externalDependencies = {})
	{
		task_system::schedule([&, p, t, externalDependencies = std::move(externalDependencies)]() mutable
		{
			defer(p->event.signal());
			p->wait_for_dependencies();
			p->release_dependencies();
			for (auto& ed : externalDependencies)
				ed.wait();
			ZoneScopedN("Task");
			t();
		});
	}

	template<bool ForceParallel = false, bool ForceNoParallel = false, class F, class F2>
	FORCEINLINE void schedule_init(
		pipeline& pipeline, std::shared_ptr<pass> p, F&& f, F2&& t, int batchCount, std::vector<task_system::Event> externalDependencies = {})
	{
		static_assert(std::is_invocable_v<std::decay_t<F>, const task_system::ecs::pipeline&, const pass&>,
			"F must be an invokable of void(const ecs::pipeline&, const ecs::pass&)>");
		static_assert(std::is_invocable_v<std::decay_t<F2>, const task_system::ecs::pipeline&, const pass&, const sakura::ecs::task&>,
			"F2 must be an invokable of void(const ecs::pipeline&, const ecs::pass&, const ecs::task&)>");
		static_assert(!(ForceParallel & ForceNoParallel),
			"A schedule can not force both parallel and not parallel!");
		task_system::schedule([&, p, batchCount, f, t, externalDependencies = std::move(externalDependencies)]() mutable
		{
			defer(p->event.signal());
			auto [tasks, groups] = pipeline.create_tasks(*p, batchCount);
			//defer(tasks.reset());
			f(pipeline, *p);
			p->wait_for_dependencies();
			p->release_dependencies();
			for (auto& ed : externalDependencies)
				ed.wait();
			ZoneScopedN("Task");
			constexpr auto MinParallelTask = 8u;
			const bool recommandParallel = !p->hasRandomWrite && groups.size > MinParallelTask;
			if (pipeline.force_no_parallel)
				goto FORCE_NO_PARALLEL;
			if ((recommandParallel && !ForceNoParallel) || ForceParallel)
			{
				task_system::WaitGroup tasksGroup((uint32_t)groups.size);
				forloop(grp, 0, groups.size)
				{
					auto& gp = groups[grp];
					task_system::schedule([&, gp, tasksGroup] {
						// Decrement the WaitGroup counter when the task has finished.
						defer(tasksGroup.done());
						forloop(tsk, gp.begin, gp.end)
						{
							auto& tk = tasks[tsk];
							t(pipeline, *p, tk);
						}
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

	template<bool ForceParallel = false, bool ForceNoParallel = false, class F>
	FORCEINLINE void schedule(
		pipeline& pipeline, std::shared_ptr<pass> p, F&& t, int batchCount, std::vector<task_system::Event> externalDependencies = {})
	{
		schedule_init(pipeline, p, [](const task_system::ecs::pipeline&, const pass&) {}, std::forward<F>(t), batchCount, externalDependencies);
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