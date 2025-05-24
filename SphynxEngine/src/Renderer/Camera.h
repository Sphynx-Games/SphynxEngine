#pragma once

#include "Core/Core.h"

#include <glm/glm.hpp>


namespace Sphynx
{
	struct SPHYNX_API Camera
	{
	public:
		glm::mat4 ProjectionMatrix;
		glm::mat4 ViewMatrix;

		glm::mat4 ViewProjectionMatrix;
	};


	enum SPHYNX_API CameraProjectionMode
	{
		ORTHOGRAPHIC, PERSPECTIVE
	};

	struct SPHYNX_API CameraInfo
	{
	public:
		float Near = 0.01f;
		float Far = 10000.0f;
		float HeightUnits = 8.0f;
		float FieldOfView = 60.0f;
	};
}


#include "Reflection/Reflection.h"

SPX_REFLECT_ENUM_BEGIN(Sphynx::CameraProjectionMode, SPHYNX_API)

SPX_REFLECT_ENUM_VALUE(ORTHOGRAPHIC)
SPX_REFLECT_ENUM_VALUE(PERSPECTIVE)

SPX_REFLECT_ENUM_END(Sphynx::CameraProjectionMode, SPHYNX_API)


SPX_REFLECT_STRUCT_BEGIN(Sphynx::CameraInfo, SPHYNX_API)

SPX_REFLECT_PROPERTY(Near)
SPX_REFLECT_PROPERTY(Far)
SPX_REFLECT_PROPERTY(HeightUnits)
SPX_REFLECT_PROPERTY(FieldOfView)

SPX_REFLECT_STRUCT_END(Sphynx::CameraInfo, SPHYNX_API)