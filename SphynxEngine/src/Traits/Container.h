#pragma once

#include "Common.h"


namespace Sphynx
{
	namespace Traits
	{
		// Container traits
		template<typename T, typename = void>
		struct is_container_like : std::false_type {};

		template<typename T>
		struct is_container_like<T, std::void_t<
			typename T::value_type,
			typename T::size_type,
			typename T::iterator,
			typename T::const_iterator
			>> : std::true_type {};

		template<typename T>
		inline constexpr bool is_container_like_v = is_container_like<T>::value;

		template<typename T, typename = void>
		struct is_iterable : std::false_type {};

		template<typename T>
		struct is_iterable<T, std::void_t<
			decltype(
				std::begin(std::declval<T&>()),
				std::end(std::declval<T&>()),
				std::begin(std::declval<const T&>()),
				std::end(std::declval<const T&>())
				)
			>> : std::true_type{};

		template<typename T>
		inline constexpr bool is_iterable_v = is_iterable<T>::value;

		template<typename T>
		struct is_container : std::conditional<
			is_iterable<T>::value && is_container_like<T>::value,
			std::true_type,
			std::false_type
		>::type {};

		template<typename T>
		inline constexpr bool is_container_v = is_container<T>::value;

		template<typename T>
		struct container_value_type : std::type_identity<typename T::value_type> {};

		template<typename T>
		using container_value_type_t = typename container_value_type<T>::type;

		template<typename T>
		struct container_size_type : std::type_identity<typename T::size_type> {};

		template<typename T>
		using container_size_type_t = typename container_size_type<T>::type;

		template<typename T>
		struct container_iterator_type : std::type_identity<typename T::iterator> {};

		template<typename T>
		using container_iterator_type_t = typename container_iterator_type<T>::type;

		template<typename T>
		struct container_const_iterator_type : std::type_identity<typename T::const_iterator> {};

		template<typename T>
		using container_const_iterator_type_t = typename container_const_iterator_type<T>::type;

	}
}
