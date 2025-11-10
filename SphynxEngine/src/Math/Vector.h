#pragma once

#include "Core/Core.h"

#include <cstdint>
#include <type_traits>

namespace Sphynx
{
	template <typename T>
	struct /*SPHYNX_API*/ Vector2T
	{
		static_assert(std::is_arithmetic_v<T>, "T must be a numeric type");

		T X;
		T Y;

		Vector2T() = default;
		Vector2T(T value) : X(value), Y(value) {}
		Vector2T(T x, T y) : X(x), Y(y) {}

		friend bool operator==(const Vector2T& lhs, const Vector2T& rhs)
		{
			return lhs.X == rhs.X && lhs.Y == rhs.Y;
		}

		friend bool operator!=(const Vector2T& lhs, const Vector2T& rhs)
		{
			return !(lhs == rhs);
		}

		Vector2T& operator+=(const Vector2T& other)
		{
			X += other.X;
			Y += other.Y;
			return *this;
		}

		Vector2T operator+(const Vector2T& other) const
		{
			return { X + other.X, Y + other.Y };
		}

		Vector2T operator*(const Vector2T& other) const
		{
			return { X * other.X, Y * other.Y };
		}

		Vector2T& operator/=(const Vector2T& other)
		{
			X /= other.X;
			Y /= other.Y;
			return *this;
		}

		Vector2T operator/(const Vector2T& other) const
		{
			return { X / other.X, Y / other.Y };
		}

		static const Vector2T Zero;
		static const Vector2T Identity;
		static const Vector2T Right;
		static const Vector2T Left;
		static const Vector2T Up;
		static const Vector2T Down;
	};

	template<typename T> inline const Vector2T<T> Vector2T<T>::Zero		{ static_cast<T>( 0), static_cast<T>( 0) };
	template<typename T> inline const Vector2T<T> Vector2T<T>::Identity	{ static_cast<T>( 1), static_cast<T>( 1) };
	template<typename T> inline const Vector2T<T> Vector2T<T>::Right	{ static_cast<T>( 1), static_cast<T>( 0) };
	template<typename T> inline const Vector2T<T> Vector2T<T>::Left		{ static_cast<T>(-1), static_cast<T>( 0) };
	template<typename T> inline const Vector2T<T> Vector2T<T>::Up		{ static_cast<T>( 0), static_cast<T>( 1) };
	template<typename T> inline const Vector2T<T> Vector2T<T>::Down		{ static_cast<T>( 0), static_cast<T>(-1) };

	template <typename T>
	struct /*SPHYNX_API*/ Vector3T
	{
		static_assert(std::is_arithmetic_v<T>, "T must be a numeric type");

		T X;
		T Y;
		T Z;

		Vector3T() = default;
		Vector3T(T value) : X(value), Y(value), Z(value) {}
		Vector3T(T x, T y, T z) : X(x), Y(y), Z(z) {}

		friend bool operator==(const Vector3T& lhs, const Vector3T& rhs)
		{
			return lhs.X == rhs.X && lhs.Y == rhs.Y && lhs.Z == rhs.Z;
		}

		friend bool operator!=(const Vector3T& lhs, const Vector3T& rhs)
		{
			return !(lhs == rhs);
		}

		Vector3T& operator+=(const Vector3T& other)
		{
			X += other.X;
			Y += other.Y;
			Z += other.Z;
			return *this;
		}

		Vector3T operator+(const Vector3T& other) const
		{
			return { X + other.X, Y + other.Y, Z + other.Z };
		}

		Vector3T operator*(const Vector3T& other) const
		{
			return { X * other.X, Y * other.Y, Z * other.Z };
		}

		Vector3T& operator/=(const Vector3T& other)
		{
			X /= other.X;
			Y /= other.Y;
			Z /= other.Z;
			return *this;
		}

		Vector3T operator/(const Vector3T& other) const
		{
			return { X / other.X, Y / other.Y, Z / other.Z };
		}

		static const Vector3T Zero;
		static const Vector3T Identity;
		static const Vector3T Right;
		static const Vector3T Left;
		static const Vector3T Up;
		static const Vector3T Down;
		static const Vector3T Forward;
		static const Vector3T Backward;
	};

	template<typename T> inline const Vector3T<T> Vector3T<T>::Zero		{ static_cast<T>( 0), static_cast<T>( 0), static_cast<T>( 0) };
	template<typename T> inline const Vector3T<T> Vector3T<T>::Identity	{ static_cast<T>( 1), static_cast<T>( 1), static_cast<T>( 1) };
	template<typename T> inline const Vector3T<T> Vector3T<T>::Right	{ static_cast<T>( 1), static_cast<T>( 0), static_cast<T>( 0) };
	template<typename T> inline const Vector3T<T> Vector3T<T>::Left		{ static_cast<T>(-1), static_cast<T>( 0), static_cast<T>( 0) };
	template<typename T> inline const Vector3T<T> Vector3T<T>::Up		{ static_cast<T>( 0), static_cast<T>( 1), static_cast<T>( 0) };
	template<typename T> inline const Vector3T<T> Vector3T<T>::Down		{ static_cast<T>( 0), static_cast<T>(-1), static_cast<T>( 0) };
	template<typename T> inline const Vector3T<T> Vector3T<T>::Forward	{ static_cast<T>( 0), static_cast<T>( 0), static_cast<T>( 1) };
	template<typename T> inline const Vector3T<T> Vector3T<T>::Backward	{ static_cast<T>( 0), static_cast<T>( 0), static_cast<T>(-1) };

	template <typename T>
	struct /*SPHYNX_API*/ Vector4T
	{
		static_assert(std::is_arithmetic_v<T>, "T must be a numeric type");

		T X;
		T Y;
		T Z;
		T W;

		Vector4T() = default;
		Vector4T(T value) : X(value), Y(value), Z(value), W(value) {}
		Vector4T(T x, T y, T z, T w) : X(x), Y(y), Z(z), W(w) {}

		friend bool operator==(const Vector4T& lhs, const Vector4T& rhs)
		{
			return lhs.X == rhs.X && lhs.Y == rhs.Y && lhs.Z == rhs.Z && lhs.W == rhs.W;
		}

		friend bool operator!=(const Vector4T& lhs, const Vector4T& rhs)
		{
			return !(lhs == rhs);
		}

		Vector4T& operator+=(const Vector4T& other)
		{
			X += other.X;
			Y += other.Y;
			Z += other.Z;
			W += other.W;
			return *this; 
		}

		Vector4T operator+(const Vector4T& other) const
		{
			return { X + other.X, Y + other.Y, Z + other.Z, W + other.W };
		}

		Vector4T operator*(const Vector4T& other) const
		{
			return { X * other.X, Y * other.Y, Z * other.Z, W * other.W };
		}

		Vector4T& operator/=(const Vector4T& other)
		{
			X /= other.X;
			Y /= other.Y;
			Z /= other.Z;
			W /= other.W;
			return *this;
		}

		Vector4T operator/(const Vector4T& other) const
		{
			return { X / other.X, Y / other.Y, Z / other.Z, W / other.W };
		}
	};

	template struct SPHYNX_API Vector2T<int32_t>;
	typedef SPHYNX_API Vector2T<int32_t> Vector2i;
	template struct SPHYNX_API Vector2T<uint32_t>;
	typedef SPHYNX_API Vector2T<uint32_t> Vector2u;
	template struct SPHYNX_API Vector2T<float>;
	typedef SPHYNX_API Vector2T<float> Vector2f;
	template struct SPHYNX_API Vector2T<double>;
	typedef SPHYNX_API Vector2T<double> Vector2d;

	template struct SPHYNX_API Vector3T<int32_t>;
	typedef SPHYNX_API Vector3T<int32_t> Vector3i;
	template struct SPHYNX_API Vector3T<uint32_t>;
	typedef SPHYNX_API Vector3T<uint32_t> Vector3u;
	template struct SPHYNX_API Vector3T<float>;
	typedef SPHYNX_API Vector3T<float> Vector3f;
	template struct SPHYNX_API Vector3T<double>;
	typedef SPHYNX_API Vector3T<double> Vector3d;

	template struct SPHYNX_API Vector4T<int32_t>;
	typedef SPHYNX_API Vector4T<int32_t> Vector4i;
	template struct SPHYNX_API Vector4T<uint32_t>;
	typedef SPHYNX_API Vector4T<uint32_t> Vector4u;
	template struct SPHYNX_API Vector4T<float>;
	typedef SPHYNX_API Vector4T<float> Vector4f;
	template struct SPHYNX_API Vector4T<double>;
	typedef SPHYNX_API Vector4T<double> Vector4d;
}

#include "Reflection/Reflection.h"

SPX_REFLECT_STRUCT_BEGIN(Sphynx::Vector2i, SPHYNX_API)

SPX_REFLECT_PROPERTY(X)
SPX_REFLECT_PROPERTY(Y)

SPX_REFLECT_STRUCT_END(Sphynx::Vector2i, SPHYNX_API)


SPX_REFLECT_STRUCT_BEGIN(Sphynx::Vector2u, SPHYNX_API)

SPX_REFLECT_PROPERTY(X)
SPX_REFLECT_PROPERTY(Y)

SPX_REFLECT_STRUCT_END(Sphynx::Vector2u, SPHYNX_API)


SPX_REFLECT_STRUCT_BEGIN(Sphynx::Vector2f, SPHYNX_API)

SPX_REFLECT_PROPERTY(X)
SPX_REFLECT_PROPERTY(Y)

SPX_REFLECT_STRUCT_END(Sphynx::Vector2f, SPHYNX_API)


SPX_REFLECT_STRUCT_BEGIN(Sphynx::Vector2d, SPHYNX_API)

SPX_REFLECT_PROPERTY(X)
SPX_REFLECT_PROPERTY(Y)

SPX_REFLECT_STRUCT_END(Sphynx::Vector2d, SPHYNX_API)


SPX_REFLECT_STRUCT_BEGIN(Sphynx::Vector3i, SPHYNX_API)

SPX_REFLECT_PROPERTY(X)
SPX_REFLECT_PROPERTY(Y)
SPX_REFLECT_PROPERTY(Z)

SPX_REFLECT_STRUCT_END(Sphynx::Vector3i, SPHYNX_API)


SPX_REFLECT_STRUCT_BEGIN(Sphynx::Vector3u, SPHYNX_API)

SPX_REFLECT_PROPERTY(X)
SPX_REFLECT_PROPERTY(Y)
SPX_REFLECT_PROPERTY(Z)


SPX_REFLECT_STRUCT_END(Sphynx::Vector3u, SPHYNX_API)


SPX_REFLECT_STRUCT_BEGIN(Sphynx::Vector3f, SPHYNX_API)

SPX_REFLECT_PROPERTY(X)
SPX_REFLECT_PROPERTY(Y)
SPX_REFLECT_PROPERTY(Z)

SPX_REFLECT_STRUCT_END(Sphynx::Vector3f, SPHYNX_API)


SPX_REFLECT_STRUCT_BEGIN(Sphynx::Vector3d, SPHYNX_API)

SPX_REFLECT_PROPERTY(X)
SPX_REFLECT_PROPERTY(Y)
SPX_REFLECT_PROPERTY(Z)

SPX_REFLECT_STRUCT_END(Sphynx::Vector3d, SPHYNX_API)


SPX_REFLECT_STRUCT_BEGIN(Sphynx::Vector4i, SPHYNX_API)

SPX_REFLECT_PROPERTY(X)
SPX_REFLECT_PROPERTY(Y)
SPX_REFLECT_PROPERTY(Z)
SPX_REFLECT_PROPERTY(W)

SPX_REFLECT_STRUCT_END(Sphynx::Vector4i, SPHYNX_API)


SPX_REFLECT_STRUCT_BEGIN(Sphynx::Vector4u, SPHYNX_API)

SPX_REFLECT_PROPERTY(X)
SPX_REFLECT_PROPERTY(Y)
SPX_REFLECT_PROPERTY(Z)
SPX_REFLECT_PROPERTY(W)

SPX_REFLECT_STRUCT_END(Sphynx::Vector4u, SPHYNX_API)


SPX_REFLECT_STRUCT_BEGIN(Sphynx::Vector4f, SPHYNX_API)

SPX_REFLECT_PROPERTY(X)
SPX_REFLECT_PROPERTY(Y)
SPX_REFLECT_PROPERTY(Z)
SPX_REFLECT_PROPERTY(W)

SPX_REFLECT_STRUCT_END(Sphynx::Vector4f, SPHYNX_API)


SPX_REFLECT_STRUCT_BEGIN(Sphynx::Vector4d, SPHYNX_API)

SPX_REFLECT_PROPERTY(X)
SPX_REFLECT_PROPERTY(Y)
SPX_REFLECT_PROPERTY(Z)
SPX_REFLECT_PROPERTY(W)

SPX_REFLECT_STRUCT_END(Sphynx::Vector4d, SPHYNX_API)
