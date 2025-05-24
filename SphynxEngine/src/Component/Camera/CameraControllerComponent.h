#pragma once

#include "Core/Core.h"

#include "Component/Common.h"
#include "CameraComponent.h"


namespace Sphynx
{
	struct SPHYNX_API CameraControllerComponent
	{
		COMPONENT_COMMON_BODY(CameraControllerComponent);

		CameraComponent* Camera;
	};
}


#include "Reflection/Reflection.h"


SPX_REFLECT_STRUCT_BEGIN(Sphynx::CameraControllerComponent, SPHYNX_API)
SPX_REFLECT_ATTRIBUTE(InternalComponent)
SPX_REFLECT_STRUCT_END(Sphynx::CameraControllerComponent, SPHYNX_API)