#include "RuntimeCore/RuntimeCore.h"

int main(void)
{
	using namespace sakura;
	using namespace sakura::math::vector;
	
	alignas(16) const float a[] = { 1, 0, 0, 1 };
	const float aa[] = { 1, 0, 0 };
	const float b[] = { 0, 0, 1, 1 };
	alignas(16) float c[] = { 0, 0, 1, 1 };
	vector3_cross(a, b, c);
	//sakura::info("plain vector3_cross: {}, {}, {}", c[0], c[1], c[2]);

	auto vec_reg = vector_register(1.f, 2.f, 3.f, 4.f);
	auto vec2_reg = load_float3_w0(aa);
	auto vec3_reg = load_aligned(a);
	auto vec4_reg = load(b);

	// Expect 1, 2, 3, 4
	Vector4f vector;
	store_aligned(vector.data_view(), vec_reg);

	Vector3f v3 = Vector3f(1.f, 1.f, 1.f);
	// Vector3 Operators Test
	{
		Vector3f add = v3 + v3;
		Vector3f sub = v3 - v3;
		Vector3f mul = v3 * v3;
		Vector3f div = v3 / v3;
		Vector3f cross = v3 ^ v3;
		float dot = v3 | v3;
	}

	// Expect 3, 4, 0, 0
	Vector4f shuffled;
	store_aligned(shuffled.data_view(), shuffle<2, 3, 2, 3>(vec_reg, vec2_reg));

	// Expect 4, 2, 3, 1
	Vector4f swizzled;
	store_aligned(swizzled.data_view(), swizzle<3, 1, 2, 0>(vec_reg));

	// Expect 4, 0, 1, 2
	Vector4f permuted;
	store_aligned(permuted.data_view(), permute<3, 5, 4, 1>(vec_reg, vec2_reg));

	// Expect -4, 0, -1, -2
	Vector4f neg;
	store_aligned(neg.data_view(),
		sakura::math::vector::negate(load_aligned(permuted.data_view())));

	// Expect 4, 0, 1, 2
	Vector4f abs;
	store_aligned(abs.data_view(),
		sakura::math::vector::abs(load_aligned(neg.data_view())));

	// Expect 0, 0, 0, 0
	Vector4f add;
	store_aligned(add.data_view(),
		sakura::math::vector::add(
			load_aligned(neg.data_view()), load_aligned(abs.data_view())
	));
	
	// Expect 0, 0, 0, 0
	Vector4f sub;
	store_aligned(sub.data_view(),
		sakura::math::vector::subtract(
			load_aligned(permuted.data_view()), load_aligned(abs.data_view())
	));

	// Expect 16, 0, 1, 4
	Vector4f mul;
	store_aligned(mul.data_view(),
		sakura::math::vector::multiply(
			load_aligned(permuted.data_view()), load_aligned(abs.data_view())
	));

	// Expect 4, -nan, 1, 2
	Vector4f div;
	store_aligned(div.data_view(),
		sakura::math::vector::divide(
			load_aligned(mul.data_view()), load_aligned(abs.data_view())
	));

	// Expect 20, 0, 2, 6
	Vector4f mul_add;
	store_aligned(mul_add.data_view(),
		sakura::math::vector::multiply_add(
			load_aligned(permuted.data_view()),
			load_aligned(abs.data_view()),
			load_aligned(permuted.data_view()
		)
	));

	// Expect 16, 16, 16, 16
	Vector4f dot2;
	store_aligned(dot2.data_view(),
		sakura::math::vector::dot4(
			load_aligned(permuted.data_view()), load_aligned(abs.data_view())
	));

	// Expect 17, 17, 17, 17
	Vector4f dot3;
	store_aligned(dot3.data_view(),
		sakura::math::vector::dot3(
			load_aligned(permuted.data_view()), load_aligned(abs.data_view())
	));
	
	// Expect 21, 21, 21, 21
	Vector4f dot4;
	store_aligned(dot4.data_view(),
		sakura::math::vector::dot4(
			load_aligned(permuted.data_view()), load_aligned(abs.data_view())
	));

	// Expect 0, -nan, 0, -nan
	Vector4f eq;
	store_aligned(eq.data_view(), sakura::math::vector::equals(vec3_reg, vec4_reg));

	// Expect -nan, 0, -nan, 0
	Vector4f neq;
	store_aligned(neq.data_view(), sakura::math::vector::not_equals(vec3_reg, vec4_reg));

	// 1010 ->  10
	int msk = component_mask(load_aligned(eq.data_view()));

	// Expect 0, -1, 0, 0
	Vector4f crs;
	store_aligned(crs.data_view(), sakura::math::vector::cross_product(
		load(std::array<float, 4>{1, 0, 0, 1}),
		load(std::array<float, 4>{ 0, 0, 1, 1 })
	));

	// Expect 256, 1, 1, 4
	Vector4f pow;
	store_aligned(pow.data_view(),
		sakura::math::vector::power(
			load_aligned(abs.data_view()),
			load_aligned(abs.data_view())
	));

	// Expect 1/16, 1, 1, 1/2
	Vector4f rsqrt;
	store_aligned(rsqrt.data_view(),
		sakura::math::vector::reciprocal_sqrt(
			load_aligned(pow.data_view())
	));
	// Expect 1/16, 1, 1, 1/2
	Vector4f rsqrt_quick;
	store_aligned(rsqrt_quick.data_view(),
		sakura::math::vector::reciprocal_sqrt_quick(
			load_aligned(pow.data_view())
	));

	// Expect 1/256, 1, 1, 1/4
	Vector4f r;
	store_aligned(r.data_view(),
		sakura::math::vector::reciprocal(
			load_aligned(pow.data_view())
		));
	// Expect 1/256, 1, 1, 1/4
	Vector4f r_quick;
	store_aligned(r_quick.data_view(),
		sakura::math::vector::reciprocal_quick(
			load_aligned(pow.data_view())
	));

	// Expect 1/length, 1/length, 1/length, 1/length
	Vector4f rl;
	store_aligned(rl.data_view(),
		sakura::math::vector::reciprocal_length(
			load_aligned(pow.data_view())
	));
	// Expect 1/length, 1/length, 1/length, 1/length
	Vector4f rl_quick;
	store_aligned(rl_quick.data_view(),
		sakura::math::vector::reciprocal_length_quick(
			load_aligned(pow.data_view())
	));

	// Expect normalized(4, 0, 1, 2)
	Vector4f n;
	store_aligned(n.data_view(),
		sakura::math::vector::normalize(
			load_aligned(abs.data_view())
	));
	// Expect normalized(4, 0, 1, 2)
	Vector4f n_quick;
	store_aligned(n_quick.data_view(),
		sakura::math::vector::normalize_quick(
			load_aligned(abs.data_view())
	));

	// Expect 3, 0, 0, 0
	Vector4f min;
	store_aligned(min.data_view(),
		sakura::math::vector::min(
			load_aligned(shuffled.data_view()), // 3 4 0 0
			load_aligned(abs.data_view()) // 4 0 1 2
	));
	// Expect 4, 4, 1, 2
	Vector4f max;
	store_aligned(max.data_view(),
		sakura::math::vector::max(
			load_aligned(shuffled.data_view()), // 3 4 0 0
			load_aligned(abs.data_view()) // 4 0 1 2
	));

	// Expect TRUE.
	bool containsNanInfinite = contains_nan_or_infinite(load_aligned(eq.data_view()));

	// Expect 8, 1, 1, 1
	Vector4f exp2;
	store_aligned(exp2.data_view(),
		sakura::math::vector::exp2(
			load_aligned(min.data_view())
	));

	auto vec4_one = Vector4f::vector_one();
	auto vec3_one = Vector3f::vector_one();
	auto location0 = Vector3d::vector_zero();
	auto location1 = Vector3d::vector_one();
	
	sakura::Transform transform(
		Quaternion(0.f, 0.500f, 0.f, 0.866f),
		Vector3f::vector_zero(),
		Vector3f::vector_one()
	);

	//Matrix4x4 transform_matrix = __transform.to_matrix();
	
	Quaternion q(0.f, 0.500f, 0.f, 0.866f); // quat3 yaw 60
	Quaternion q1(0.500f, 0.f, 0.f, 0.866f); // quat2 pitch 60
	Quaternion q2(0.f, 0.f, 0.500f, 0.866f); // quat roll 60
	Quaternion q3(0.f, 0.866f, 0.f, 0.5f); // quat4 yaw 120
	//Rotator rrr= q.rotator();
	VectorRegister quat  =  // 0, 0, 0.5, 0.866
		DirectX::XMQuaternionRotationRollPitchYaw(0, 0, 3.14159f / 3.f);
	VectorRegister quat2 =  // 0.5, 0, 0, 0.866
		DirectX::XMQuaternionRotationRollPitchYaw(3.14159f / 3.f, 0, 0);
	VectorRegister quat3 =  // 0, 0.5, 0, 0.866
		DirectX::XMQuaternionRotationRollPitchYaw(0.f, 3.14159f / 3.f, 0.f);
	VectorRegister quat4 =  // 0, 0.866, 0, 0.5
		DirectX::XMQuaternionRotationRollPitchYaw(0.f, 3.14159f * 2.f / 3.f, 0.f);

	Rotator rot = q.rotator(); // yaw: 60
	Rotator rot1 = q1.rotator(); // pitch: 60
	Rotator rot2 = q2.rotator(); // roll: 60
	Rotator rot3 = q3.rotator(); // yaw: 120
	
	bool end = true;
	if(end)
	{
		return 0;
	}
}
