#pragma once

#include <type_traits>
#include <utility>


#ifdef _MSVC_LANG  // MS compiler has different __cplusplus value for whatever reason
#define _CPP_VERSION _MSVC_LANG
#else
#define _CPP_VERSION __cplusplus
#endif

#if _CPP_VERSION <= 201703L
namespace std
{
	template<typename T>
	struct type_identity
	{
		using type = T;
	};

	template<class...>
	using void_t = void;
}
#endif

#if _CPP_VERSION < 201402L
namespace std
{
	template<typename T, T... Ints>
	struct integer_sequence {};

	template<size_t... Ints>
	using index_sequence = integer_sequence<size_t, Ints...>;

	namespace details
	{
		template<typename T, size_t Int, T... Ints>
		struct make_integer_sequence : make_integer_sequence<T, Int - 1, Int - 1, Ints...> {};

		template<typename T, T... Ints>
		struct make_integer_sequence<T, 0, Ints...>
		{
			using type = integer_sequence<T, Ints...>;
		};
	}

	template<typename T, T Int>
	using make_integer_sequence = typename details::make_integer_sequence<T, Int>::type;

	template<size_t Int>
	using make_index_sequence = typename details::make_integer_sequence<size_t, Int>::type;
}
#endif
