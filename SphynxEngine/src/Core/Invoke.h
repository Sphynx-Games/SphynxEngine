#pragma once

#include "Traits/Traits.h"


namespace Sphynx
{
	namespace details
	{
		template<typename Functor, size_t ...I>
		typename Traits::callable_return_type<typename std::remove_reference<Functor>::type>::type
			InvokeExpanded(Functor&& functor, void* params, std::index_sequence<I...>)
		{
			using args_pack_type = typename traits::callable_args_type<typename std::remove_reference<Functor>::type>::type;

			return functor(
				*reinterpret_cast<traits::args_pack_element_t<args_pack_type, I>*>(
					(char*)params + traits::args_pack_element_offset<args_pack_type, I>::value
					)...
			);
		}

		template<typename MemberFunc, size_t ...I>
		typename Traits::function_return_type<typename std::remove_reference<MemberFunc>::type>::type
			InvokeExpanded(
				typename std::conditional<
				std::is_reference<typename Traits::function_scope_type<MemberFunc>::type>::value,
				typename Traits::function_scope_type<MemberFunc>::type,
				typename Traits::function_scope_type<MemberFunc>::type&
				>::type obj,
				MemberFunc&& memfunc,
				void* params,
				std::index_sequence<I...>)
		{
			using function_type = typename std::remove_reference<MemberFunc>::type;
			using args_pack_type = typename Traits::callable_args_type<function_type>::type;

			return (static_cast<decltype(obj)>(obj).*memfunc)(
				*reinterpret_cast<Traits::args_pack_element_t<args_pack_type, I>*>(
					(char*)params + Traits::args_pack_element_offset<args_pack_type, I>::value
					)...
				);
		}
	}

	template<typename Functor>
	typename Traits::callable_return_type<typename std::remove_reference<Functor>::type>::type
		Invoke(Functor&& functor, void* params)
	{
		using function_type = typename std::remove_reference<Functor>::type;
		using args_pack_type = typename Traits::callable_args_type<function_type>::type;

		return details::InvokeExpanded(
			functor,
			params,
			std::make_index_sequence<Traits::args_pack_size<args_pack_type>::value>()
		);
	}

	template<typename MemberFunc>
	typename Traits::function_return_type<typename std::remove_reference<MemberFunc>::type>::type
		Invoke(
			typename std::conditional<
			std::is_reference<typename Traits::function_scope_type<MemberFunc>::type>::value,
			typename Traits::function_scope_type<MemberFunc>::type,
			typename Traits::function_scope_type<MemberFunc>::type&
			>::type obj,
			MemberFunc&& func,
			void* params)
	{
		using function_type = typename std::remove_reference<MemberFunc>::type;
		using args_pack_type = typename Traits::callable_args_type<function_type>::type;

		return details::InvokeExpanded(
			static_cast<decltype(obj)>(obj),
			std::forward<MemberFunc>(func),
			params,
			std::make_index_sequence<Traits::args_pack_size<args_pack_type>::value>()
		);
	}
}