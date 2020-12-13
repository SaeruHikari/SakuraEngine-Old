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
	struct pipeline final : public core::codebase::pipeline
	{
		using base_t = core::codebase::pipeline;
		pipeline(sakura::ecs::world& ctx) :base_t(ctx) {
			pass_events.reserve(100);
		};
		template<class T>
		sakura::ecs::pass* create_pass(const sakura::ecs::filters& v, T paramList)
		{
			pass_events.emplace_back(task_system::Event::Mode::Manual);
			return static_cast<sakura::ecs::pass*>(base_t::create_pass(v, paramList));
		}
		void wait()
		{
			forloop(i, 0u, pass_events.size())
				pass_events[i].wait();
		}
		sakura::vector<task_system::Event> pass_events;
		bool force_no_parallel = false;
	};

	template<typename F, bool ForceParallel = false, bool ForceNoParallel = false>
	FORCEINLINE task_system::Event schedule(
		pipeline& pipeline, sakura::ecs::pass& pass, F&& t, int maxSlice = -1)
	{
		static_assert(std::is_invocable_v<std::decay_t<F>, const task_system::ecs::pipeline&, const sakura::ecs::pass&, const sakura::ecs::task&>,
			"F must be an invokable of void(const ecs::pipeline&, const ecs::pass&, const ecs::task&)>");
		static_assert(!(ForceParallel & ForceNoParallel),
			"A schedule can not force both parallel and not parallel!");
		task_system::schedule([&, maxSlice, t]()
		{
			defer(pipeline.pass_events[pass.passIndex].signal());
			//defer(tasks.reset());
			for(auto i = 0u; i < pass.dependencyCount; i++)
				pipeline.pass_events[pass.dependencies[i]->passIndex].wait();
			auto tasks = pipeline.create_tasks(pass, maxSlice); //�� pass ��ȡ task

			constexpr auto MinParallelTask = 10u;
			const bool recommandParallel = !pass.hasRandomWrite && tasks.size > MinParallelTask;
			if (pipeline.force_no_parallel)
				goto FORCE_NO_PARALLEL;
			if ((recommandParallel & !ForceNoParallel) || ForceParallel) // task����task_system
			{
				task_system::WaitGroup tasksGroup(tasks.size);
				forloop(tsk, 0, tasks.size)
				{
					auto& tk = tasks[tsk];
					task_system::schedule([&, tasksGroup] {
						// Decrement the WaitGroup counter when the task has finished.
						defer(tasksGroup.done());
						t(pipeline, pass, tk);
					});
				}
				tasksGroup.wait();
			}
			else // ����
			{
			FORCE_NO_PARALLEL:
				std::for_each(
					tasks.begin(), tasks.end(), [&, t](auto& tk)
					{
						t(pipeline, pass, tk);
					});
			}
		});
		return pipeline.pass_events[pass.passIndex];
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