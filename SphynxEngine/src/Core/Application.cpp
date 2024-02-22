#include "Application.h"

#include "Renderer/Window.h"
#include "Input/Input.h"
#include "Renderer/RendererAPI.h"
#include "Events/Event.h"
#include "Events/WindowEvent.h"
#include <SDL.h>

namespace Sphynx 
{
	Application* Application::s_Application = nullptr;

	Application::Application() :
		m_IsRunning(false),
		m_Window(nullptr)
	{
		s_Application = this;
	}

	Application::~Application()
	{
	}

	Application* Sphynx::Application::GetInstance()
	{
		return s_Application;
	}

	const Window* Sphynx::Application::GetWindow()
	{
		return m_Window.get();
	}

	const RendererAPI* Application::GetRenderer()
	{
		return m_Renderer.get();
	}

	void Application::Init()
	{
		m_Window.reset(Window::Create({ "Sphynx Application", 1280, 720 }));
		m_Renderer.reset(RendererAPI::Create());

		// event dispatcher
		m_Window->SetEventCallbackFunction([&](Event& event)
			{
				EventDispatcher dispatcher(event);
				dispatcher.Dispatch<WindowClosedEvent>([&](WindowClosedEvent& windowClosedEvent)
					{
						m_IsRunning = false;
						return true;
					});
			});

	}

	void Application::Run()
	{
		m_IsRunning = true;
		while (m_IsRunning)
		{
			m_Renderer->Clear();

			//m_Renderer->DrawPoint(0, 0, 255, 0, 0, 255);
			//m_Renderer->DrawLine(0, 0, 50, 50, 255, 0, 0, 255);
			//m_Renderer->DrawQuad(0, 0, 1280/2, 720/2, 255, 0, 0, 255);
			m_Renderer->DrawTriangle(50, 50, 100, 50, 10, 0, 255, 0, 0, 255);

			m_Renderer->Present();

			m_Window->Update();
		}
	}

	void Application::Shutdown()
	{
		m_Renderer.reset();
		m_Window.reset();
	}

	void Application::HandleEvent(Event& event)
	{
		// TODO: propagate events to listeners (?)
	}
}