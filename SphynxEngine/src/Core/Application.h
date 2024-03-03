#pragma once

#include "Core.h"
#include "LayerStack.h"

namespace Sphynx
{
	class Window;
	class Layer;

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
		void AddTexture(const std::string& path);
		void AddTextures(const std::vector<const std::string&> paths);

	protected:
		Application();

	protected:
		static Application* s_Application;

	private:
		bool m_IsRunning;
		std::unique_ptr<Window> m_Window;
		LayerStack m_LayerStack;

	};

	// Should be defined in client
	Application* CreateApplication();
}