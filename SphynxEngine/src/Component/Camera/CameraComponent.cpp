#include "spxpch.h"
#include "CameraComponent.h"

#include <glm/gtc/matrix_transform.hpp>


namespace Sphynx
{
	CameraComponent::CameraComponent(CameraProjectionMode mode, CameraInfo info) :
		IsMainCamera(false),
		m_ProjectionMode(mode),
		m_CameraInfo(info),
		m_ProjectionMatrix()
	{
		RecalculateProjectionMatrix();
	}

	CameraProjectionMode CameraComponent::GetProjectionMode() const
	{
		return m_ProjectionMode;
	}

	void CameraComponent::SetProjectionMode(CameraProjectionMode mode)
	{
		if (m_ProjectionMode == mode) return;

		m_ProjectionMode = mode;
		RecalculateProjectionMatrix();
	}

	float CameraComponent::GetNear() const
	{
		return m_CameraInfo.Near;
	}

	void CameraComponent::SetNear(float camNear)
	{
		m_CameraInfo.Near = camNear;
		RecalculateProjectionMatrix();
	}

	float CameraComponent::GetFar() const
	{
		return m_CameraInfo.Far;
	}

	void CameraComponent::SetFar(float camFar)
	{
		m_CameraInfo.Far = camFar;
		RecalculateProjectionMatrix();
	}

	float CameraComponent::GetHeightUnits() const
	{
		return m_CameraInfo.HeightUnits;
	}

	void CameraComponent::SetHeightUnits(float heightUnits)
	{
		m_CameraInfo.HeightUnits = heightUnits;
		if (m_ProjectionMode == ORTHOGRAPHIC)
		{
			RecalculateProjectionMatrix();
		}
	}

	float CameraComponent::GetFieldOfView() const
	{
		return m_CameraInfo.FieldOfView;
	}

	void CameraComponent::SetFieldOfView(float fieldOfView)
	{
		m_CameraInfo.FieldOfView = fieldOfView;
		if (m_ProjectionMode == PERSPECTIVE)
		{
			RecalculateProjectionMatrix();
		}
	}

	void CameraComponent::RecalculateProjectionMatrix()
	{
		const float aspectRatio = 1.0f;
		switch (m_ProjectionMode)
		{
		case ORTHOGRAPHIC:
		{
			const float height = m_CameraInfo.HeightUnits;
			const float width = aspectRatio * height;
			m_ProjectionMatrix = glm::ortho(-width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f, m_CameraInfo.Near, m_CameraInfo.Far);
		}
		break;
		case PERSPECTIVE:
		{
			m_ProjectionMatrix = glm::perspective(glm::radians(m_CameraInfo.FieldOfView), aspectRatio, m_CameraInfo.Near, m_CameraInfo.Far);
		}
		break;
		default:
			break;
		}
	}
}