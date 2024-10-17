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
			enum class Kind
			{
				Type,
				Value,
			};

			/*TemplateArgument() = default;
			TemplateArgument(const TemplateArgument& other):
				Kind(other.Kind),
				Type(other.Type),
				Value(other.Value)
			{}*/

			Kind Kind;
			const Type& Type;
			uint64_t Value;
		};

		namespace details
		{
			template<typename ArgsPack, size_t... Indices>
			static auto GetTemplateArgumentPackArray(std::index_sequence<Indices...> = {})
			{
				std::array<TemplateArgument, ::Sphynx::Traits::args_pack_size<ArgsPack>::value> arguments{
					TemplateArgument{
						TemplateArgument::Kind::Type, 
						::Sphynx::Reflection::GetType<typename ::Sphynx::Traits::args_pack_element<ArgsPack, Indices>::type>(),
						0 
					}...
				};

				return arguments;
			}
		}
	}
}

