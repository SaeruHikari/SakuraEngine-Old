#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#define TRACY_ENABLE
#include "tracy/Tracy.hpp"

#include "RuntimeCore/RuntimeCore.h"
#include "spdlog/spdlog.h"
#include "marl/finally.h"
#include "SakuraSTL.hpp"

#include "System/VirtualFileSystem.h"
#include "System/Log.h"

#include "ECS/ECS.h"
#include "RingBuffer.h"

#include "TransformComponents.h"
#include "RenderSystem.h"
#include "Boids.h"
#include "TaskSystem/TaskSystem.h"
#include "RuntimeCore/RuntimeCore.h"
#include "kdtree.h"
#include <iostream>
#include <random>
#include <cmath>
#include <fstream>
#include <filesystem>
#include "System/Messages.h"
#include "imgui/sakura_imgui.h"


#define forloop(i, z, n) for(auto i = std::decay_t<decltype(n)>(z); i<(n); ++i)
#define def static constexpr auto

namespace task_system = sakura::task_system;
namespace math = sakura::math;
using Vector3f = sakura::Vector3f;
using Quaternion = sakura::Quaternion;
using Rotator = sakura::Rotator;
using float4x4 = sakura::float4x4;
using IModule = sakura::IModule;

sakura::ecs::world ctx;

struct Timer
{
	void start_up()
	{
		tmpt = std::chrono::system_clock::now();
	}

	double end()
	{
		auto dur = std::chrono::system_clock::now() - tmpt;
		auto delta_time =
			static_cast<double>(dur.count()) / static_cast<double>(decltype(dur)::period::den);
		tmpt = std::chrono::system_clock::now();
		return delta_time;
	}
	std::chrono::system_clock::time_point tmpt;
};



struct buffer_serializer final : sakura::ecs::serializer_i
{
	std::vector<char>& buffer;
	buffer_serializer(std::vector<char>& buffer)
		: buffer(buffer) {}
	void stream(const void* data, uint32_t bytes) override
	{
		if (bytes == 0)
			return;
		size_t size = buffer.size();
		buffer.resize(size + bytes);
		memcpy(&buffer[size], data, bytes);
	}
	bool is_serialize() override { return true; }
};
struct buffer_deserializer final : sakura::ecs::serializer_i
{
	const char* buffer;
	size_t offset = 0;
	int i = 0;
	buffer_deserializer(const char* buffer)
		: buffer(buffer) {}
	void stream(const void* data, uint32_t bytes) override
	{
		void* mdata = const_cast<void*>(data);
		if (bytes == 0)
			return;
		memcpy(mdata, &buffer[offset], bytes);
		offset += bytes;
	}
	bool is_serialize() override { return false; }
};


std::size_t calc_align(std::size_t n, std::size_t align)
{
	return ((n + align - 1) / align)* align;
}

template<class T, class... Ts, class F>
void ConvertSystem(task_system::ecs::pipeline& ppl, ecs::filters& filter, F&& f, const task_system::ecs::pass_location* location)
{
	using namespace ecs;
	static_assert(std::is_invocable<F, value_type_t<T>, const value_type_t<Ts>...>(), "wrong signature of convert function");
	static size_t timestamp = 0;
	/*filter.chunkFilter =
	{
		complist<Ts...>,
		timestamp
	};*/
	def paramList = boost::hana::make_tuple(
		param<T>,
		param<const Ts>...
	);
	timestamp = ppl.get_timestamp();
	return task_system::ecs::schedule(ppl, ppl.create_pass(filter, paramList, location),
		[&, f](const task_system::ecs::pass& pass, const task& tk)
		{
			auto o = operation{ paramList, pass, tk };
			hana::tuple arrays = boost::hana::make_tuple(
				o.get_parameter<T>(), o.get_parameter<const Ts>()...);
			forloop(i, 0, o.get_count())
			{
				auto params = hana::transform(arrays, [i](auto v) { return v ? v + i : nullptr; });
				hana::unpack(params, f);
			}
		}, 200);
}

sakura::Quaternion ToRot(const sakura::Vector3f& hd)
{

	Quaternion rot = {};
	const auto v1 = Vector3f(0.f, 0.f, 1.f);
	const auto v2 = hd;
	Vector3f a = math::cross_product(v1, v2);
	rot = Quaternion(a[0], a[1], a[2],
		sqrt(v1.length_squared() * v2.length_squared()) + math::dot_product(v1, v2));
	return math::normalize(rot);
}

template<class T>
void Local2XSystem(task_system::ecs::pipeline& ppl, ecs::filters& filter, const task_system::ecs::pass_location* location)
{
	return ConvertSystem<T, Translation, Rotation, Scale, Heading>(ppl, filter,
		[](typename T::value_type* dst, const sakura::Vector3f* inTranslation, const sakura::Quaternion* inQuaternion, const sakura::Vector3f* inScale, const sakura::Vector3f* inHeading)
		{
			const Vector3f scale = inScale ? *inScale : Vector3f::vector_one();
			const Vector3f translation = inTranslation ? *inTranslation : Vector3f::vector_zero();
			const Quaternion quaternion = inQuaternion ? *inQuaternion : (inHeading? ToRot(*inHeading) : Quaternion::identity());

			*dst = math::make_transform(translation, scale, quaternion);
		}, location);
}

void RotationEulerSystem(task_system::ecs::pipeline& ppl)
{
	using namespace ecs;
	filters filter;
	filter.archetypeFilter = {
		{complist<RotationEuler, Rotation>}
	};
	return ConvertSystem<Rotation, RotationEuler>(ppl, filter,
		[](sakura::Quaternion* dst, const sakura::Rotator* inRotator)
		{
			*dst = math::quaternion_from_rotator(*inRotator);
		}, PassLocation(EulerToQuat));
}

void Child2WorldSystem(task_system::ecs::pipeline& ppl)
{
	using namespace ecs;
	filters filter;
	filter.archetypeFilter = {
		{complist<Child, LocalToWorld>},
		{},
		{complist<Parent, LocalToParent>} // from root
	};
	def paramList = boost::hana::make_tuple(
		// write
		param<LocalToWorld, true>,
		// read.
		param<const LocalToParent, true>, param<const Child, true>
	);
	struct children2World
	{
		static void solve(const float4x4& parent_l2w, const entity e)
		{
			float4x4 l2w = float4x4();
			auto child_l2w = static_cast<float4x4*>(ctx.get_owned_rw(e, cid<LocalToWorld>));
			const auto child_l2p = static_cast<const float4x4*>(ctx.get_owned_ro(e, cid<LocalToParent>));
			if (child_l2w && child_l2p)
			{
				l2w = sakura::math::multiply(parent_l2w, *child_l2p);
				*child_l2w = l2w;
			}

			index_t childSet[] = { cid<Child> };
			if (ctx.has_component(e, childSet))
			{
				ecs::value_type_t<Child> child_children = ctx.get_owned_ro(e, cid<Child>);
				for (auto& child : child_children)
				{
					children2World::solve(l2w, child);
				}
			}
		}
	};
	return task_system::ecs::schedule(ppl,
		ppl.create_pass(filter, paramList, PassLocation(Child2World)),
		[](const task_system::ecs::pass& pass, const ecs::task& tk)
		{
			auto o = operation{ paramList, pass, tk };
			auto [childrens, l2ws] = o.get_parameters<const Child, LocalToWorld>();

			forloop(i, 0, o.get_count())
			{
				auto& children = childrens[i];
				for (const auto& child : children)
				{
					children2World::solve(l2ws[i], child);
				}
			}
		}, 100);
}

void World2LocalSystem(task_system::ecs::pipeline& ppl)
{
	using namespace ecs;
	filters filter;
	filter.archetypeFilter = {
		{complist<LocalToWorld, WorldToLocal>}, //all
		{}, //any
		{} //none
	};
	def paramList = boost::hana::make_tuple(
		// write
		param<WorldToLocal>,
		// read.
		param<const LocalToWorld>
	);
	return task_system::ecs::schedule(ppl,
		ppl.create_pass(filter, paramList, PassLocation(World2Local)),
		[](const task_system::ecs::pass& pass, const ecs::task& tk)
		{
			auto o = operation{ paramList, pass, tk };
			auto [l2ws, w2ls] = o.get_parameters<const LocalToWorld, WorldToLocal>();

			forloop(i, 0, o.get_count())
			{
				w2ls[i] = sakura::math::inverse(l2ws[i]);
			}
		}, 200);
}

template<class C, bool useMemcpy = true, class T>
void CopyComponent(task_system::ecs::pipeline& ppl, const ecs::filters& filter, ecs::shared_resource<T>& vector, const task_system::ecs::pass_location* location)
{
	using namespace ecs;
	def paramList = boost::hana::make_tuple( param<const C> );
	shared_entry shareList[] = { write(vector) };
	auto pass = ppl.create_pass(filter, paramList, location, shareList);
	return task_system::ecs::schedule_init(ppl, pass,
		[vector](const task_system::ecs::pass& pass) mutable
		{
			vector->resize(pass.calc_size());
		},
		[&, vector](const task_system::ecs::pass& pass, const ecs::task& tk) mutable
		{
			auto o = operation{ paramList, pass, tk };
			auto index = o.get_index();
			auto comps = o.get_parameter<const C>();
			if constexpr(useMemcpy)
				memcpy((*vector).data() + index, comps, o.get_count() * sizeof(C));
			else
				forloop(i, 0, o.get_count())
					(*vector)[index + i] = comps[i];
		}, 2000);
}

template<class C, bool useMemcpy = true, class T>
void FillComponent(task_system::ecs::pipeline& ppl, const ecs::filters& filter, ecs::shared_resource<T>& vector, const task_system::ecs::pass_location* location)
{
	using namespace ecs;
	def paramList = boost::hana::make_tuple(param<C>);
	shared_entry shareList[] = { read(vector) };
	auto pass = ppl.create_pass(filter, paramList, location, shareList);
	return task_system::ecs::schedule(ppl, pass,
		[&, vector](const task_system::ecs::pass& pass, const ecs::task& tk) mutable
		{
			auto o = operation{ paramList, pass, tk };
			auto index = o.get_index();
			auto comps = o.get_parameter<C>();
			if constexpr (useMemcpy)
				memcpy(comps, (*vector).data() + index, o.get_count() * sizeof(C));
			else
				forloop(i, 0, o.get_count())
				comps[i] = (*vector)[index + i];
		}, 2000);
}

template<class T>
void CollectEntites(task_system::ecs::pipeline& ppl, const ecs::filters& filter, ecs::shared_resource<T>& vector, const task_system::ecs::pass_location* location)
{
	using namespace ecs;
	def paramList = boost::hana::make_tuple();
	shared_entry shareList[] = { write(vector) };
	auto pass = ppl.create_pass(filter, paramList, location, shareList);
	return task_system::ecs::schedule_init(ppl, pass,
		[vector](const task_system::ecs::pass& pass) mutable
		{
			vector->resize(pass.calc_size());
		},
		[&, vector](const task_system::ecs::pass& pass, const ecs::task& tk) mutable
		{
			auto o = operation{ paramList, pass, tk };
			auto index = o.get_index();
			auto entities = o.get_entities();
			memcpy((*vector).data() + index, entities, o.get_count() * sizeof(entity));
		}, 2000);
}

struct BoidPosition
{
	sakura::Vector3f value;
	BoidPosition() {}
	BoidPosition(sakura::Vector3f value)
		:value(value) {}
	def dim = 3;
	using value_type = float;
	float operator[](size_t i) const { return value.data_view()[i]; }
};

sakura::Vector3f nearest_position(const sakura::Vector3f& query, const std::vector<sakura::Vector3f>& searchTargets)
{
	float minDistance = std::numeric_limits<float>::max();
	sakura::Vector3f result;
	for (const auto& target : searchTargets)
	{
		float d = math::distance(query, target);
		if (d < minDistance)
		{
			minDistance = d;
			result = target;
		}
	}
	return result;
}

auto& get_random_engine()
{
	static std::random_device r;
	static std::default_random_engine el(r());
	return el;
}

void RandomTargetSystem(task_system::ecs::pipeline& ppl)
{
	using namespace ecs;
	filters filter;
	filter.archetypeFilter =
	{
		{complist<Translation, MoveToward, RandomMoveTarget>}
	};
	def paramList = boost::hana::make_tuple(
		param<MoveToward>, param<const RandomMoveTarget>, param<const Translation>
	);
	return task_system::ecs::schedule(ppl, ppl.create_pass(filter, paramList, PassLocation(RandomTarget)), 
		[](const task_system::ecs::pass& pass, const ecs::task& tk)
		{
			auto o = operation{ paramList, pass, tk };
			auto [mts, trs, rmts] = o.get_parameters<MoveToward, const Translation, const RandomMoveTarget>();
			forloop(i, 0, o.get_count())
			{
				if (math::subtract(mts[i].Target, trs[i]).is_nearly_zero(40.f))
				{
					auto sphere = rmts[i];
					mts[i].Target = sphere.random_point(get_random_engine());
				}
			}
		}, 200);
}

void MoveTowardSystem(task_system::ecs::pipeline& ppl, float deltaTime)
{
	using namespace ecs;
	//PhaseScopedN(ppl, "MoveTowardSystem");
	filters filter;
	filter.archetypeFilter =
	{
		{complist<Translation, MoveToward>}
	};
	def paramList = boost::hana::make_tuple(
		param<Translation>, param<const MoveToward>
	);
	static std::random_device r;
	static std::default_random_engine el(r());
	return task_system::ecs::schedule(ppl, ppl.create_pass(filter, paramList, PassLocation(MoveToward)),
		[deltaTime](const task_system::ecs::pass& pass, const ecs::task& tk)
		{
			auto o = operation{ paramList, pass, tk };
			auto [mts, trs] = o.get_parameters<const MoveToward, Translation>();
			forloop(i, 0, o.get_count())
				trs[i] = trs[i] + math::normalize(mts[i].Target - trs[i]) * mts[i].MoveSpeed * deltaTime;
		}, 400);
}
std::atomic<size_t> averageNeighberCount = 0;
std::atomic<size_t> maxNeighberCount = 0;

template<typename T>
void update_maximum(std::atomic<T>& maximum_value, T const& value) noexcept
{
	T prev_value = maximum_value;
	while (prev_value < value &&
		!maximum_value.compare_exchange_weak(prev_value, value))
	{
	}
}

void BoidMoveSystem(task_system::ecs::pipeline& ppl, float deltaTime)
{
	using namespace ecs;
	filters boidFilter;
	boidFilter.archetypeFilter =
	{
		{complist<Boid, Translation, Heading>}, //all
		{}, //any
		{}, //none
		complist<Boid> //shared
	};
	def paramList =
		boost::hana::make_tuple(param<const Heading>, param<Translation>, param<const Boid>);
	return task_system::ecs::schedule(ppl, ppl.create_pass(boidFilter, paramList, PassLocation(BoidMove)),
		[deltaTime](const task_system::ecs::pass& pass, const ecs::task& tk)
		{
			auto o = operation{ paramList, pass, tk };
			auto [hds, trs] = o.get_parameters_owned<const Heading, Translation>();
			auto boid = o.get_parameter<const Boid>(); //这玩意是 shared
			forloop(i, 0, o.get_count())
				trs[i] = trs[i] + hds[i] * deltaTime * boid->MoveSpeed;
		}, 500);
}

void BoidsSystem(task_system::ecs::pipeline& ppl, float deltaTime)
{
	using namespace ecs;
	filters boidFilter;
	boidFilter.archetypeFilter =
	{
		{complist<Boid, Translation, Heading>}, //all
		{}, //any
		{}, //none
		complist<Boid> //shared
	};
	filters targetFilter;
	targetFilter.archetypeFilter =
	{
		{complist<BoidTarget, Translation>}
	};
	//构造 kdtree, 提取 headings
	static auto positions = make_resource<std::vector<BoidPosition>>();
	static auto kdtree = make_resource<core::algo::kdtree<BoidPosition>>();
	{
		CopyComponent<Translation>(ppl, boidFilter, positions, PassLocation(CopyPosition));
		shared_entry shareList[] = { read(positions), write(kdtree) };
		task_system::ecs::schedule_custom(ppl, ppl.create_custom_pass(PassLocation(BoidKdTree), shareList), []() mutable
			{
				kdtree->initialize(*positions);
			});
	}

	//收集目标和障碍物
	static auto targets = make_resource<std::vector<BoidPosition>>();
	static auto targetEntities = make_resource<std::vector<entity>>();
	static auto targetTree = make_resource<core::algo::kdtree<BoidPosition>>();
	{
		CopyComponent<Translation>(ppl, targetFilter, targets, PassLocation(CopyTargetPosition));
		CollectEntites(ppl, targetFilter, targetEntities, PassLocation(CopyTargetEntities));
		shared_entry shareList[] = { read(targets), write(targetTree) };
		task_system::ecs::schedule_custom(ppl, ppl.create_custom_pass(PassLocation(TargetKdTree), shareList), []() mutable
			{
				targetTree->initialize(*targets);
			});
	}
	//计算新的朝向
	static auto headings = make_resource<std::vector<sakura::Vector3f>>();
	static auto newHeadings = make_resource<std::vector<sakura::Vector3f>>();
	{
		CopyComponent<Heading>(ppl, boidFilter, headings, PassLocation(CopyHeading));
		shared_entry shareList[] = { read(kdtree), read(headings), read(targetTree), read(targetEntities), write(newHeadings) };
		def paramList = 
			boost::hana::make_tuple( param<const Heading>, param<const Translation>, param<const Boid>, param<BoidDebugData>);
		auto pass = ppl.create_pass(boidFilter, paramList, PassLocation(BoidMain), shareList);
		task_system::ecs::schedule_init(ppl, pass,
			[](const task_system::ecs::pass& pass) mutable
			{
				newHeadings->resize(pass.calc_size());
			},
			[deltaTime](const task_system::ecs::pass& pass, const ecs::task& tk) mutable
			{
				auto o = operation{ paramList, pass, tk };
				auto index = o.get_index();
				auto [hds, trs, dbg] = o.get_parameters_owned<const Heading, const Translation, BoidDebugData>();
				auto boid = o.get_parameter<const Boid>(); //这玩意是 shared
				std::vector<std::pair<float, int>> neighbers;
				neighbers.reserve(10);
				chunk_vector<sakura::Vector3f> alignments;
				chunk_vector<sakura::Vector3f> separations;
				chunk_vector<sakura::Vector3f> targetings;
				chunk_vector<uint32_t> neighborCount;
				alignments.resize(o.get_count());
				separations.resize(o.get_count());
				targetings.resize(o.get_count());
				neighborCount.resize(o.get_count());
				{
					ZoneScopedN("Collect Neighbors");
					forloop(i, 0, o.get_count())
					{
						//收集附近单位的位置和朝向信息
						neighbers.clear();
						kdtree->search_k_radius(trs[i], boid->SightRadius, 9, neighbers);
						alignments[i] = sakura::Vector3f::vector_zero();
						separations[i] = sakura::Vector3f::vector_zero();
						for (auto ng : neighbers)
						{
							auto location = (*kdtree)[ng.second].value;
							//assert(math::distance(location, trs[i]) < boid->SightRadius);
							alignments[i] = alignments[i] + (*headings)[ng.second];
							separations[i] = separations[i] + location;
						}
						neighborCount[i] = neighbers.size();
						dbg[i].neighborCount = neighbers.size() - 1;
						averageNeighberCount += neighbers.size() - 1;
						update_maximum(maxNeighberCount, neighbers.size() - 1);
					}
				}

				{
					ZoneScopedN("Collect Targets");
					forloop(i, 0, o.get_count())
					{
						//寻找一个目标
						int id = targetTree->search_nearest(trs[i]);
						if (id != -1)
						{
							dbg[i].following = (*targetEntities)[id];
							targetings[i] = (*targetTree)[id].value;
						}
					}
				}
				
				{
					ZoneScopedN("Calculate Boids");
					forloop(i, 0, o.get_count())
					{
						//Boid 算法
						sakura::Vector3f alignment = math::normalize(alignments[i] / (float)neighborCount[i] - hds[i]);
						sakura::Vector3f separation = math::normalize((float)neighborCount[i] * trs[i] - separations[i]);
						sakura::Vector3f targeting = math::normalize(targetings[i] - trs[i]);
						sakura::Vector3f newHeading = math::normalize(alignment * boid->AlignmentWeight + separation * boid->SeparationWeight + targeting * boid->TargetWeight);
						(*newHeadings)[index + i] = math::normalize((hds[i] + (newHeading - hds[i]) * deltaTime * boid->TurnSpeed));
					}
				}
			}, 100);
	}
	//结果转换
	{
		FillComponent<Heading>(ppl, boidFilter, newHeadings, PassLocation(CopyBackHeading));
	}
}


float Radius = 1500.f;
float TimeScale = 1.f;
sakura::ecs::entity BoidSettingEntity;
Boid BoidSetting;

void SpawnBoidTargets(int Count)
{
	LeaderCount = Count;
	using namespace sakura::ecs;
	//创建 Boid 目标
	entity_type type
	{
		complist<BoidTarget, Translation, LocalToWorld, MoveToward, RandomMoveTarget, Scale>
	};
	for (auto slice : ctx.allocate(type, Count))
	{
		auto trs = init_component<Translation>(ctx, slice);
		auto mts = init_component<MoveToward>(ctx, slice);
		auto rmts = init_component<RandomMoveTarget>(ctx, slice);
		auto ss = init_component<Scale>(ctx, slice);

		forloop(i, 0, slice.count)
		{
			std::uniform_real_distribution<float> speedDst(200.f, 300.f);
			rmts[i].center = sakura::Vector3f(0, 0, 500.f);
			rmts[i].radius = Radius;
			mts[i].Target = rmts[i].random_point(get_random_engine());
			mts[i].MoveSpeed = speedDst(get_random_engine());
			trs[i] = rmts[i].random_point(get_random_engine());
			ss[i] = sakura::Vector3f(15.f, 15.f, 15.f);
		}
	}
}

void SpawnBoidSetting()
{
	using namespace sakura::ecs;

	//创建 Boid 设置
	entity_type type
	{
		complist<Boid>
	};
	for (auto slice : ctx.allocate(type, 1))
	{
		auto bs = init_component<Boid>(ctx, slice);
		*bs = BoidSetting;
		BoidSettingEntity = ctx.get_entities(slice.c)[slice.start];
	}
}

core::entity GetBoidSetting(sakura::ecs::pipeline& ppl)
{
	using namespace sakura::ecs;

	archetype_filter filter = { complist<Boid>, {}, {}, {}, {}, complist<Boid> };
	for (auto pf : ppl.query(filter))
		for (auto i : ppl.query(pf.type))
			return ppl.get_entities(i)[0];
}

void SpawnBoids(task_system::ecs::pipeline& ppl, int Count)
{
	using namespace sakura::ecs;
	//创建 Boid
	entity_type type
	{
		complist<Translation, Heading, LocalToWorld, BoidDebugData>,
		{&BoidSettingEntity, 1}
	};
	sphere s;
	s.center = sakura::Vector3f(0, 0, 500.f);
	s.radius = Radius;
	for (auto slice : ppl.allocate(type, Count))
	{
		auto& ctx = (world&)ppl; //warning! 解放!
		auto trs = init_component<Translation>(ctx, slice);
		auto hds = init_component<Heading>(ctx, slice);
		forloop(i, 0, slice.count)
		{
			std::uniform_real_distribution<float> uniform_dist(-1, 1);
			auto& el = get_random_engine();
			sakura::Vector3f vector{ uniform_dist(el), uniform_dist(el), uniform_dist(el) };
			vector = math::normalize(vector);
			hds[i] = vector;
			//hds[i] = -vector;
			trs[i] = sakura::Vector3f(0, 0, 500.f);
			//trs[i] = sakura::Vector3f(0, 0, 500.f)+ vector*2000;
			//trs[i] = s.random_point(el);
		}
	}
}

void DestroyBoids(task_system::ecs::pipeline& ppl, int Count)
{
	using namespace sakura::ecs;
	filters filter;
	filter.archetypeFilter = {
		{complist<LocalToWorld, Boid>}, //all
		{}, //any
		{} //none
	};
	for (auto at : ppl.query(filter.archetypeFilter))
	{
		ppl.sync_archetype(at.type);
		auto& ctx = (world&)ppl; //warning! 解放!
		for (auto j : ctx.query(at.type))
		{
			uint32_t ToDestory = std::min((uint32_t)Count, j->get_count());
			ctx.destroy({ j, 0, ToDestory });
			Count -= ToDestory;
			if (Count == 0)
				return;
		}
	}
		
}

void UpdateBoidsCount(task_system::ecs::pipeline& ppl, int Count)
{
	if (Count > 0)
		SpawnBoids(ppl, Count);
	else if (Count < 0)
		DestroyBoids(ppl, -Count);
}

int CountBoids(task_system::ecs::pipeline& ppl)
{
	using namespace sakura::ecs;
	filters filter;
	filter.archetypeFilter = {
		{complist<LocalToWorld, Boid>}, //all
		{}, //any
		{} //none
	};
	auto& ctx = (world&)ppl; //warning! 解放!
	int counter = 0;
	for (auto at : ctx.query(filter.archetypeFilter))
		for (auto j : ctx.query(at.type))
			counter += j->get_count();
	return counter;
}

bool bEnableBoids = true;
bool bEnableBoidsMove = true;
bool bEnableL2W = true;

void BoidMainLoop(task_system::ecs::pipeline& ppl, float deltaTime)
{
	using namespace sakura::ecs;
	ZoneScopedN("Schedule Systems")

	RandomTargetSystem(ppl);
	MoveTowardSystem(ppl, deltaTime);
	if (bEnableBoids)
	{
		BoidsSystem(ppl, deltaTime);
	}
	if (bEnableBoidsMove)
	{

		BoidMoveSystem(ppl, deltaTime);
	}
	if (bEnableL2W)
	{
		filters wrd_filter;
		wrd_filter.archetypeFilter = {
			{complist<LocalToWorld>},
			{complist<Translation, Scale, Rotation>},
			{complist<LocalToParent, Parent>}
		};
		Local2XSystem<LocalToWorld>(ppl, wrd_filter, PassLocation(LocalToWorld));
	}


	filters c2p_filter;
	c2p_filter.archetypeFilter = {
		{complist<LocalToParent, Parent>},
		{complist<Translation, Scale, Rotation>},
		{}
	};
	Local2XSystem<LocalToParent>(ppl, c2p_filter, PassLocation(LocalToParent));
	Child2WorldSystem(ppl);
	World2LocalSystem(ppl);
}

const bool bUseImGui = true;
bool bUseSnapshot = false;
float SnaphotRate = 30.f;
bool bNoGroupParallel = false;
bool bEnableGameInput = false;
bool bNoFibers = false;
float CameraSpeed = 400.f;
float RotationRate = 1.f;
sakura::graphics::RenderPassHandle imgui_pass = sakura::GenerationalId::UNINITIALIZED;
sakura::graphics::RenderCommandBuffer imgui_command_buffer("ImGuiRender", 4096);
int main()
{
	if (!IModule::Registry::regist("ECS", &ECSModule::create) || !sakura::IModule::StartUp("ECS"))
	{
		sakura::error("Failed to StartUp ECSModule!");
		return -1;
	}
	if (!IModule::Registry::regist("RenderGraph", &sakura::graphics::RenderGraphModule::create) || !sakura::IModule::StartUp("RenderGraph"))
	{
		sakura::error("Failed to StartUp RG!");
		return -1;
	}
	Timer timer;
	double deltaTime = 1.f / 60.f;
	size_t cycle = 0;
	task_system::Scheduler scheduler(task_system::Scheduler::Config::allCores());
	scheduler.bind();
	task_defer(scheduler.unbind());  // Automatically unbind before returning.
	sakura::graphics::RenderCommandBuffer buffer = sakura::graphics::RenderCommandBuffer("", 4096 * 8 * 16 * 32);
	
	
	using namespace sakura::ecs;

	BoidSetting.AlignmentWeight = 3.f;
	BoidSetting.TargetWeight = 1.f;
	BoidSetting.SeparationWeight = 5.f;
	BoidSetting.TurnSpeed = 1.5f;
	BoidSetting.MoveSpeed = 200.f;
	BoidSetting.SightRadius = 25.f;
	declare_components<Rotation, Translation, RotationEuler, Scale, LocalToWorld, LocalToParent,
		WorldToLocal, Child, Parent, Boid, BoidTarget, MoveToward, RandomMoveTarget, Heading, BoidDebugData>();
	SpawnBoidSetting();
	SpawnBoidTargets(10);
	
	task_system::ecs::pipeline ppl(std::move(ctx));
	if (!bUseImGui)
	{
		SpawnBoids(ppl, TARGET_NUM);
	}
	render_system::initialize();
	if(bUseImGui)
	{
		using namespace render_system;
		// ImGui
		imgui::initialize(main_window);
		imgui::initialize_gfx(render_graph, *render_device);
		imgui_pass = render_graph.create_render_pass<imgui::RenderPassImGui>(swap_chain, render_graph);
	}


	bool rolling_back = false;
	uint32_t current_frame = 0;
	int selected_frame = 0;
	std::deque<std::vector<char>> snapshots;
	size_t memory_used = 0;
	int memory_size_limit = 4096;
	float snapshotTimer = 0.f;
	const float snapshotInterval = 1.f / SnaphotRate;
	auto rx_os = sakura::Core::find_messenger(render_system::main_window.handle());
	rx_os->messages<sakura::OSKeyboardMessage>().subscribe([](auto m)
		{
			if (m.message == sakura::SM_KEYUP || m.message == sakura::SM_SYS_KEYUP)
			{
				if (m.key_code == sakura::input::EKeyCode::Backslash)
				{
					bEnableGameInput = !bEnableGameInput;
					if (bEnableGameInput)
						sakura::input::set_cursor(sakura::input::EMouseCursor::Hide);
					else
						sakura::input::set_cursor(sakura::input::EMouseCursor::Arrow);
				}
			
			}
		});
	// Game & Rendering Logic
	while(sakura::Core::yield())
	{
		ZoneScoped;
		timer.start_up(); 
		ppl.force_no_fibers = bNoFibers;
		ppl.force_no_group_parallel = bNoGroupParallel;

		// 结束 GamePlay Cycle 并开始收集渲染信息. 此举动必须在下一帧开始渲染之前完成。
		render_system::CollectAndUpload(ppl, deltaTime);
		render_system::PrepareCommandBuffer(buffer);

		if (!bUseSnapshot)
		{
			BoidMainLoop(ppl, deltaTime * TimeScale);
		}
		else
		{
			if (!rolling_back)
			{
				BoidMainLoop(ppl, deltaTime * TimeScale);
				snapshotTimer += deltaTime;
				ppl.sync_all_ro();
				if (snapshotTimer > snapshotInterval)
				{
					snapshotTimer = 0.f;
					ZoneScopedN("Take Snapshot");
					auto& ctx = (ecs::world&)ppl;
					std::vector<char> snapshot;
					if (snapshots.size() > 0)
						snapshot.reserve(snapshots.back().size());
					buffer_serializer archive(snapshot);
					ctx.serialize(&archive);
					memory_used += snapshot.size();
					snapshots.emplace_back(std::move(snapshot));
					while (memory_used > (long long)memory_size_limit * 1024 * 1024)
					{
						memory_used -= snapshots.front().size();
						snapshots.pop_front();
					}
					selected_frame = current_frame = snapshots.size();
				}
			}
			else if (selected_frame != current_frame)
			{
				snapshotTimer = 0.f;
				ppl.sync_all();
				ZoneScopedN("Load Snapshot");
				auto& ctx = (ecs::world&)ppl;
				auto data = snapshots[selected_frame].data();
				buffer_deserializer archive(data);
				ctx.deserialize(&archive);
				BoidSettingEntity = GetBoidSetting(ppl);
				BoidSetting = *(const Boid*)ctx.get_component_ro(BoidSettingEntity, cid<Boid>);
				current_frame = selected_frame;
			}
		}

		if(bEnableGameInput)
		{
			using namespace sakura::input;
			float forward = 0.f, right = 0.f;
			if (key_down(EKeyCode::W))
				forward += 1.f;
			if (key_down(EKeyCode::S))
				forward -= 1.f;
			if (key_down(EKeyCode::A))
				right -= 1.f;
			if (key_down(EKeyCode::D))
				right += 1.f;
			render_system::CameraPos += forward * render_system::GetForwardVector() * deltaTime * CameraSpeed;
			render_system::CameraPos += right * render_system::GetRightVector() * deltaTime * CameraSpeed;
			static auto LastCursorPos = cursor_pos();
			auto cursorPos = cursor_pos();
			render_system::Yaw += ((float)cursorPos.x - LastCursorPos.x)/1920.f * RotationRate;
			render_system::Pitch += ((float)cursorPos.y - LastCursorPos.y)/1080.f * RotationRate;
			render_system::Pitch = std::clamp(render_system::Pitch, -179.5f, 179.5f);
			cursorPos = { 1920 / 2, 1080 / 2 };
			set_cursor_pos(cursorPos.x, cursorPos.y);
			LastCursorPos = cursorPos;
		}

		// IMGUI
		if (bUseImGui)
		{
			using namespace render_system;
			static float f = 0.0f;

			ZoneScopedN("ImGui Command");
			imgui::new_frame(main_window, deltaTime);
			imgui::Begin("Boid Settings");
			static std::mutex fileLock;
			if (imgui::CollapsingHeader("Camera"))
			{
				imgui::InputFloat("Speed", &CameraSpeed, 1.f, 10.f);
				imgui::InputFloat("RotationRate", &RotationRate, 1.f, 10.f);
				std::tuple Rot = { render_system::Pitch, render_system::Yaw , render_system::Roll };
				imgui::InputFloat3("Rotation", (float*)&Rot);
				std::tie(render_system::Pitch, render_system::Yaw, render_system::Roll) = Rot;
				imgui::InputFloat3("Location", render_system::CameraPos.data_view().data());

			}

			if (imgui::CollapsingHeader("SnapShot"))
			{
				if (imgui::Button("Save Snapshot", ImVec2(120, 20)))
				{
					ppl.sync_all_ro();
					auto& ctx = (ecs::world&)ppl;
					std::vector<char> snapshot;
					buffer_serializer archive(snapshot);
					ctx.serialize(&archive);
					std::thread writeThread([snapshot = std::move(snapshot)]()
					{
						auto file = std::fstream("test.snapshot", std::ios::out | std::ios::binary);
						if (file)
						{
							std::unique_lock _(fileLock);
							file.write(snapshot.data(), snapshot.size());
						}
					});
					writeThread.detach();
				}
				imgui::SameLine();
				if (imgui::Button("Load Snapshot", ImVec2(120, 20)))
				{
					auto file = std::fstream("test.snapshot", std::ios::in | std::ios::binary);
					if (file)
					{
						ppl.sync_all();
						auto& ctx = (ecs::world&)ppl;
						fileLock.lock();
						file.seekg(0, std::ios_base::end);
						auto size = file.tellg();
						std::unique_ptr<char[]> data(new char[size]);
						file.seekg(0, std::ios_base::beg);
						file.read(data.get(), size);
						file.close();
						fileLock.unlock();
						buffer_deserializer archive(data.get());
						ctx.deserialize(&archive);
						BoidSettingEntity = GetBoidSetting(ppl);
						BoidSetting = *(const Boid*)ctx.get_component_ro(BoidSettingEntity, cid<Boid>);
						BoidCount = CountBoids(ppl);
					}
				}
				if (imgui::Checkbox("Snapshot Every Frame", &bUseSnapshot) && !bUseSnapshot)
				{
					rolling_back = false;
					memory_used = 0;
					snapshots.clear();
				};
				if (bUseSnapshot)
				{
					if (imgui::SliderInt("Frame", &selected_frame, 0, snapshots.size() - 1))
						rolling_back = true;
					imgui::SameLine();
					if (imgui::Button(rolling_back ? "Play" : "Pause", ImVec2(-1, 20)))
					{
						rolling_back = !rolling_back;
						while (snapshots.size() > (current_frame + 1))
						{
							memory_used -= snapshots.back().size();
							snapshots.pop_back();
						}
						// Boids 的生成可能被回滚了
						BoidCount = CountBoids(ppl);
					}
					imgui::InputInt("Memory Usage", &memory_size_limit, 256);
					imgui::SameLine();
					imgui::ProgressBar((float)memory_used / ((double)memory_size_limit * 1024 * 1024));
				}
			}
			
			if (imgui::CollapsingHeader("Parallel"))
			{
				imgui::Checkbox("Close Fibers Dispatch", &bNoFibers);
				imgui::Checkbox("Close Group Parallel", &bNoGroupParallel);
			}

			if (imgui::CollapsingHeader("Render"))
			{
				imgui::SliderFloat4("Light Direction", render_system::passCB.lightdir, -1.f, 1.f);
				imgui::ColorEdit4("Light Color", render_system::passCB.lightcolor, ImGuiColorEditFlags_Float);
				imgui::ColorEdit4("Ambient Color", render_system::passCB.ambient, ImGuiColorEditFlags_Float);
				const char* DebugModeNames[] = { "NeighborCount", "FollowingLeader", "UniqueColor" };
				imgui::ListBox("Debug Mode", (int*)&DebugMode, DebugModeNames, 3);
			}
			
			if (imgui::CollapsingHeader("Boids"))
			{
				imgui::Checkbox("Enable Boids", &bEnableBoids);
				imgui::Checkbox("Enable Boids Move", &bEnableBoidsMove);
				imgui::Checkbox("Enable L2W", &bEnableL2W);
				bool UpdateBoid = false;
				UpdateBoid |= imgui::SliderFloat("AlignmentWeight", &BoidSetting.AlignmentWeight, 0, 10);
				UpdateBoid |= imgui::SliderFloat("TargetWeight", &BoidSetting.TargetWeight, 0, 10);
				UpdateBoid |= imgui::SliderFloat("SeparationWeight", &BoidSetting.SeparationWeight, 0, 10);
				UpdateBoid |= imgui::SliderFloat("TurnSpeed", &BoidSetting.TurnSpeed, 0, 5);
				UpdateBoid |= imgui::SliderFloat("MoveSpeed", &BoidSetting.MoveSpeed, 0, 1000);
				UpdateBoid |= imgui::SliderFloat("SightRadius", &BoidSetting.SightRadius, 5, 50);
				if (UpdateBoid)
				{
					*(Boid*)ppl.get_owned_rw(BoidSettingEntity, cid<Boid>) = BoidSetting;
				}
				imgui::SliderFloat("TimeScale", &TimeScale, 0, 10);
				int prevBoidCount = BoidCount;
				if (!rolling_back && imgui::SliderInt("BoidCount", &BoidCount, 0, TARGET_NUM))
				{
					UpdateBoidsCount(ppl, BoidCount - prevBoidCount);
				}
				if (BoidCount)
				{
					imgui::Text("Max Neighbor Count: %d", maxNeighberCount.load());
					imgui::Text("Average Neighbor Count: %d", averageNeighberCount.load() / BoidCount);
				}
			}

			if (imgui::CollapsingHeader("Inspector"))
			{

			}
			//if (imgui::SliderInt("LeaderCount", &LeaderCount, 0, 100)){}

			//imgui::SameLine();

			imgui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / imgui::GetIO().Framerate, imgui::GetIO().Framerate);
			imgui::End();
			imgui::Render();
		}
		averageNeighberCount.store(0);
		cycle += 1;
		{
			ZoneScopedN("Render");
			using namespace render_system;

			// 开始渲染已经准备好的那帧 Command Buffer, 目前 Compile 内联在渲染系统中.
			render_system::Render(buffer); // 0 + 1

			// 渲染ImGui
			if (bUseImGui)
			{
				ZoneScopedN("ImGui Render");

				RenderPass* pass_ptr = render_graph.render_pass(imgui_pass);
				pass_ptr->construct(render_graph.builder(imgui_pass), *render_device);
				imgui_command_buffer.reset();
				pass_ptr->execute(imgui_command_buffer, render_graph, *render_device);
				render_device->execute(imgui_command_buffer, pass_ptr->handle());
			}

			// Present
			render_system::Present();
		}

		if (cycle % 60 == 0)
			render_system::main_window.set_title(fmt::format(L"SakuraEngine: {:.2f} FPS", 1.0 / deltaTime).c_str());

		deltaTime = timer.end();

		FrameMark;
		ppl.mark_frame();
	}
	ctx = ppl.release();
}