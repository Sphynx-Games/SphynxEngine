#include "spxpch.h"
#include "EditorCameraController.h"
#include "Renderer/Renderer2D.h"
#include "Input/Input.h"
#include "Input/Keycode.h"
#include "Math/Transform.h"

//#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>


namespace Sphynx
{
	EditorCameraController::EditorCameraController(bool allowRotation) :
		m_AllowRotation(allowRotation),
		m_ProjectionMode(ORTHOGRAPHIC),
		m_CameraInfo(),
		m_AspectRatio(1.0f),
		m_Zoom(1.0f),
		m_Position({ 0.0f, 0.0f, 1.0f }),
		m_Rotation(0.0f),
		m_TranslationSpeed(1.5f),
		m_RotationSpeed(30.0f),
		m_Camera()
	{
		RecalculateProjectionMatrix();
	}

	void EditorCameraController::Update(float deltaTime)
	{
		float radRotation = glm::radians(m_Rotation);
		float cosValue = cos(radRotation) * m_TranslationSpeed * deltaTime;
		float sinValue = sin(radRotation) * m_TranslationSpeed * deltaTime;

		if (Input::IsKeyDown(SPX_KEY_A))
		{
			m_Position.X -= cosValue;
			m_Position.Y -= sinValue;
		}
		else if (Input::IsKeyDown(SPX_KEY_D))
		{
			m_Position.X += cosValue;
			m_Position.Y += sinValue;
		}

		if (Input::IsKeyDown(SPX_KEY_W))
		{
			m_Position.X += -sinValue;
			m_Position.Y += cosValue;
		}
		else if (Input::IsKeyDown(SPX_KEY_S))
		{
			m_Position.X -= -sinValue;
			m_Position.Y -= cosValue;
		}

		if (m_AllowRotation)
		{
			if (Input::IsKeyDown(SPX_KEY_Q))
			{
				m_Rotation += m_RotationSpeed * deltaTime;
			}
			if (Input::IsKeyDown(SPX_KEY_E))
			{
				m_Rotation -= m_RotationSpeed * deltaTime;
			}

			if (m_Rotation > 180.0f)
			{
				m_Rotation -= 360.0f;
			}
			else if (m_Rotation <= -180.0f) 
			{
				m_Rotation += 360.0f;
			}
		}

		m_TranslationSpeed = m_Zoom;

		// set view matrix
		glm::vec3 position = glm::vec3(m_Position.X, m_Position.Y, m_Position.Z);
		if (m_ProjectionMode == PERSPECTIVE)
		{
			position.z *= m_Zoom;
		}
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::mat4_cast(glm::quat({ 0.0f, 0.0f, glm::radians(m_Rotation) }));
		m_Camera.ViewMatrix = glm::inverse(transform);

		// set view projection matrix
		m_Camera.ViewProjectionMatrix = m_Camera.ProjectionMatrix * m_Camera.ViewMatrix;
	}

	void EditorCameraController::HandleEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>([&](MouseScrolledEvent& e)
			{
				m_Zoom -= e.GetY() * 0.25f;
				SetZoom(m_Zoom);

				return false;
			});
	}

	void EditorCameraController::SetAspectRatio(float aspectRatio)
	{
		if (m_AspectRatio == aspectRatio) return;

		m_AspectRatio = aspectRatio;
		RecalculateProjectionMatrix();
	}

	void EditorCameraController::SetZoom(float zoom)
	{
		m_Zoom = std::max(zoom, 10e-8f);

		RecalculateProjectionMatrix();
	}

	void EditorCameraController::RecalculateProjectionMatrix()
	{
		switch (m_ProjectionMode)
		{
		case ORTHOGRAPHIC:
		{
			const float height = m_CameraInfo.HeightUnits;
			const float width = m_AspectRatio * height;
			m_Camera.ProjectionMatrix = glm::ortho((-width / 2.0f) * m_Zoom, (width / 2.0f) * m_Zoom, (-height / 2.0f) * m_Zoom, (height / 2.0f) * m_Zoom, m_CameraInfo.Near, m_CameraInfo.Far);
		}
		break;
		case PERSPECTIVE:
		{
			m_Camera.ProjectionMatrix = glm::perspective(glm::radians(m_CameraInfo.FieldOfView), m_AspectRatio, m_CameraInfo.Near, m_CameraInfo.Far);
		}
		break;
		default:
			break;
		}
	}
}