#pragma once

#include "Core/Core.h"
#include "Common.h"
#include "Math/Transform.h"


namespace Sphynx
{
	struct SPHYNX_API TransformComponent
	{
		COMPONENT_COMMON_BODY(TransformComponent);

	public:
		TransformComponent(const Transform& transform) :
			Transform(transform) {}

	public:
		Transform Transform;

	};
}
