#include "OrthographicCameraController.h"
#include "Input/Input.h"
#include "Input/Keycode.h"

namespace Sphynx
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool allowRotation) :
		m_AllowRotation(allowRotation),
		m_AspectRatio(aspectRatio),
		m_Zoom(1.0f),
		m_Camera(-m_AspectRatio * m_Zoom, m_AspectRatio* m_Zoom, -m_Zoom, m_Zoom),
		m_Position({ 0.0f, 0.0f, 0.0f }),
	    m_Rotation(0.0f),
		m_TranslationSpeed(5.0f),
		m_RotationSpeed(1.0f)
	{
	}

	void OrthographicCameraController::Update()
	{
		float radRotation = glm::radians(m_Rotation);
		float cosValue = cos(radRotation) * m_TranslationSpeed /** ts*/;
		float sinValue = sin(radRotation) * m_TranslationSpeed /** ts*/;

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
				m_Rotation += m_RotationSpeed /** ts*/;
			}
			if (Input::IsKeyDown(SPX_KEY_E))
			{
				m_Rotation -= m_RotationSpeed /** ts*/;
			}

			if (m_Rotation > 180.0f)
			{
				m_Rotation -= 360.0f;
			}
			else if (m_Rotation <= -180.0f) 
			{
				m_Rotation += 360.0f;
			}

			m_Camera.SetRotation(m_Rotation);
		}

		m_Camera.SetPosition(m_Position);
		m_TranslationSpeed = m_Zoom;
	}

	void OrthographicCameraController::HandleEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>([&](MouseScrolledEvent& e)
			{
				m_Zoom -= e.GetY() * 0.25f;
				m_Zoom = std::max(m_Zoom, 0.25f);
				m_Camera.SetProjection(-m_AspectRatio * m_Zoom, m_AspectRatio * m_Zoom, -m_Zoom, m_Zoom);

				return false;
			});
		dispatcher.Dispatch<WindowResizedEvent>([&](WindowResizedEvent& e)
			{
				m_AspectRatio = (float)e.GetWidth(), (float)e.GetHeight();
				m_Camera.SetProjection(-m_AspectRatio * m_Zoom, m_AspectRatio * m_Zoom, -m_Zoom, m_Zoom);

				return false;
			});
	}
}