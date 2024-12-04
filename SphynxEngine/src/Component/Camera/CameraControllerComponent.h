#pragma once

#include "Core/Core.h"
#include "Component/Common.h"
#include "CameraComponent.h"


namespace Sphynx
{

	struct SPHYNX_API CameraControllerComponent
	{
		COMPONENT_COMMON_BODY_NO_DEFAULT_CONSTRUCTOR(CameraControllerComponent);

	//public:
	};
}


#include "Reflection/Reflection.h"


SPX_REFLECT_STRUCT_BEGIN(Sphynx::CameraControllerComponent)

//SPX_REFLECT_PROPERTY(Camera)

SPX_REFLECT_STRUCT_END(Sphynx::CameraControllerComponent)