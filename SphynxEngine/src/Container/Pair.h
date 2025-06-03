#pragma once

#include "Core/Core.h"


namespace Sphynx
{
	template <typename T1, typename T2>
	struct Pair
	{
	public:
		Pair() : First(), Second() {}
		Pair(const T1& v1, const T2& v2) : First(v1), Second(v2) {}
		Pair(T1&& v1, T2&& v2) : First(std::move(v1)), Second(std::move(v2)) {}
		Pair(const Pair& other) : First(other.First), Second(other.Second) {}
		~Pair() = default;

		Pair& operator = (const Pair& other)
		{
			First = other.First;
			Second = other.Second;
			return *this;
		}

		Pair& operator = (Pair&& other)
		{
			First = std::move(other.First);
			Second = std::move(other.Second);
			return *this;
		}

		template <size_t I>
		auto& get()&
		{
			if constexpr (I == 0) return First;
			else if constexpr (I == 1) return Second;
		}

		template <size_t I>
		auto const& get() const&
		{
			if constexpr (I == 0) return First;
			else if constexpr (I == 1) return Second;
		}

		template <size_t I>
		auto&& get()&&
		{
			if constexpr (I == 0) return std::move(First);
			else if constexpr (I == 1) return std::move(Second);
		}

	public:
		T1 First;
		T2 Second;
	};
}