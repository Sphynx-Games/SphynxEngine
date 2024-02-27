#pragma once

#include "Math/Vector.h"
#include <glm/glm.hpp>

namespace Sphynx
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left = -1.0f, float right = 1.0f, float bottom = -1.0f, float top = 1.0f);

		void SetProjection(float left, float right, float bottom, float top);

		const Vector3f& GetPosition() const { return m_Position; }
		float GetRotation() const { return m_Rotation; }
		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

		void SetPosition(const Vector3f& position);
		void SetRotation(float rotation);

	private:
		void RecalculateViewMatrix();

	private:
		Vector3f m_Position;
		float m_Rotation;

		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;

		glm::mat4 m_ViewProjectionMatrix;
	};
}