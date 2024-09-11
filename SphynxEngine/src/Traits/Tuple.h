#pragma once

#include "Common.h"


namespace Sphynx
{
	namespace Traits
	{
		// Tuple traits
		template<typename T, size_t Size = std::tuple_size<T>::value, typename = void>
		struct is_tuple_elements_defined : std::false_type {};

		template<typename T, size_t Size>
		struct is_tuple_elements_defined<T, Size, std::void_t<
			decltype(std::tuple_element<Size - 1, T>::type)
			>> : std::conditional<
			true && is_tuple_elements_defined<T, Size - 1>::value,
			std::true_type,
			std::false_type
			>::type {};

		template<typename T>
		struct is_tuple_elements_defined<T, 0> : std::true_type {};

		template<typename T, typename = void>
		struct is_tuple_sizeable : std::false_type {};

		template<typename T>
		struct is_tuple_sizeable<T, std::void_t<
			decltype(std::tuple_size<T>::value)
			>> : std::true_type {};

		template<typename T>
		struct is_tuple_like : std::conditional<
			is_tuple_elements_defined<T>::value&& is_tuple_sizeable<T>::value,
			std::true_type,
			std::false_type
		>::type {};

		template<typename T>
		inline constexpr bool is_tuple_like_v = is_tuple_like<T>::value;

	}
}
