#pragma once

#include "Core/Core.h"
#include "Core/UUID.h"
#include "Common.h"


namespace Sphynx
{
	struct SPHYNX_API UUIDComponent
	{
		COMPONENT_COMMON_BODY(UUIDComponent);

	public:
		UUIDComponent(const UUID& UUID) :
			UUID(UUID) {}

	public:
		UUID UUID;

	};
}

#include "Reflection/Reflection.h"


SPX_REFLECT_STRUCT_BEGIN(Sphynx::UUIDComponent)
SPX_REFLECT_ATTRIBUTE(InternalComponent);

SPX_REFLECT_PROPERTY(UUID)

SPX_REFLECT_STRUCT_END(Sphynx::UUIDComponent)