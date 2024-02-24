#pragma once

#include <cstdint>
#include <type_traits>

namespace Sphynx
{
	template <typename T = float>
	struct Vector2
	{
		static_assert(std::is_arithmetic_v<T>, "T must be a numeric type");

		T X;
		T Y;
	};

	template <typename T = float>
	struct Vector3
	{
		static_assert(std::is_arithmetic_v<T>, "T must be a numeric type");

		T X;
		T Y;
		T Z;
	};

	template <typename T = float>
	struct Vector4
	{
		static_assert(std::is_arithmetic_v<T>, "T must be a numeric type");

		T X;
		T Y;
		T Z;
		T W;
	};

	typedef Vector2<int32_t> Vector2i;
	typedef Vector2<uint32_t> Vector2u;
	typedef Vector2<double> Vector2d;

	typedef Vector3<int32_t> Vector3i;
	typedef Vector3<uint32_t> Vector3u;
	typedef Vector3<double> Vector3d;

	typedef Vector4<int32_t> Vector4i;
	typedef Vector4<uint32_t> Vector4u;
	typedef Vector4<double> Vector4d;
}
