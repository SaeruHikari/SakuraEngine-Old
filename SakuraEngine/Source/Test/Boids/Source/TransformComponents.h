#pragma once
#include <set>
#include "ECS/ECS.h"
#include "Math/Math.hpp"

using namespace core::guid_parse::literals;
namespace ecs = sakura::ecs;
struct Translation
{
	using value_type = sakura::Vector3f;
	static constexpr auto guid = "1FA307F7-F6A4-4F75-895F-7BDF25A361A6"_guid;
	sakura::Vector3f value;
};

struct Rotation
{
	using value_type = sakura::Quaternion;
	static constexpr auto guid = "D694D636-FE05-4382-9543-1FE9517B14D6"_guid;
	sakura::Quaternion value;
};

struct RotationEuler
{
	using value_type = sakura::Rotator;
	static constexpr auto guid = "1D6AC7B7-EDDF-480D-BFCF-57ABCB16E27A"_guid;
	sakura::Rotator value;
};

struct Scale
{
	using value_type = sakura::Vector3f;
	static constexpr auto guid = "6DD253F8-D5B2-42BF-BC26-90E5085175A1"_guid;
	sakura::Vector3f value;
};

struct LocalToWorld
{
	using value_type = sakura::float4x4;
	static constexpr auto guid = "CFEE8C60-3EE0-40F5-893A-5FCE162383C9"_guid;
	sakura::float4x4 value;
};

struct LocalToParent
{
	using value_type = sakura::float4x4;
	static constexpr auto guid = "9E00BCCA-FF74-4B91-AD71-11282CAB06BF"_guid;
	sakura::float4x4 value;
};

struct WorldToLocal
{
	using value_type = sakura::float4x4;
	static constexpr auto guid = "3C572C40-41B6-400C-819C-E4629D080588"_guid;
	sakura::float4x4 value;
};

struct Child
{
	using value_type = ecs::buffer_t<ecs::entity>;
	static constexpr auto buffer_capacity = 10;
	static constexpr auto guid = "BC7F6B6A-2231-4B33-BD5E-076DE47FFEBC"_guid;
	sakura::ecs::entity value;
};

struct Parent
{
	using value_type = ecs::entity;
	static constexpr auto guid = "3C9B48E0-F5E0-4251-94E5-A5F04DF9706E"_guid;
	sakura::ecs::entity value;
};

