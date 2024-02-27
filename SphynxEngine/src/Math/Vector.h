#pragma once

#include <cstdint>
#include <type_traits>

namespace Sphynx
{
	template <typename T>
	struct Vector2T
	{
		static_assert(std::is_arithmetic_v<T>, "T must be a numeric type");

		T X;
		T Y;
	};

	template <typename T>
	struct Vector3T
	{
		static_assert(std::is_arithmetic_v<T>, "T must be a numeric type");

		T X;
		T Y;
		T Z;
	};

	template <typename T>
	struct Vector4T
	{
		static_assert(std::is_arithmetic_v<T>, "T must be a numeric type");

		T X;
		T Y;
		T Z;
		T W;
	};

	typedef Vector2T<int32_t> Vector2i;
	typedef Vector2T<uint32_t> Vector2u;
	typedef Vector2T<float> Vector2f;
	typedef Vector2T<double> Vector2d;

	typedef Vector3T<int32_t> Vector3i;
	typedef Vector3T<uint32_t> Vector3u;
	typedef Vector3T<float> Vector3f;
	typedef Vector3T<double> Vector3d;

	typedef Vector4T<int32_t> Vector4i;
	typedef Vector4T<uint32_t> Vector4u;
	typedef Vector4T<float> Vector4f;
	typedef Vector4T<double> Vector4d;
}