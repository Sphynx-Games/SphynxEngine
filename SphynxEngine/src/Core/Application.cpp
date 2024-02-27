#include "Application.h"
#include "Renderer/Window.h"
#include "Input/Input.h"
#include "Events/Event.h"
#include "Events/WindowEvent.h"
#include "Renderer/Renderer2D.h"
#include "LayerStack.h"
#include "Layer.h"
#include "Renderer/OrthographicCameraController.h"
#include <SDL.h>

namespace Sphynx
{
	Application* Application::s_Application = nullptr;

	Application::Application() :
		m_IsRunning(false),
		m_Window(nullptr),
		m_LayerStack(nullptr),
		m_CameraController(new OrthographicCameraController(1.0f, true))
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
		m_LayerStack.reset(new LayerStack());
		m_Window.reset(Window::Create({ "Sphynx Application", 1280, 720 }));
		Input::Init();
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
				dispatcher.Dispatch<WindowResizedEvent>([](WindowResizedEvent& windowResizeEvent)
					{
						// TODO: tell the renderer that the windows was resized
						return false;
					});

				HandleEvent(event);
			});

	}

	void Application::Run()
	{
		m_IsRunning = true;
		while (m_IsRunning)
		{
			// update camera
			m_CameraController.get()->Update();

			//Renderer2D::SetClearColor(Color::White);

			//m_Camera->SetPosition({ 200.0f, 200.0f, 0.0f });
			//m_Camera->SetRotation(180.0f);

			// begin scene render
			Renderer2D::Begin(&m_CameraController.get()->GetCamera());

			for (Layer* layer : m_LayerStack.get()->get())
			{
				layer->Update();
			}

			/* World space draw commands tests */
			Transform quadTransform = { { 200.0f, 300.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } };
			Renderer2D::DrawQuad(DrawMode::FILLED, quadTransform, { 200, 100 }, { 0.5f, 0.5f }, Color::Blue);

			Transform triangleTransform = { { 100.0f, 125.0f, 0.0f }, { 5.0f, 15.0f, 1.0f }, { 0.0f, 0.0f, 180.0f } };
			Renderer2D::DrawTriangle(DrawMode::FILLED, triangleTransform, Vector2f{ -50.0f, 0.0f }, Vector2f{ 0.0f, 50.0f }, Vector2f{ 50.f, 0.0f }, Vector2f{0.5f, 1.0f}, Color::Red);

			Transform circleTransform = { { 250.0f, 125.0f, 0.0f }, { 5.5f, 1.5f, 1.0f }, { 0.0f, 0.0f, 180.0f } };
			Renderer2D::DrawCircle(DrawMode::FILLED, circleTransform, 10.f, 32, { 0.5f, 0.5f }, Color::Green);

			/* Screen-space draw commands tests */
			Renderer2D::DrawPoint({0, 0}, Color::Red);
			Renderer2D::DrawLine({ 0,0 }, { 100,50 }, Color{ 206, 16, 236, 255 });
			Renderer2D::DrawQuad({ 300, 600 }, {1280/2, 720/2}, Color{255, 0, 0, 255});
			Renderer2D::DrawTriangle(DrawMode::WIREFRAME, Vector2i{ 50, 50 }, Vector2i{ 100, 50 }, Vector2i{ 10, 0 }, Color::Red);

			//Renderer2D::DrawCircle(DrawMode::WIREFRAME, { 400,400 }, 200, 50, trans, { 206, 255, 236, 255 });

			Renderer2D::End();

			// update state before window processes the events
			Input::Update();

			// update window
			m_Window->Update();
		}
	}

	void Application::Shutdown()
	{
		Renderer2D::Shutdown();
		Input::Shutdown();
		m_Window.reset();
		m_LayerStack.reset();
	}

	void Application::HandleEvent(Event& event)
	{
		// TODO: propagate events to listeners (?)
		m_CameraController.get()->HandleEvent(event);

		for (auto it = m_LayerStack.get()->rbegin(); it != m_LayerStack.get()->rend(); ++it)
		{
			if (event.IsHandled())
				break;

			(*it)->HandleEvent(event);
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.get()->PushLayer(layer);
		layer->Attach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.get()->PushOverlay(layer);
		layer->Attach();
	}
}