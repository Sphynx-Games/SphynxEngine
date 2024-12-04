#include "spxpch.h"
#include "CameraComponent.h"

#include <glm/gtc/matrix_transform.hpp>


namespace Sphynx
{
	CameraComponent::CameraComponent(CameraProjectionMode mode, CameraInfo info) :
		m_ProjectionMode(mode),
		m_CameraInfo(info),
		m_ProjectionMatrix()
	{
		RecalculateViewMatrix();
	}

	void CameraComponent::SetProjectionMode(CameraProjectionMode mode)
	{
		if (m_ProjectionMode == mode) return;

		m_ProjectionMode = mode;
		RecalculateViewMatrix();
	}

	void CameraComponent::SetNear(float camNear)
	{
		m_CameraInfo.Near = camNear;
		RecalculateViewMatrix();
	}

	void CameraComponent::SetFar(float camFar)
	{
		m_CameraInfo.Far = camFar;
		RecalculateViewMatrix();
	}

	void CameraComponent::SetWidthUnits(uint8_t widthUnits)
	{
		m_CameraInfo.WidthUnits = widthUnits;
		if (m_ProjectionMode == ORTHOGRAPHIC)
		{
			RecalculateViewMatrix();
		}
	}

	void CameraComponent::SetHeightUnits(uint8_t heightUnits)
	{
		m_CameraInfo.HeightUnits = heightUnits;
		if (m_ProjectionMode == ORTHOGRAPHIC)
		{
			RecalculateViewMatrix();
		}
	}

	void CameraComponent::SetFieldOfView(float fieldOfView)
	{
		m_CameraInfo.FieldOfView = fieldOfView;
		if (m_ProjectionMode == PERSPECTIVE)
		{
			RecalculateViewMatrix();
		}
	}

	void CameraComponent::RecalculateViewMatrix()
	{
		switch (m_ProjectionMode)
		{
		case ORTHOGRAPHIC:
			float width = m_CameraInfo.WidthUnits;
			float height = m_CameraInfo.HeightUnits;
			m_ProjectionMatrix = glm::ortho(-width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f, m_CameraInfo.Near, m_CameraInfo.Far);
			break;
		case PERSPECTIVE:
			float aspectRatio = 1.0f; // lo saco del tamaño de la ventana
			m_ProjectionMatrix = glm::perspective(m_CameraInfo.FieldOfView, aspectRatio, m_CameraInfo.Near, m_CameraInfo.Far);
			break;
		default:
			break;
		}
	}
}