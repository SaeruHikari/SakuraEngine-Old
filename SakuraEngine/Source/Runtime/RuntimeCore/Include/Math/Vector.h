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


    // Vector3
	template<typename T>
    struct Vector<T, 3>
    {
	public:
		FORCEINLINE constexpr Vector() = default;
		FORCEINLINE constexpr Vector(const T x, const T y, const T z)
			:m_({ x, y, z})
		{

		}
		FORCEINLINE constexpr Vector(const std::array<T, 3> v)
			: m_(v)
		{

		}
		FORCEINLINE sakura::span<T, 3> data_view()
		{
			return m_;
		}
		FORCEINLINE sakura::span<const T, 3> data_view() const
		{
			return m_;
		}
		FORCEINLINE static constexpr Vector<T, 3> vector_one();
		FORCEINLINE static constexpr Vector<T, 3> vector_zero();

        /**
		 * Calculate cross product between this and another vector.
		 *
		 * @param V The other vector.
		 * @return The cross product.
		 */
        FORCEINLINE Vector operator^(const Vector V) const;
        /**
         * Calculate the cross product of two vectors.
         *
         * @param A The first vector.
         * @param B The second vector.
         * @return The cross product.
         */
        FORCEINLINE static Vector cross_product(const Vector A, const Vector B);

		/**
		 * Calculate the dot product between this and another vector.
		 *
		 * @param V The other vector.
		 * @return The dot product.
		 */
		FORCEINLINE T operator|(const Vector V) const;

		/**
		 * Calculate the dot product of two vectors.
		 *
		 * @param A The first vector.
		 * @param B The second vector.
		 * @return The dot product.
		 */
		FORCEINLINE static T dot_product(const Vector A, const Vector B);

		/**
		 * Gets the result of component-wise addition of this and another vector.
		 *
		 * @param V The vector to add to this.
		 * @return The result of vector addition.
		 */
		FORCEINLINE Vector operator+(const Vector V) const;

		/**
		 * Gets the result of component-wise subtraction of this by another vector.
		 *
		 * @param V The vector to subtract from this.
		 * @return The result of vector subtraction.
		 */
		FORCEINLINE Vector operator-(const Vector V) const;

		/**
		 * Gets the result of subtracting from each component of the vector.
		 *
		 * @param Bias How much to subtract from each component.
		 * @return The result of subtraction.
		 */
		FORCEINLINE Vector operator-(T Bias) const;

		/**
		 * Gets the result of adding to each component of the vector.
		 *
		 * @param Bias How much to add to each component.
		 * @return The result of addition.
		 */
		FORCEINLINE Vector operator+(T Bias) const;

		/**
		 * Gets the result of scaling the vector (multiplying each component by a value).
		 *
		 * @param Scale What to multiply each component by.
		 * @return The result of multiplication.
		 */
		FORCEINLINE Vector operator*(T Scale) const;

		/**
		 * Gets the result of dividing each component of the vector by a value.
		 *
		 * @param Scale What to divide each component by.
		 * @return The result of division.
		 */
		Vector operator/(T Scale) const;

		/**
		 * Gets the result of component-wise multiplication of this vector by another.
		 *
		 * @param V The vector to multiply with.
		 * @return The result of multiplication.
		 */
		FORCEINLINE Vector operator*(const Vector V) const;

		/**
		 * Gets the result of component-wise division of this vector by another.
		 *
		 * @param V The vector to divide by.
		 * @return The result of division.
		 */
		FORCEINLINE Vector operator/(const Vector V) const;

		// Binary comparison operators.

		/**
		 * Check against another vector for equality.
		 *
		 * @param V The vector to check against.
		 * @return true if the vectors are equal, false otherwise.
		 */
		bool operator==(const Vector V) const;

		/**
		 * Check against another vector for inequality.
		 *
		 * @param V The vector to check against.
		 * @return true if the vectors are not equal, false otherwise.
		 */
		bool operator!=(const Vector V) const;

		/**
		 * Check against another vector for equality, within specified error limits.
		 *
		 * @param V The vector to check against.
		 * @param Tolerance Error tolerance.
		 * @return true if the vectors are equal within tolerance limits, false otherwise.
		 */
		bool equals(const Vector V, T Tolerance = KINDA_SMALL_NUMBER) const;

		/**
		 * Get a negated copy of the vector.
		 *
		 * @return A negated copy of the vector.
		 */
		FORCEINLINE Vector operator-() const;

		/**
		 * Adds another vector to this.
		 * Uses component-wise addition.
		 *
		 * @param V Vector to add to this.
		 * @return Copy of the vector after addition.
		 */
		FORCEINLINE Vector operator+=(const Vector V);

		/**
		 * Subtracts another vector from this.
		 * Uses component-wise subtraction.
		 *
		 * @param V Vector to subtract from this.
		 * @return Copy of the vector after subtraction.
		 */
		FORCEINLINE Vector operator-=(const Vector V);

		/**
		 * Scales the vector.
		 *
		 * @param Scale Amount to scale this vector by.
		 * @return Copy of the vector after scaling.
		 */
		FORCEINLINE Vector operator*=(T Scale);

		/**
		 * Divides the vector by a number.
		 *
		 * @param V What to divide this vector by.
		 * @return Copy of the vector after division.
		 */
		Vector operator/=(T V);

		/**
		 * Multiplies the vector with another vector, using component-wise multiplication.
		 *
		 * @param V What to multiply this vector with.
		 * @return Copy of the vector after multiplication.
		 */
		Vector operator*=(const Vector V);

		/**
		 * Divides the vector by another vector, using component-wise division.
		 *
		 * @param V What to divide vector by.
		 * @return Copy of the vector after division.
		 */
		Vector operator/=(const Vector V);
    protected:
		union
		{
			struct 
			{
                T X, Y, Z;
			};
			std::array<T, 3> m_ = { 0, 0, 0};
		};
    };
	template<typename T>
	FORCEINLINE constexpr Vector<T, 3> Vector<T, 3>::vector_zero()
	{
		return Vector<T, 3>(0, 0, 0);
	}
	template<typename T>
	FORCEINLINE constexpr Vector<T, 3> Vector<T, 3>::vector_one()
	{
		return Vector<T, 3>(1, 1, 1);
	}
    using Vector3f = Vector<float, 3>;


	
    // Vector4
    template<>
    struct alignas(16) Vector<float, 4>
    {
    public:
        FORCEINLINE constexpr Vector() = default;
        FORCEINLINE constexpr Vector(float x, float y, float z, float w)
            :m_({ x, y, z, w })
        {

        }
        FORCEINLINE constexpr Vector(const std::array<float, 4> v)
			:m_(v)
        {
	        
        }
        FORCEINLINE sakura::span<float, 4> data_view()
        {
            return m_;
        }
        FORCEINLINE sakura::span<const float, 4> data_view() const
        {
            return m_;
        }
        FORCEINLINE static constexpr Vector<float, 4> vector_one();
        FORCEINLINE static constexpr Vector<float, 4> vector_zero();
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
	FORCEINLINE constexpr Vector4f Vector4f::vector_one()
    {
        return Vector4f(1.f, 1.f, 1.f, 1.f);
    }
	FORCEINLINE constexpr Vector4f Vector4f::vector_zero()
    {
        return Vector4f(0.f, 0.f, 0.f, 0.f);
    }

	
    using Vector3d = Vector<double, 3>;
    using Vector4d = Vector<double, 4>;
}


#include "Detail/Vector3Impl.inl"
#include "Detail/Vector4fImpl.inl"