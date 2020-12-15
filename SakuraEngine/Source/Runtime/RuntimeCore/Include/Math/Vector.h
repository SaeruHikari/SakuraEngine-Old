#pragma once
#include "SakuraSTL.hpp"

namespace sakura
{
    template<typename T, size_t N>
    struct Vector
    {
        constexpr Vector() = default;
        constexpr Vector(const Vector& rhs) = default;
        Vector& operator=(const Vector& rhs) = default;
    	
        constexpr Vector(const std::array<T, N> v)
            :m_(v)
        {

        }
        sakura::span<T, N> data_view()
        {
            return m_;
        }
        sakura::span<const T, N> data_view() const
        {
            return m_;
        }
        static constexpr Vector<T, N> vector_one();
        static constexpr Vector<T, N> vector_zero();
    protected:
        std::array<T, N> m_ = sakura::create_array<float, N>(0.f);
    };
    template <typename T, size_t N>
    constexpr Vector<T, N> Vector<T, N>::vector_one()
    {
        return Vector<T, N>(sakura::create_array<T, N>( 1 ));
    }
    template <typename T, size_t N>
    constexpr Vector<T, N> Vector<T, N>::vector_zero()
    {
        return Vector<T, N>(sakura::create_array<T, N>( 0 ));
    }
    using Vector3f = Vector<float, 3>;

	
    template<>
    struct alignas(16) Vector<float, 4>
    {
    public:
        constexpr Vector() = default;
        constexpr Vector(float x, float y, float z, float w)
            :m_({ x, y, z, w })
        {

        }
        constexpr Vector(const std::array<float, 4> v)
			:m_(v)
        {
	        
        }
        sakura::span<float, 4> data_view()
        {
            return m_;
        }
        sakura::span<const float, 4> data_view() const
        {
            return m_;
        }
        static constexpr Vector<float, 4> vector_one();
        static constexpr Vector<float, 4> vector_zero();
    protected:
    	union
    	{
            struct alignas(16)
            {
                float X, Y, Z, W;
            };
            alignas(16) std::array<float, 4> m_ = { 0.f, 0.f, 0.f, 0.f };
    	};
    };
    using Vector4f = Vector<float, 4>;
    constexpr Vector4f Vector4f::vector_one()
    {
        return Vector4f(1.f, 1.f, 1.f, 1.f);
    }
    constexpr Vector4f Vector4f::vector_zero()
    {
        return Vector4f(0.f, 0.f, 0.f, 0.f);
    }

	
    using Vector3d = Vector<double, 3>;
    using Vector4d = Vector<double, 4>;
}
