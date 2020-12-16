#include <set>
#include "ECS/ECS.h"
#include "Math/Math.hpp"
#include <cmath>
#include <random>

using namespace core::guid_parse::literals;
namespace ecs = sakura::ecs;

struct Boid
{
	float SightRadius;
	float SeparationWeight;
	float AlignmentWeight;
	float TargetWeight;
	float MoveSpeed;

	static constexpr auto guid = "A8C09FA6-F29C-4477-9B90-75F8F5490380"_guid;
};

struct BoidTarget
{
	static constexpr auto guid = "86EA5D88-3885-469B-BE73-5DB1B20BD817"_guid;
};

struct Heading
{
	using value_type = sakura::Vector3f;
	static constexpr auto guid = "BA82CDD4-56B8-40C5-BD5B-7AD8CFF867C4"_guid;
	sakura::Vector3f value;
};

struct MoveToward
{
	sakura::Vector3f Target;
	static constexpr auto guid = "6A1B5112-1477-4624-A4C1-E1693F236D76"_guid;
	float MoveSpeed;
};

struct sphere
{
	sakura::Vector3f center;
	float radius;

	template <class E>
	sakura::Vector3f random_point(E& el)
	{
		std::uniform_real_distribution<float> a(-1, 1);
		std::uniform_real_distribution<float> b(0, 1);
		sakura::Vector3f vector{ a(el), a(el), a(el) };
		float scale = std::cbrt(b(el)) * radius;
		return center + sakura::math::normalize(vector) * scale;
	}
};

struct RandomMoveTarget
{
	using value_type = sphere;
	sphere value;
	static constexpr auto guid = "7C3EE662-B044-429E-9CF2-B03DC23B3AA4"_guid;
};