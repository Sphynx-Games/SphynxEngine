#pragma once

#include "Core/Core.h"
#include "Common.h"
#include <string>


namespace Sphynx
{
	struct SPHYNX_API NameComponent
	{
		COMPONENT_COMMON_BODY(NameComponent);

	public:
		NameComponent(const std::string& name) :
			Name(name) {}

	public:
		std::string Name;

	};
}

#include "Reflection/Reflection.h"


SPX_REFLECT_STRUCT_BEGIN(Sphynx::NameComponent)

SPX_REFLECT_PROPERTY(Name)

SPX_REFLECT_STRUCT_END(Sphynx::NameComponent)