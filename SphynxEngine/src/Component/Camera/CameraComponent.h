#pragma once

#include "Core/Core.h"
#include "Component/Common.h"

#include <glm/glm.hpp>


namespace Sphynx
{
	enum SPHYNX_API CameraProjectionMode
	{
		ORTHOGRAPHIC, PERSPECTIVE
	};

	struct SPHYNX_API CameraInfo
	{
	public:
		float Near = -1.0f;
		float Far = 1.0f;
		uint8_t WidthUnits = 10;
		uint8_t HeightUnits = 10;
		float FieldOfView = 1.0f;
	};

	struct SPHYNX_API CameraComponent
	{
		COMPONENT_COMMON_BODY_NO_DEFAULT_CONSTRUCTOR(CameraComponent);

	public:
		CameraComponent(CameraProjectionMode mode = CameraProjectionMode::ORTHOGRAPHIC, CameraInfo info = CameraInfo());

		void SetProjectionMode(CameraProjectionMode mode);
		void SetNear(float camNear);
		void SetFar(float camFar);
		void SetWidthUnits(uint8_t widthUnits);
		void SetHeightUnits(uint8_t heightUnits);
		void SetFieldOfView(float fieldOfView);

		const glm::mat4& GetProjectionMatrix() { return m_ProjectionMatrix; }

	private:
		void RecalculateViewMatrix();

	private:
		CameraProjectionMode m_ProjectionMode;
		CameraInfo m_CameraInfo;

		//bool m_IsMainCamera;

		glm::mat4 m_ProjectionMatrix;
	};
}


//#include "Reflection/Reflection.h"
//
//
//SPX_REFLECT_ENUM_BEGIN(Sphynx::CameraProjectionMode)
//
//SPX_REFLECT_ENUM_VALUE(ORTHOGRAPHIC)
//SPX_REFLECT_ENUM_VALUE(PERSPECTIVE)
//
//SPX_REFLECT_ENUM_END(Sphynx::CameraProjectionMode)
//
//
//SPX_REFLECT_STRUCT_BEGIN(Sphynx::CameraInfo)
//
//SPX_REFLECT_PROPERTY(Near)
//SPX_REFLECT_PROPERTY(Far)
//SPX_REFLECT_PROPERTY(WidthUnits)
//SPX_REFLECT_PROPERTY(HeightUnits)
//SPX_REFLECT_PROPERTY(FieldOfView)
//
//SPX_REFLECT_STRUCT_END(Sphynx::CameraInfo)
//
//
//SPX_REFLECT_STRUCT_BEGIN(Sphynx::CameraComponent)
//
//SPX_REFLECT_PROPERTY(m_ProjectionMode)
//SPX_REFLECT_PROPERTY(m_CameraInfo)
//SPX_REFLECT_PROPERTY(m_ProjectionMatrix)
//
//SPX_REFLECT_STRUCT_END(Sphynx::CameraComponent)