#pragma once

#include "Common.h"


namespace Sphynx
{
	namespace Traits
	{
		template<typename T, typename Tag>
		using unique_type = T;

		template<typename... Args>
		struct args_pack {};

		template<>
		struct args_pack<>
		{
			struct nulltype {};
		};

		template<typename ArgsPack>
		struct args_pack_as_tuple;

		template<typename... Args>
		struct args_pack_as_tuple<args_pack<Args...>> : std::type_identity<std::tuple<Args...>> {};

		template<typename ArgsPack>
		using args_pack_as_tuple_t = typename args_pack_as_tuple<ArgsPack>::type;

		template<typename ArgsPack>
		struct args_pack_size;

		template<typename... Args>
		struct args_pack_size<args_pack<Args...>> : std::integral_constant<size_t, sizeof...(Args)> {};

		template<typename ArgsPack>
		inline constexpr size_t args_pack_size_v = args_pack_size<ArgsPack>::value;

		template<typename ArgsPack>
		struct is_args_pack_empty : std::integral_constant<bool, args_pack_size<ArgsPack>::value == 0> {};

		template<typename ArgsPack>
		inline constexpr bool is_args_pack_empty_v = is_args_pack_empty<ArgsPack>::value;

		template<typename ArgsPack, size_t I>
		struct args_pack_element : std::type_identity<
			typename std::conditional<
			0 <= I && I < args_pack_size<ArgsPack>::value,
			typename std::tuple_element<I, args_pack_as_tuple_t<ArgsPack>>::type,
			typename args_pack<>::nulltype
			>::type
			> {};

		template<typename ArgsPack, size_t I>
		using args_pack_element_t = typename args_pack_element<ArgsPack, I>::type;

		template<typename ArgsPack, size_t I>
		struct args_pack_element_offset : std::conditional<
			((args_pack_element_offset<ArgsPack, I - 1>::value
				+ sizeof(args_pack_element_t<ArgsPack, I - 1>))
				% alignof(args_pack_element_t<ArgsPack, I>)) == 0,
			std::integral_constant<size_t, args_pack_element_offset<ArgsPack, I - 1>::value
			+ sizeof(args_pack_element_t<ArgsPack, I - 1>)>,
			std::integral_constant<size_t, (args_pack_element_offset<ArgsPack, I - 1>::value
				+ sizeof(args_pack_element_t<ArgsPack, I - 1>))
			+ alignof(args_pack_element_t<ArgsPack, I>)
			- ((args_pack_element_offset<ArgsPack, I - 1>::value
				+ sizeof(args_pack_element_t<ArgsPack, I - 1>))
				% alignof(args_pack_element_t<ArgsPack, I>))>
		>::type {};

		template<typename ArgsPack>
		struct args_pack_element_offset<ArgsPack, 0> : std::integral_constant<size_t, 0> {};

		template<typename ArgsPack, size_t I>
		inline constexpr size_t args_pack_element_offset_v = args_pack_element_offset<ArgsPack, I>::value;

		// Meta traits
		template<class T>
		struct is_templated_class : std::false_type {};

		template<template<typename...> class T, typename ...Args>
		struct is_templated_class<T<Args...>> : std::true_type {};
	}
}

