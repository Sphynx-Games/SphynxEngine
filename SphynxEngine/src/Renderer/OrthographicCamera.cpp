#include "OrthographicCamera.h"
#include <glm/gtc/matrix_transform.hpp>

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

	void OrthographicCamera::RecalculateViewMatrix()
	{
		glm::mat4 position = glm::translate(glm::mat4(1.0f), { m_Position.X, m_Position.Y, m_Position.Z });
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

		glm::mat4 transform = position * rotation;

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}