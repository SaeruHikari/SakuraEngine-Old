namespace sakura
{
	template<typename T>
	FORCEINLINE Vector<T, 3> Vector<T, 3>::operator^(const Vector V) const
	{
		return Vector
		(
			Y * V.Z - Z * V.Y,
			Z * V.X - X * V.Z,
			X * V.Y - Y * V.X
		);
	}

	template<typename T>
	FORCEINLINE Vector<T, 3> Vector<T, 3>::cross_product(const Vector A, const Vector B)
	{
		return A ^ B;
	}

	template<typename T>
	FORCEINLINE T Vector<T, 3>::operator|(const Vector V) const
	{
		return X * V.X + Y * V.Y + Z * V.Z;
	}

	template<typename T>
	FORCEINLINE T Vector<T, 3>::dot_product(const Vector A, const Vector B)
	{
		return A | B;
	}

	template<typename T>
	FORCEINLINE Vector<T, 3> Vector<T, 3>::operator+(const Vector V) const
	{
		return Vector(X + V.X, Y + V.Y, Z + V.Z);
	}

	template<typename T>
	FORCEINLINE Vector<T, 3> Vector<T, 3>::operator-(const Vector V) const
	{
		return Vector(X - V.X, Y - V.Y, Z - V.Z);
	}

	template<typename T>
	FORCEINLINE Vector<T, 3> Vector<T, 3>::operator-(T Bias) const
	{
		return Vector(X - Bias, Y - Bias, Z - Bias);
	}

	template<typename T>
	FORCEINLINE Vector<T, 3> Vector<T, 3>::operator+(T Bias) const
	{
		return Vector(X + Bias, Y + Bias, Z + Bias);
	}

	template<typename T>
	FORCEINLINE Vector<T, 3> Vector<T, 3>::operator*(T Scale) const
	{
		return Vector(X * Scale, Y * Scale, Z * Scale);
	}

	template<typename T>
	FORCEINLINE Vector<T, 3> Vector<T, 3>::operator/(T Scale) const
	{
		const T RScale = 1.f / Scale;
		return Vector(X * RScale, Y * RScale, Z * RScale);
	}

	template<typename T>
	FORCEINLINE Vector<T, 3> Vector<T, 3>::operator*(const Vector V) const
	{
		return Vector(X * V.X, Y * V.Y, Z * V.Z);
	}

	template<typename T>
	FORCEINLINE Vector<T, 3> Vector<T, 3>::operator/(const Vector V) const
	{
		return Vector(X / V.X, Y / V.Y, Z / V.Z);
	}

	template<typename T>
	FORCEINLINE bool Vector<T, 3>::operator==(const Vector V) const
	{
		return X == V.X && Y == V.Y && Z == V.Z;
	}

	template<typename T>
	FORCEINLINE bool Vector<T, 3>::operator!=(const Vector V) const
	{
		return X != V.X || Y != V.Y || Z != V.Z;
	}

	template<typename T>
	FORCEINLINE bool Vector<T, 3>::equals(const Vector V, T Tolerance) const
	{
		return std::abs(X - V.X) <= Tolerance && std::abs(Y - V.Y) <= Tolerance && std::abs(Z - V.Z) <= Tolerance;
	}

}
