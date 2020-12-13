#pragma once
#include "TaskSystem/TaskSystem.h"
#include "RuntimeCore/RuntimeCore.h"
#include "Codebase/Codebase.h"

#define forloop(i, z, n) for(auto i = std::decay_t<decltype(n)>(z); i<(n); ++i)
#define def static constexpr auto

namespace sakura::ecs
{
	using namespace core::database;
	using core::entity;
	//using namespace core::codebase;
	using pass = core::codebase::pass;
	using filters = core::codebase::filters;
	using task = core::codebase::task;
	
	using core::codebase::value_type_t;
	using core::codebase::cid;
	using core::codebase::param;
	using core::codebase::operation;
	using core::codebase::array_type_t;
	
	struct pipeline final : public core::codebase::pipeline
	{
		using base_t = core::codebase::pipeline;
		pipeline(world& ctx):base_t(ctx){
			pass_events.reserve(100);
		};
		template<class T>
		pass* create_pass(const filters& v, T paramList)
		{
			pass_events.emplace_back(task_system::Event::Mode::Manual);
			return static_cast<pass*>(base_t::create_pass(v, paramList));
		}
		void wait()
		{
			forloop(i, 0u, pass_events.size())
				pass_events[i].wait();
		}
		sakura::vector<task_system::Event> pass_events;
		bool force_no_parallel = false;
	};
	
	template<class ...Ts>
	struct component_list
	{
		operator sakura::ecs::typeset() const
		{
			static core::database::index_t list[] = { sakura::ecs::cid<std::decay_t<Ts>>... };
			return list;
		}
		static const core::database::typeset typeset()
		{
			static sakura::ecs::index_t list[] = { sakura::ecs::cid<std::decay_t<Ts>>... };
			return list;
		}
	};

	class ECSAPI World : public core::database::world
	{
	public:
		using base_t = core::database::world;

		//FORCEINLINE chunk_vector<chunk_slice> initialize(const entity_type& type, uint32_t count = 1)
		//{
		//	auto res = base_t::allocate(type, count);
		//	return res;
		//}
	};

	
#define DEFINE_GETTER(Name, Default) \
template<class T, class = void> \
struct get_##Name{ static constexpr auto value = Default; }; \
template<class T> \
struct get_##Name<T, std::void_t<decltype(T::Name)>> { static constexpr auto value = T::Name; }; \
template<class T> \
static constexpr auto get_##Name##_v = get_##Name<T>::value;

	DEFINE_GETTER(manual_clean, false);
	DEFINE_GETTER(manual_copy, false);
	DEFINE_GETTER(buffer_capacity, 1);

	namespace detail
	{
		template<template<class...> class TP, class T>
		struct is_template : std::false_type {};

		template<template<class...> class TP, class... T>
		struct is_template<TP, TP<T...>> : std::true_type {};

		template<template<class...> class TP, class... T>
		static constexpr auto is_template_v = is_template<TP, T...>{};
	}

	template<class T, class = void>
	struct is_buffer : std::false_type {};

	template<class T>
	struct is_buffer<T, std::void_t<typename T::value_type>> : detail::is_template<core::database::buffer_t, typename T::value_type> {};

	template<class T>
	core::database::index_t register_component(intptr_t* entityRefs = nullptr, int entityRefCount = 0, core::database::component_vtable vtable = {})
	{
		using namespace core::codebase;
		component_desc desc;
		desc.isElement = is_buffer<T>{};
		desc.manualClean = get_manual_clean_v<T>;
		desc.manualCopy = get_manual_copy_v<T>;
		desc.size = get_buffer_capacity_v<T> *sizeof(T);
		desc.elementSize = sizeof(T);
		desc.GUID = T::guid;
		desc.entityRefs = entityRefs;
		desc.entityRefCount = entityRefCount;
		desc.alignment = alignof(T);
		desc.name = typeid(T).name();
		desc.vtable = vtable;
		return register_type(desc);
	}

	template<class T>
	core::codebase::array_type_t<T> init_component(core::database::world& ctx, core::database::chunk_slice c)
	{
		using namespace core::codebase;
		return (array_type_t<T>)const_cast<void*>(ctx.get_component_ro(c.c, cid<T>)) + (size_t)c.start;
	}
	
}

// task_system support.
namespace sakura::task_system
{
	template<typename F, bool ForceParallel = false, bool ForceNoParallel = false>
	FORCEINLINE task_system::Event ecs_schedule(
		ecs::pipeline& pipeline, ecs::pass& pass, F&& t, int maxSlice = -1)
	{
		static_assert(std::is_invocable_v<std::decay_t<F>, const ecs::pipeline&, const ecs::pass&, const ecs::task&>,
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