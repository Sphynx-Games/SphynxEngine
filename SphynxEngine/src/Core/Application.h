#pragma once

#include "Core.h"


namespace Sphynx
{
	class SPHYNX_API Application
	{
	public:
		Application();
		virtual ~Application();

		virtual void Init();
		virtual void Run();
		virtual void Shutdown();

		virtual void HandleEvent(class Event& event);

	private:
		bool m_IsRunning;
		std::shared_ptr<class Window> m_Window;

	};

	// Should be defined in client
	Application* CreateApplication();

}
