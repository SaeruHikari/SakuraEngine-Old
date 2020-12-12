#pragma once
#include "Quaternion.h"
#include "Math/VectorMath.h"

namespace sakura
{
    struct Translation 
    {
        Vector3f translation_ = Vector3f::vector_zero();
    };

    struct Rotation
    {
        Vector3f rotation_ = Vector3f::vector_zero();
    };

    struct Scale
    {
        Vector3f scale_ = Vector3f::vector_one();
    };
	
	// LH.
    struct alignas(16) Transform
    {
        using VectorStorage = sakura::math::vector::VectorRegister;
    public:
        FORCEINLINE Transform(
            const Quaternion& rotation, const Vector3f& translation, const Vector3f& scale = Vector3f::vector_one())
        {
            rotation_ = sakura::math::vector::load_aligned(rotation.data_view());
            translation_ = sakura::math::vector::load_float3_w0(translation.data_view());
            scale_ = sakura::math::vector::load_float3_w0(scale.data_view());
        }

        FORCEINLINE Transform(
            const VectorStorage& rotation, const VectorStorage& translation, const VectorStorage& scale)
	            :rotation_(rotation), translation_(translation), scale_(scale)
        {
            
        }
    	
        FORCEINLINE Transform& operator=(const Transform& Other)
        {
            this->rotation_ = Other.rotation_;
            this->translation_ = Other.translation_;
            this->scale_ = Other.scale_;
            return *this;
        }
    protected:
        VectorStorage rotation_ = sakura::math::vector::register_zero;
        VectorStorage translation_ = sakura::math::vector::vector_register(0.f, 0.f, 0.f, 1.f);
        VectorStorage scale_ = sakura::math::vector::register_one;
    };



}