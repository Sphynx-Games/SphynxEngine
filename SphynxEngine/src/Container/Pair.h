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

		friend bool operator==(const Pair& p1, const Pair& p2)
		{
			return p1.First == p2.First && p1.Second == p2.Second;
		}

	public:
		T1 First;
		T2 Second;
	};
}

template<typename T1, typename T2>
struct std::hash<Sphynx::Pair<T1, T2>>
{
	std::size_t operator()(const Sphynx::Pair<T1, T2>& pair) const
	{
		uint64_t l = std::hash<T1>{}(pair.First);
		uint64_t h = std::hash<T2>{}(pair.Second);

		if constexpr (sizeof(size_t) > 4)
		{
			return size_t(l ^ h);
		}
		else
		{
			uint64_t hash64 = l ^ h;
			return size_t(uint32_t(hash64 >> 32) ^ uint32_t(hash64));
		}
	}
};
