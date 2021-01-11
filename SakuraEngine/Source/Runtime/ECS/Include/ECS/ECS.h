#pragma once
#include "TaskSystem/TaskSystem.h"
#include "RuntimeCore/RuntimeCore.h"
#include "Codebase.h"
#include <string>

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
	struct source_location_data
	{
		std::string_view name;
		std::string_view function;
		std::string_view file;
		uint32_t line;
	};

	struct pass_location
	{
		source_location_data location;
#ifdef TRACY_ENABLE
		tracy::SourceLocationData system;
		tracy::SourceLocationData task;
		tracy::SourceLocationData initialize;
#endif
	};
#define ECS_CAT(a, b) a##b
#define ECS_STR(a) ECS_STR_(a)
#define ECS_STR_(a) #a
#define SourceLocation( name ) sakura::task_system::ecs::source_location_data{ name, __FUNCTION__,  __FILE__, (uint32_t)__LINE__ }
#ifdef TRACY_ENABLE
#define TracyLocation( name ) tracy::SourceLocationData{ name, __FUNCTION__,  __FILE__, (uint32_t)__LINE__, 0 }
#define PassLocation( name ) []() { static constexpr sakura::task_system::ecs::pass_location _ {  SourceLocation(ECS_STR(name)), TracyLocation(ECS_STR(ECS_CAT(name, System))), TracyLocation(ECS_STR(ECS_CAT(name, Task))), TracyLocation(ECS_STR(ECS_CAT(name, Schedule)))}; return &_; }()
#else
#define PassLocation( name ) []() { static constexpr sakura::task_system::ecs::pass_location _ { SourceLocation(ECS_STR(name)) };
#endif

	struct phase
	{
		const source_location_data* location;
		const phase* parentPhase;
		std::vector<phase> childPhases;
		std::vector<const pass_location*> childPasses;
	};

	struct pass_statistics
	{
		const pass_location* location;
		int matchedCount;
		bool isEnabled;
	};

	struct custom_pass : core::codebase::custom_pass
	{
		task_system::Event event{ task_system::Event::Mode::Manual };
		const pass_location* location;
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
		pipeline(sakura::ecs::world&& ctx) :base_t(std::move(ctx)), topPhase(&rootPhase)
		{
			allPasses.reserve(10000);
		};
		template<class T>
		std::shared_ptr<pass> create_pass(const sakura::ecs::filters& v, T paramList, const pass_location* location, gsl::span<core::codebase::shared_entry> sharedEntries = {})
		{
			auto p = base_t::create_pass<pass>(v, paramList, sharedEntries);
			p->location = location;
			allPasses.push_back(std::static_pointer_cast<custom_pass>(p));
			return p;
		}
		std::shared_ptr<custom_pass> create_custom_pass(const pass_location* location, gsl::span<core::codebase::shared_entry> sharedEntries = {})
		{
			auto p = base_t::create_custom_pass<custom_pass>(sharedEntries);
			p->location = location;
			allPasses.push_back(p);
			return p;
		}
		void wait() const
		{
			forloop(i, 0u, allPasses.size())
				if(auto pass = allPasses[i].lock())
					pass->event.wait();
			allPasses.clear();
		}
		void begin_phase()
		{

		}
		void end_phase()
		{

		}
		pass_statistics* get_statistics(const pass_location* location)
		{

		}
		void mark_frame()
		{
			allPasses.erase(remove_if(allPasses.begin(), allPasses.end(), [](auto& n) {return n.expired(); }), allPasses.end());
			inc_timestamp();
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
		mutable std::vector<std::weak_ptr<custom_pass>> allPasses;
		phase rootPhase;
		phase* topPhase;
		bool force_no_group_parallel = false;
		bool force_no_fibers = false;
	};

#ifdef TRACY_ENABLE
#define ZoneScopedPass(name) tracy::ScopedZone ___tracy_scoped_zone(&name, true);
#else
#define ZoneScopedPass(name)
#endif

	template<class F>
	FORCEINLINE void schedule_custom(pipeline& pipeline, std::shared_ptr<custom_pass> p, F&& t, std::vector<task_system::Event> externalDependencies = {})
	{
		task_system::schedule([&, p, t, externalDependencies = std::move(externalDependencies)]() mutable
		{
			task_defer(p->event.signal());
			{
				p->wait_for_dependencies();
				p->release_dependencies();
				for (auto& ed : externalDependencies)
					ed.wait();
			}
			ZoneScopedPass(p->location->system);
			t();
		});
	}

	template<bool ForceParallel = false, bool ForceNoGroupParallel = false, bool ForceNotFiber = false, class F, class F2>
	FORCEINLINE void schedule_init(
		pipeline& pipeline, std::shared_ptr<pass> p, F&& f, F2&& t, int batchCount, std::vector<task_system::Event> externalDependencies = {})
	{
		static_assert(std::is_invocable_v<std::decay_t<F>, const task_system::ecs::pipeline&, const pass&>,
			"F must be an invokable of void(const ecs::pipeline&, const ecs::pass&)>");
		static_assert(std::is_invocable_v<std::decay_t<F2>, const task_system::ecs::pipeline&, const pass&, const sakura::ecs::task&>,
			"F2 must be an invokable of void(const ecs::pipeline&, const ecs::pass&, const ecs::task&)>");
		static_assert(!(ForceParallel & ForceNoGroupParallel),
			"A schedule can not force both parallel and not parallel!");
		auto toSchedule = [&, p, batchCount, f, t, externalDependencies = std::move(externalDependencies)]() mutable
		{
			task_defer(p->event.signal());
			auto [tasks, groups] = [&]() 
			{
				ZoneScopedPass(p->location->initialize);
				f(pipeline, *p);
				return pipeline.create_tasks(*p, batchCount); 
			}();
			//defer(tasks.reset());
			{
				p->wait_for_dependencies();
				p->release_dependencies();
				for (auto& ed : externalDependencies)
					ed.wait();
			}
			ZoneScopedPass(p->location->system);
			constexpr auto MinParallelTask = 8u;
			const bool recommandParallel = !p->hasRandomWrite && groups.size > MinParallelTask;
			if (pipeline.force_no_group_parallel)
				goto FORCE_NO_GROUP_PARALLEL;
			if ((recommandParallel && !ForceNoGroupParallel) || ForceParallel)
			{
				task_system::WaitGroup tasksGroup((uint32_t)groups.size);
				forloop(grp, 0, groups.size)
				{
					auto& gp = groups[grp];
					task_system::schedule([&, gp, tasksGroup] {
						// Decrement the WaitGroup counter when the task has finished.
						ZoneScopedPass(p->location->task);
						task_defer(tasksGroup.done());
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
			FORCE_NO_GROUP_PARALLEL:
				std::for_each(
					groups.begin(), groups.end(), [&, t](auto& gp) mutable
					{
						ZoneScopedPass(p->location->task);
						forloop(tsk, gp.begin, gp.end)
						{
							auto& tk = tasks[tsk];
							t(pipeline, *p, tk);
						}
					});
			}
		};
		
		if (ForceNotFiber || pipeline.force_no_fibers)
			toSchedule();
		else
			task_system::schedule(toSchedule);
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