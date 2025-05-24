#pragma once

#include "Core/Core.h"
#include "Reflection/Reflection.h"
#include "Renderer/Camera.h"
#include "Component/Common.h"

#include <glm/glm.hpp>


namespace Sphynx
{
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
		void RecalculateProjectionMatrix();

	public:
		bool IsMainCamera;

	private:
		CameraProjectionMode m_ProjectionMode;
		CameraInfo m_CameraInfo;

		glm::mat4 m_ProjectionMatrix;
	};
}


SPX_REFLECT_STRUCT_BEGIN(Sphynx::CameraComponent, SPHYNX_API)
SPX_REFLECT_ATTRIBUTE(InternalComponent)

SPX_REFLECT_PROPERTY(IsMainCamera)
SPX_REFLECT_PROPERTY(m_ProjectionMode)
SPX_REFLECT_PROPERTY(m_CameraInfo)

SPX_REFLECT_STRUCT_END(Sphynx::CameraComponent, SPHYNX_API)