#include "OrthographicCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Sphynx
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top) : 
		m_Position({ 0.0f, 0.0f, 0.0f }),
		m_Rotation(0.0f),
		m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
		m_ViewMatrix(),
		m_ViewProjectionMatrix()
	{
		RecalculateViewMatrix();
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetPosition(const Vector3f& position)
	{
		m_Position = position; 
		RecalculateViewMatrix();
	}

	void OrthographicCamera::SetRotation(float rotation)
	{
		m_Rotation = rotation; 
		RecalculateViewMatrix();
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), { m_Position.X, m_Position.Y, m_Position.Z }) *
			glm::mat4_cast(glm::quat({ 0.0f, 0.0f, glm::radians(m_Rotation) }));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}