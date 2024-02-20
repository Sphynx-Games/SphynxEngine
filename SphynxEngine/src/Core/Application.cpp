#include "Application.h"
#include <SDL.h>

#include "Renderer/Window.h"
#include "Events/Event.h"
#include "Events/WindowEvent.h"


Sphynx::Application::Application() :
	m_IsRunning(false),
	m_Window(nullptr)
{
}

Sphynx::Application::~Application()
{
}

void Sphynx::Application::Init()
{
	m_Window.reset(Window::Create({ "Sphynx Application", 1280, 720 }));

	// event dispatcher
	m_Window->SetEventCallbackFunction([&](Event& event)
		{
			EventDispatcher dispatcher(event);
			dispatcher.Dispatch<WindowClosedEvent>([&](WindowClosedEvent& windowClosedEvent)
				{
					m_IsRunning = false;
					return true;
				});
			dispatcher.Dispatch<WindowResizedEvent>([](WindowResizedEvent& windowResizeEvent)
				{
					// TODO: tell the renderer that the windows was resized
					return false;
				});

			HandleEvent(event);
		});

}

void Sphynx::Application::Run()
{
	m_IsRunning = true;
	while (m_IsRunning)
	{
		m_Window->Update();
	}
}

void Sphynx::Application::Shutdown()
{
	m_Window.reset();
}

void Sphynx::Application::HandleEvent(Event& event)
{
	// TODO: propagate events to listeners (?)
}
