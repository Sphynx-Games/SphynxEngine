#pragma once

#include "Core/Core.h"
#include "Reflection/Reflection.h"

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
		float Near = 0.01f;
		float Far = 10000.0f;
		float HeightUnits = 8.0f;
		float FieldOfView = 60.0f;
	};

	struct SPHYNX_API CameraComponent
	{
		SPX_REFLECT_GENERATED_BODY();
		COMPONENT_COMMON_BODY_NO_DEFAULT_CONSTRUCTOR(CameraComponent);

	public:
		CameraComponent(CameraProjectionMode mode = CameraProjectionMode::ORTHOGRAPHIC, CameraInfo info = CameraInfo());

		CameraProjectionMode GetProjectionMode() const;
		void SetProjectionMode(CameraProjectionMode mode);

		float GetNear() const;
		void SetNear(float camNear);

		float GetFar() const;
		void SetFar(float camFar);

		float GetHeightUnits() const;
		void SetHeightUnits(float heightUnits);

		float GetFieldOfView() const;
		void SetFieldOfView(float fieldOfView);

		const glm::mat4& GetProjectionMatrix() { return m_ProjectionMatrix; }

	private:
		void RecalculateViewMatrix();

	public:
		bool IsMainCamera;

	private:
		CameraProjectionMode m_ProjectionMode;
		CameraInfo m_CameraInfo;

		glm::mat4 m_ProjectionMatrix;
	};
}




SPX_REFLECT_ENUM_BEGIN(Sphynx::CameraProjectionMode)

SPX_REFLECT_ENUM_VALUE(ORTHOGRAPHIC)
SPX_REFLECT_ENUM_VALUE(PERSPECTIVE)

SPX_REFLECT_ENUM_END(Sphynx::CameraProjectionMode)


SPX_REFLECT_STRUCT_BEGIN(Sphynx::CameraInfo)

SPX_REFLECT_PROPERTY(Near)
SPX_REFLECT_PROPERTY(Far)
SPX_REFLECT_PROPERTY(HeightUnits)
SPX_REFLECT_PROPERTY(FieldOfView)

SPX_REFLECT_STRUCT_END(Sphynx::CameraInfo)


SPX_REFLECT_STRUCT_BEGIN(Sphynx::CameraComponent)

SPX_REFLECT_PROPERTY(IsMainCamera)
SPX_REFLECT_PROPERTY(m_ProjectionMode)
SPX_REFLECT_PROPERTY(m_CameraInfo)

SPX_REFLECT_STRUCT_END(Sphynx::CameraComponent)