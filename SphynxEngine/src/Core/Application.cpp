#include "Application.h"

#include "Renderer/Window.h"
#include "Input/Input.h"
#include "Renderer/Renderer2D.h"
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

	void Application::Init()
	{
		m_Window.reset(Window::Create({ "Sphynx Application", 1280, 720 }));
		Renderer2D::Init();

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
			//Renderer2D::SetClearColor({ 0, 255, 0, 255 });
			Renderer2D::Begin();
			
			//Renderer2D::DrawPoint({0, 0}, {255, 0, 0, 255});
			Renderer2D::DrawLine({ 0,0 }, { 100,50 }, { 206, 16, 236, 255 });
			//Renderer2D::DrawQuad({0, 0}, {1280/2, 720/2}, {255, 0, 0, 255});
			//Renderer2D::DrawTriangle({50, 50}, {100, 50}, {10, 0}, {255, 0, 0, 255});
			Renderer2D::DrawCircle({ 400,400 }, 200, 50, { 206, 16, 236, 255 });

			Renderer2D::End();

			m_Window->Update();
		}
	}

	void Application::Shutdown()
	{
		Renderer2D::Shutdown();
		m_Window.reset();
	}

	void Application::HandleEvent(Event& event)
	{
		// TODO: propagate events to listeners (?)
	}
}