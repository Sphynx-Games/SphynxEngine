#pragma once

#include "Core/Core.h"
#include "Type.h"
#include "Attribute.h"
#include "Traits/Utility.h"


namespace Sphynx
{
	namespace Reflection
	{
		struct SPHYNX_API Function
		{
			struct SPHYNX_API Parameter
			{
				Parameter() = default;

				const char* Name;
				const Type& Type;

				std::vector<Attribute*> Attributes;
			};

			const char* Name;
			const Type& ReturnType;
			const Parameter* Parameters;
			size_t ParameterCount;
			void(*FunctionPtr)(void*, void*);

			std::vector<Attribute*> Attributes;
		};

		namespace details
		{
			template<typename ArgsPack, size_t... Indices>
			static auto GetParameterPackArray(std::index_sequence<Indices...> = {})
			{
				std::array<Function::Parameter, ::Sphynx::Traits::args_pack_size<ArgsPack>::value> params {
					Function::Parameter{ "", ::Sphynx::Reflection::GetType<
								typename std::remove_reference<
								typename std::remove_cv<typename ::Sphynx::Traits::args_pack_element<ArgsPack, Indices>::type>::type
								>::type>() }...
				};

				return params;
			}
		}
	}
}

