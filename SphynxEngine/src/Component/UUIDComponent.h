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
