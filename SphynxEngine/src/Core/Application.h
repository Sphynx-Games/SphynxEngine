#pragma once

#include "Core.h"



namespace Sphynx
{
	class Window;
	class RendererAPI;

	class SPHYNX_API Application
	{
	public:
		Application(Application& other) = delete;
		virtual ~Application();

		static Application* GetInstance();

		void operator=(const Application&) = delete;

		const Window* GetWindow();
		const RendererAPI* GetRenderer();

		virtual void Init();
		virtual void Run();
		virtual void Shutdown();

		virtual void HandleEvent(class Event& event);

	protected:
		Application();

	protected:
		static Application* s_Application;

	private:
		bool m_IsRunning;
		std::unique_ptr<Window> m_Window;
		std::unique_ptr<RendererAPI> m_Renderer;
	};

	// Should be defined in client
	Application* CreateApplication();
}
