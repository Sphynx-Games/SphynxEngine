#pragma once

#include "Core/Core.h"
#include "Math/Vector.h"

#include <glm/glm.hpp>
#include "Camera.h"

namespace Sphynx
{
	class SPHYNX_API OrthographicCamera
	{
	public:
		OrthographicCamera(float left = -1.0f, float right = 1.0f, float bottom = -1.0f, float top = 1.0f);

		void SetProjection(float left, float right, float bottom, float top);

		const Vector3f& GetPosition() const { return m_Position; }
		float GetRotation() const { return m_Rotation; }
		const glm::mat4& GetProjectionMatrix() const { return m_Camera.ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_Camera.ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_Camera.ViewProjectionMatrix; }

		void SetPosition(const Vector3f& position);
		void SetRotation(float rotation);
		const Camera* GetCamera() const { return &m_Camera; }

	private:
		void RecalculateViewMatrix();

	private:
		Vector3f m_Position;
		float m_Rotation;

		Camera m_Camera;
	};
}