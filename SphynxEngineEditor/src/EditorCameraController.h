#pragma once

//#include "Core/Core.h"
#include "Renderer/Camera.h"
#include "Events/Event.h"
#include "Events/InputEvent.h"
#include "Events/WindowEvent.h"
#include "Math/Vector.h"


namespace Sphynx
{
	class EditorCameraController
	{
	public:
		EditorCameraController(bool allowRotation = false);

		void Update(float deltaTime);
		void HandleEvent(Event& event);

		const Camera& GetCamera() const { return m_Camera; }
		
		void SetAspectRatio(float aspectRatio);
		void SetZoom(float zoom);
		void SetPosition(const Vector3f& position) { m_Position = position; }
		void SetRotation(float rotation) { m_Rotation = rotation; }
		void SetTranslationSpeed(float speed) { m_TranslationSpeed = speed; }
		void SetRotationSpeed(float speed) { m_RotationSpeed = speed; }

	private:
		void RecalculateProjectionMatrix();

	private:
		bool m_AllowRotation;

		CameraProjectionMode m_ProjectionMode;
		CameraInfo m_CameraInfo;

		float m_AspectRatio;
		float m_Zoom;
		Vector3f m_Position;
		float m_Rotation;
		float m_TranslationSpeed;
		float m_RotationSpeed;

		Camera m_Camera;
	};
}