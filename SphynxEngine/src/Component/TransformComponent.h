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

#include "Reflection/Reflection.h"


SPX_REFLECT_STRUCT_BEGIN(Sphynx::TransformComponent)
SPX_REFLECT_ATTRIBUTE(InternalComponent);

SPX_REFLECT_PROPERTY(Transform)

SPX_REFLECT_STRUCT_END(Sphynx::TransformComponent)
