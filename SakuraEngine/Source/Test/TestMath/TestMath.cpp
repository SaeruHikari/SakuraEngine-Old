#include "Math/Math.hpp"
#include <cassert>

#pragma optimize("", off)

int main(void)
{
	using namespace sakura;
	using namespace sakura::math::__vector;
	
	alignas(16) const float a[] = { 1, 0, 0, 1 };
	const float aa[] = { 1, 0, 0 };
	const float b[] = { 0, 0, 1, 1 };
	alignas(16) float c[] = { 0, 0, 1, 1 };
 
	auto vec_reg = vector_register(1.f, 2.f, 3.f, 4.f);
	auto vec2_reg = load_float3_w0(aa);
	auto vec3_reg = load_aligned(a);
	auto vec4_reg = load(b);

	// Expect 1, 2, 3, 4
	Vector4f vector;
	store_aligned(vector.data_view(), vec_reg);
	assert(vector == Vector4f(1, 2, 3, 4));

	Vector3f v3 = Vector3f(1.f, 1.f, 1.f);

	// Expect 3, 4, 0, 0
	Vector4f shuffled;
	store_aligned(shuffled.data_view(), shuffle<2, 3, 2, 3>(vec_reg, vec2_reg));
	assert(shuffled == Vector4f(3.f, 4.f, 0.f, 0.f));

	// Expect 4, 2, 3, 1
	Vector4f swizzled;
	store_aligned(swizzled.data_view(), swizzle<3, 1, 2, 0>(vec_reg));
	assert(swizzled == Vector4f(4.f, 2.f, 3.f, 1.f));

	// Expect 4, 0, 1, 2
	Vector4f permuted;
	store_aligned(permuted.data_view(), permute<3, 5, 4, 1>(vec_reg, vec2_reg));
	assert(permuted == Vector4f(4.f, 0.f, 1.f, 2.f));

	// Expect -4, 0, -1, -2
	Vector4f neg;
	store_aligned(neg.data_view(),
		sakura::math::__vector::negate(load_aligned(permuted.data_view())));
	assert(neg == Vector4f(-4.f, 0.f, -1.f, -2.f));

	// Expect 4, 0, 1, 2
	Vector4f abs;
	store_aligned(abs.data_view(),
		sakura::math::__vector::abs(load_aligned(neg.data_view())));
	assert(abs == Vector4f(4.f, 0.f, 1.f, 2.f));

	// Expect 0, 0, 0, 0
	Vector4f add;
	store_aligned(add.data_view(),
		sakura::math::__vector::add(
			load_aligned(neg.data_view()), load_aligned(abs.data_view())
	));
	assert(add == Vector4f(0.f, 0.f, 0.f, 0.f));

	// Expect 0, 0, 0, 0
	Vector4f sub;
	store_aligned(sub.data_view(),
		sakura::math::__vector::subtract(
			load_aligned(permuted.data_view()), load_aligned(abs.data_view())
	));
	assert(sub == Vector4f(0.f, 0.f, 0.f, 0.f));

	// Expect 16, 0, 1, 4
	Vector4f mul;
	store_aligned(mul.data_view(),
		sakura::math::__vector::multiply(
			load_aligned(permuted.data_view()), load_aligned(abs.data_view())
	));
	assert(mul == Vector4f(16.f, 0.f, 1.f, 4.f));

	// Expect 4, -nan, 1, 2
	Vector4f div;
	store_aligned(div.data_view(),
		sakura::math::__vector::divide(
			load_aligned(mul.data_view()), load_aligned(abs.data_view())
	));

	// Expect 20, 0, 2, 6
	Vector4f mul_add;
	store_aligned(mul_add.data_view(),
		sakura::math::__vector::multiply_add(
			load_aligned(permuted.data_view()),
			load_aligned(abs.data_view()),
			load_aligned(permuted.data_view()
		)
	));
	assert(mul_add == Vector4f(20.f, 0.f, 2.f, 6.f));

	// Expect 16, 16, 16, 16
	Vector4f dot2;
	store_aligned(dot2.data_view(),
		sakura::math::__vector::dot2(
			load_aligned(permuted.data_view()), load_aligned(abs.data_view())
	));
	assert(dot2 == Vector4f(16.f, 16.f, 16.f, 16.f));

	// Expect 17, 17, 17, 17
	Vector4f dot3;
	store_aligned(dot3.data_view(),
		sakura::math::__vector::dot3(
			load_aligned(permuted.data_view()), load_aligned(abs.data_view())
	));
	assert(dot3 == Vector4f(17.f, 17.f, 17.f, 17.f));

	// Expect 21, 21, 21, 21
	Vector4f dot4;
	store_aligned(dot4.data_view(),
		sakura::math::__vector::dot4(
			load_aligned(permuted.data_view()), load_aligned(abs.data_view())
	));
	assert(dot4 == Vector4f(21.f, 21.f, 21.f, 21.f));

	auto dot44 = sakura::math::dot_product(permuted, abs);

	// Expect 0, -nan, 0, -nan
	Vector4f eq;
	store_aligned(eq.data_view(), sakura::math::__vector::equals(vec3_reg, vec4_reg));

	// Expect -nan, 0, -nan, 0
	Vector4f neq;
	store_aligned(neq.data_view(), sakura::math::__vector::not_equals(vec3_reg, vec4_reg));

	// 1010 ->  10
	int msk = component_mask(load_aligned(eq.data_view()));

	// Expect 0, -1, 0, 0
	Vector4f crs;
	store_aligned(crs.data_view(), sakura::math::__vector::cross_product(
		load(std::array<float, 4>{1, 0, 0, 1}),
		load(std::array<float, 4>{ 0, 0, 1, 1 })
	));
	assert(crs == Vector4f(0.f, -1.f, 0.f, 0.f));

	// Expect 256, 1, 1, 4
	Vector4f pow;
	store_aligned(pow.data_view(),
		sakura::math::__vector::power(
			load_aligned(abs.data_view()),
			load_aligned(abs.data_view())
	));

	// Expect 1/16, 1, 1, 1/2
	Vector4f rsqrt;
	store_aligned(rsqrt.data_view(),
		sakura::math::__vector::reciprocal_sqrt(
			load_aligned(pow.data_view())
	));
	// Expect 1/16, 1, 1, 1/2
	Vector4f rsqrt_quick;
	store_aligned(rsqrt_quick.data_view(),
		sakura::math::__vector::reciprocal_sqrt_quick(
			load_aligned(pow.data_view())
	));

	// Expect 1/256, 1, 1, 1/4
	Vector4f r;
	store_aligned(r.data_view(),
		sakura::math::__vector::reciprocal(
			load_aligned(pow.data_view())
		));
	// Expect 1/256, 1, 1, 1/4
	Vector4f r_quick;
	store_aligned(r_quick.data_view(),
		sakura::math::__vector::reciprocal_quick(
			load_aligned(pow.data_view())
	));

	// Expect 1/length, 1/length, 1/length, 1/length
	Vector4f rl;
	store_aligned(rl.data_view(),
		sakura::math::__vector::reciprocal_length(
			load_aligned(pow.data_view())
	));
	// Expect 1/length, 1/length, 1/length, 1/length
	Vector4f rl_quick;
	store_aligned(rl_quick.data_view(),
		sakura::math::__vector::reciprocal_length_quick(
			load_aligned(pow.data_view())
	));

	// Expect normalized(4, 0, 1, 2)
	Vector4f n;
	store_aligned(n.data_view(),
		sakura::math::__vector::normalize(
			load_aligned(abs.data_view())
	));
	// Expect normalized(4, 0, 1, 2)
	Vector4f n_quick;
	store_aligned(n_quick.data_view(),
		sakura::math::__vector::normalize_quick(
			load_aligned(abs.data_view())
	));

	// Expect 3, 0, 0, 0
	Vector4f min;
	store_aligned(min.data_view(),
		sakura::math::__vector::min(
			load_aligned(shuffled.data_view()), // 3 4 0 0
			load_aligned(abs.data_view()) // 4 0 1 2
	));
	// Expect 4, 4, 1, 2
	Vector4f max;
	store_aligned(max.data_view(),
		sakura::math::__vector::max(
			load_aligned(shuffled.data_view()), // 3 4 0 0
			load_aligned(abs.data_view()) // 4 0 1 2
	));

	// Expect TRUE.
	bool containsNanInfinite = contains_nan_or_infinite(load_aligned(eq.data_view()));

	// Expect 8, 1, 1, 1
	Vector4f exp2;
	store_aligned(exp2.data_view(),
		sakura::math::__vector::exp2(
			load_aligned(min.data_view())
	));

	auto vec4_one = Vector4f::vector_one();
	auto vec3_one = Vector3f::vector_one();
	auto location0 = Vector3lf::vector_zero();
	auto location1 = Vector3lf::vector_one();
	
	sakura::Transform transform(
		Quaternion(0.f, 0.500f, 0.f, 0.866f),
		Vector3f::vector_zero(),
		Vector3f::vector_one()
	);

	//Matrix4x4 transform_matrix = __matrix.to_matrix();
	
	Quaternion q(0.f, 0.500f, 0.f, 0.866f); // quat3 yaw 60
	Quaternion q1(0.500f, 0.f, 0.f, 0.866f); // quat2 pitch 60
	Quaternion q2(0.f, 0.f, 0.500f, 0.866f); // quat roll 60
	Quaternion q3(0.f, 0.866f, 0.f, 0.5f); // quat4 yaw 120
	//Rotator rrr= q.rotator();
	Quaternion roll60  =  // 0, 0, 0.5, 0.866
		math::quaternion_from_euler(0, 0, 3.14159f / 3.f);
	Quaternion pitch60 =  // 0.5, 0, 0, 0.866
		math::quaternion_from_euler(3.14159f / 3.f, 0, 0);
	Quaternion yaw60 =  // 0, 0.5, 0, 0.866
		math::quaternion_from_euler(0.f, 3.14159f / 3.f, 0.f);
	Quaternion yaw120 =  // 0, 0.866, 0, 0.5
		math::quaternion_from_euler(0.f, 3.14159f * 2.f / 3.f, 0.f);
	
	Rotator rr = Rotator{ 0.f, 0.f, 0.f };

	float dis = math::distance(v3, v3);

	auto nt = sakura::math::normalize(Vector3f(1.f, 1.f, 1.f));

	bool end = true;
	if(end)
	{
		return 0;
	}
}

#pragma optimize("", on)