#pragma once

#include "Core.h"

namespace Sphynx
{
	class Window;
	class Layer;
	class LayerStack;

	class SPHYNX_API Application
	{
	public:
		Application(Application& other) = delete;
		virtual ~Application();

		static Application* GetInstance();

		void operator=(const Application&) = delete;

		const Window* GetWindow();

		virtual void Init();
		virtual void Run();
		virtual void Shutdown();

		virtual void HandleEvent(class Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

	protected:
		Application();

	protected:
		static Application* s_Application;

	private:
		bool m_IsRunning;
		std::unique_ptr<Window> m_Window;
		std::unique_ptr<LayerStack> m_LayerStack;

		// This should not be here, only for testing purposes (move to Sandbox)
		std::unique_ptr<class OrthographicCameraController> m_CameraController;
	};

	// Should be defined in client
	Application* CreateApplication();
}
