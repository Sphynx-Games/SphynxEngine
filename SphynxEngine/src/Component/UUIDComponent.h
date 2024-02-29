#pragma once

#include "Core/Core.h"
#include "Common.h"
#include <cstdint>


namespace Sphynx
{
	struct SPHYNX_API UUIDComponent
	{
		COMPONENT_COMMON_BODY(UUIDComponent);

	public:
		UUIDComponent(uint32_t UUID) :
			UUID(UUID) {}

	public:
		uint32_t UUID;

	};
}
