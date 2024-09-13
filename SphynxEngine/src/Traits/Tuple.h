#pragma once

#include "Common.h"
#include <utility>


namespace Sphynx
{
	namespace Traits
	{
		// Tuple traits
		template<typename T, size_t I, typename = void>
		struct is_tuple_element_defined : std::false_type {};

		template<typename T, size_t I>
		struct is_tuple_element_defined<T, I, std::void_t<
			decltype(std::tuple_element<I, T>::type)
			>> : std::true_type {};

		namespace details
		{
			template<typename T, size_t I>
			struct is_tuple_elements_defined : std::conditional<
				is_tuple_element_defined<T, I>::value&& is_tuple_elements_defined<T, I - 1>::value,
				std::true_type,
				std::false_type
			>::type {};

			template<typename T>
			struct is_tuple_elements_defined<T, 0> : is_tuple_element_defined<T, 0> {};
		}

		template<typename T, typename = void>
		struct is_tuple_elements_defined : std::false_type {};

		template<typename T>
		struct is_tuple_elements_defined<T, std::void_t<
			decltype(std::tuple_size<T>::value)
			>> : details::is_tuple_elements_defined<T, std::tuple_size<T>::value>
			::type {};

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