#pragma once

#include "Common.h"
#include "Utility.h"


namespace Sphynx
{
	namespace Traits
	{
		// Function traits
		template<typename Function>
		struct is_function_ptr : std::conditional<
			std::is_pointer<Function>::value&&
			std::is_function<typename std::remove_pointer<Function>::type>::value,
			std::true_type,
			std::false_type
		>::type {};

		template<typename T, typename TReturn, typename... Args>
		struct is_function_ptr<TReturn(T::*)(Args...)> : std::true_type {};

		template<typename T>
		inline constexpr bool is_function_ptr_v = is_function_ptr<T>::value;

		template<typename T>
		struct function_return_type;

		template<typename TReturn, typename... Args>
		struct function_return_type<TReturn(*)(Args...)> : std::type_identity<TReturn> {};

		template<typename T, typename TReturn, typename... Args>
		struct function_return_type<TReturn(T::*)(Args...)> : std::type_identity<TReturn> {};

		template<typename T, typename TReturn, typename... Args>
		struct function_return_type<TReturn(T::*)(Args...) const> : std::type_identity<TReturn> {};

		template<typename T, typename TReturn, typename... Args>
		struct function_return_type<TReturn(T::*)(Args...) volatile> : std::type_identity<TReturn> {};

		template<typename T, typename TReturn, typename... Args>
		struct function_return_type<TReturn(T::*)(Args...) const volatile> : std::type_identity<TReturn> {};

		template<typename T, typename TReturn, typename... Args>
		struct function_return_type<TReturn(T::*)(Args...)&> : std::type_identity<TReturn> {};

		template<typename T, typename TReturn, typename... Args>
		struct function_return_type<TReturn(T::*)(Args...) const&> : std::type_identity<TReturn> {};

		template<typename T, typename TReturn, typename... Args>
		struct function_return_type<TReturn(T::*)(Args...) volatile&> : std::type_identity<TReturn> {};

		template<typename T, typename TReturn, typename... Args>
		struct function_return_type<TReturn(T::*)(Args...) const volatile&> : std::type_identity<TReturn> {};

		template<typename T, typename TReturn, typename... Args>
		struct function_return_type<TReturn(T::*)(Args...)&&> : std::type_identity<TReturn> {};

		template<typename T, typename TReturn, typename... Args>
		struct function_return_type<TReturn(T::*)(Args...) const&&> : std::type_identity<TReturn> {};

		template<typename T, typename TReturn, typename... Args>
		struct function_return_type<TReturn(T::*)(Args...) volatile&&> : std::type_identity<TReturn> {};

		template<typename T, typename TReturn, typename... Args>
		struct function_return_type<TReturn(T::*)(Args...) const volatile&&> : std::type_identity<TReturn> {};

		template<typename T>
		using function_return_type_t = typename function_return_type<T>::type;

		template<typename T>
		struct function_args_type;

		template<typename TReturn, typename... Args>
		struct function_args_type<TReturn(*)(Args...)> : std::type_identity<args_pack<Args...>> {};

		template<typename T, typename TReturn, typename... Args>
		struct function_args_type<TReturn(T::*)(Args...)> : std::type_identity<args_pack<Args...>> {};

		template<typename T, typename TReturn, typename... Args>
		struct function_args_type<TReturn(T::*)(Args...) const> : std::type_identity<args_pack<Args...>> {};

		template<typename T, typename TReturn, typename... Args>
		struct function_args_type<TReturn(T::*)(Args...) volatile> : std::type_identity<args_pack<Args...>> {};

		template<typename T, typename TReturn, typename... Args>
		struct function_args_type<TReturn(T::*)(Args...) const volatile> : std::type_identity<args_pack<Args...>> {};

		template<typename T, typename TReturn, typename... Args>
		struct function_args_type<TReturn(T::*)(Args...)&> : std::type_identity<args_pack<Args...>> {};

		template<typename T, typename TReturn, typename... Args>
		struct function_args_type<TReturn(T::*)(Args...) const&> : std::type_identity<args_pack<Args...>> {};

		template<typename T, typename TReturn, typename... Args>
		struct function_args_type<TReturn(T::*)(Args...) volatile&> : std::type_identity<args_pack<Args...>> {};

		template<typename T, typename TReturn, typename... Args>
		struct function_args_type<TReturn(T::*)(Args...) const volatile&> : std::type_identity<args_pack<Args...>> {};

		template<typename T, typename TReturn, typename... Args>
		struct function_args_type<TReturn(T::*)(Args...)&&> : std::type_identity<args_pack<Args...>> {};

		template<typename T, typename TReturn, typename... Args>
		struct function_args_type<TReturn(T::*)(Args...) const&&> : std::type_identity<args_pack<Args...>> {};

		template<typename T, typename TReturn, typename... Args>
		struct function_args_type<TReturn(T::*)(Args...) volatile&&> : std::type_identity<args_pack<Args...>> {};

		template<typename T, typename TReturn, typename... Args>
		struct function_args_type<TReturn(T::*)(Args...) const volatile&&> : std::type_identity<args_pack<Args...>> {};

		template<typename T>
		using function_args_type_t = typename function_args_type<T>::type;

		template<typename T>
		struct function_scope_type;

		template<typename T, typename TReturn, typename... Args>
		struct function_scope_type<TReturn(T::*)(Args...)> : std::type_identity<T> {};

		template<typename T, typename TReturn, typename... Args>
		struct function_scope_type<TReturn(T::*)(Args...) const> : std::type_identity<const T> {};

		template<typename T, typename TReturn, typename... Args>
		struct function_scope_type<TReturn(T::*)(Args...) volatile> : std::type_identity<volatile T> {};

		template<typename T, typename TReturn, typename... Args>
		struct function_scope_type<TReturn(T::*)(Args...) const volatile> : std::type_identity<const volatile T> {};

		template<typename T, typename TReturn, typename... Args>
		struct function_scope_type<TReturn(T::*)(Args...)&> : std::type_identity<T&> {};

		template<typename T, typename TReturn, typename... Args>
		struct function_scope_type<TReturn(T::*)(Args...) const&> : std::type_identity<const T&> {};

		template<typename T, typename TReturn, typename... Args>
		struct function_scope_type<TReturn(T::*)(Args...) volatile&> : std::type_identity<volatile T&> {};

		template<typename T, typename TReturn, typename... Args>
		struct function_scope_type<TReturn(T::*)(Args...) const volatile&> : std::type_identity<const volatile T&> {};

		template<typename T, typename TReturn, typename... Args>
		struct function_scope_type<TReturn(T::*)(Args...)&&> : std::type_identity<T&&> {};

		template<typename T, typename TReturn, typename... Args>
		struct function_scope_type<TReturn(T::*)(Args...) const&&> : std::type_identity<const T&&> {};

		template<typename T, typename TReturn, typename... Args>
		struct function_scope_type<TReturn(T::*)(Args...) volatile&&> : std::type_identity<volatile T&&> {};

		template<typename T, typename TReturn, typename... Args>
		struct function_scope_type<TReturn(T::*)(Args...) const volatile&&> : std::type_identity<const volatile T&&> {};

		template<typename T>
		using function_scope_type_t = typename function_scope_type<T>::type;

		// Functor traits
		template<typename Functor>
		struct functor_return_type
			: std::type_identity<function_return_type_t<decltype(&Functor::operator())>> {};

		template<typename... Args>
		using functor_return_type_t = typename functor_return_type<Args...>::type;

		template<typename Functor>
		struct functor_args_type
			: std::type_identity<function_args_type_t<decltype(&Functor::operator())>> {};

		template<typename... Args>
		using functor_args_type_t = typename functor_args_type<Args...>::type;

		// Method traits
		template<typename T>
		struct is_member_function_ptr : std::false_type {};

		template<typename T, typename TReturn, typename... Args>
		struct is_member_function_ptr<TReturn(T::*)(Args...)> : std::true_type {};

		template<typename T, typename TReturn, typename... Args>
		struct is_member_function_ptr<TReturn(T::*)(Args...) const> : std::true_type {};

		template<typename T, typename TReturn, typename... Args>
		struct is_member_function_ptr<TReturn(T::*)(Args...) volatile> : std::true_type {};

		template<typename T, typename TReturn, typename... Args>
		struct is_member_function_ptr<TReturn(T::*)(Args...) const volatile> : std::true_type {};

		template<typename T, typename TReturn, typename... Args>
		struct is_member_function_ptr<TReturn(T::*)(Args...)&> : std::true_type {};

		template<typename T, typename TReturn, typename... Args>
		struct is_member_function_ptr<TReturn(T::*)(Args...) const&> : std::true_type {};

		template<typename T, typename TReturn, typename... Args>
		struct is_member_function_ptr<TReturn(T::*)(Args...) volatile&> : std::true_type {};

		template<typename T, typename TReturn, typename... Args>
		struct is_member_function_ptr<TReturn(T::*)(Args...) const volatile&> : std::true_type {};

		template<typename T, typename TReturn, typename... Args>
		struct is_member_function_ptr<TReturn(T::*)(Args...)&&> : std::true_type {};

		template<typename T, typename TReturn, typename... Args>
		struct is_member_function_ptr<TReturn(T::*)(Args...) const&&> : std::true_type {};

		template<typename T, typename TReturn, typename... Args>
		struct is_member_function_ptr<TReturn(T::*)(Args...) volatile&&> : std::true_type {};

		template<typename T, typename TReturn, typename... Args>
		struct is_member_function_ptr<TReturn(T::*)(Args...) const volatile&&> : std::true_type {};

		template<typename T>
		inline constexpr bool is_member_function_ptr_v = is_member_function_ptr<T>::value;

		// Callable traits
		template<typename Callable, typename = void>
		struct callable_return_type : function_return_type<Callable> {};

		template<typename Callable>
		struct callable_return_type<Callable, std::void_t<
			decltype(&Callable::operator())
			>> : functor_return_type<Callable> {};

		template<typename Callable, typename = void>
		struct callable_args_type : function_args_type<Callable> {};

		template<typename Callable>
		struct callable_args_type<Callable, std::void_t<
			decltype(&Callable::operator())
			>> : functor_args_type<Callable> {};
	}
}
