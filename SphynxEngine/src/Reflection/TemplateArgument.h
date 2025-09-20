#pragma once

#include "Core/Core.h"
#include "Traits/Traits.h"


namespace Sphynx
{
	namespace Reflection
	{
		struct Type;

		struct SPHYNX_API TemplateArgument
		{
			enum class SPHYNX_API Kind
			{
				Type,
				Value,
			};

			Kind Kind;
			QualifiedType QualifiedType;
			uint64_t Value;
		};

		namespace details
		{
			template<typename T>
			static QualifiedType GenerateQualifiedType()
			{
				using cleared_property_type = typename ::Sphynx::Traits::remove_pointers<typename std::remove_reference<typename std::remove_cv<T>::type>::type>::type;
				using qualifier_mask_type = ::Sphynx::Reflection::Qualifier::Mask;
				constexpr qualifier_mask_type qualifierMask =
					((std::is_const<T>::value ? 1 : 0) * static_cast<qualifier_mask_type>(::Sphynx::Reflection::Qualifier::CONSTANT)) |
					((std::is_volatile<T>::value ? 1 : 0) * static_cast<qualifier_mask_type>(::Sphynx::Reflection::Qualifier::VOLATILE));
				
				constexpr ValueType valueType = []() 
					{
						if constexpr (std::is_lvalue_reference<T>::value)
							return ::Sphynx::Reflection::ValueType::LVALUE_REFERENCE;
						else if constexpr (std::is_rvalue_reference<T>::value)
							return ::Sphynx::Reflection::ValueType::RVALUE_REFERENCE;
						else
							return ::Sphynx::Reflection::ValueType::VALUE;
					}();

				return {
					::Sphynx::Reflection::GetType<cleared_property_type>(),
					qualifierMask,
					valueType,
					::Sphynx::Traits::pointer_indirection_count<T>::value
				};
			}

			template<typename ArgsPack, size_t... Indices>
			static auto GetTemplateArgumentPackArray(std::index_sequence<Indices...> = {})
			{
				std::array<TemplateArgument, ::Sphynx::Traits::args_pack_size<ArgsPack>::value> arguments{
					TemplateArgument{
						TemplateArgument::Kind::Type,
						GenerateQualifiedType<typename ::Sphynx::Traits::args_pack_element<ArgsPack, Indices>::type>(),
						0 
					}...
				};

				return arguments;
			}
		}
	}
}

