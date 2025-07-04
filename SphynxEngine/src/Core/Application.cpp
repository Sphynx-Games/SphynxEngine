#include "spxpch.h"
#include "Application.h"
#include "Renderer/Window.h"
#include "Renderer/Renderer2D.h"
#include "Physics/Physics2D.h"
#include "Input/Input.h"
#include "Core/Time.h"
#include "Events/Event.h"
#include "Events/WindowEvent.h"
#include "LayerStack.h"
#include "Layer.h"
#include "Module/ModuleManager.h"
#include "Command/CommandManager.h"
#include "Sound/SoundManager.h"


namespace Sphynx
{
	Application* Application::s_Application = nullptr;

	Application::Application() :
		m_IsRunning(false),
		m_Window(nullptr),
		m_LayerStack()
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

	Window* Sphynx::Application::GetWindow() const
	{
		return m_Window.get();
	}

	void Application::Init(const HashMap<std::string, Array<std::string>>& commandArguments)
	{
		// Manage the command line arguments
		CommandManager::Init();
		ManageCommandArguments(commandArguments);

		// Create window
		m_Window.reset(Window::Create({ "Sphynx Application", 1280, 720 }));

		// Initialize renderer
		Renderer2D::Init();

		// Initialize physics
		Physics2D::Init();

		// Initialize sounds
		SoundManager::Init();

		// Initialize input
		Input::Init();

		// Initialize application global time
		Time::Init();

		// Event dispatcher
		m_Window->SetEventCallbackFunction([&](Event& event)
			{
				EventDispatcher dispatcher(event);
				dispatcher.Dispatch<WindowClosedEvent>([&](WindowClosedEvent& windowClosedEvent)
					{
						Quit();
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
			// Update global time
			Time::Tick();
			float deltaTime = Time::GetScaledDeltaTime();

			// Update state before window processes the events
			Input::Update();

			// Update sounds
			SoundManager::Update();

			// Update window (process events)
			m_Window->Update();

			// Update layers
			for (Layer* layer : m_LayerStack)
			{
				layer->Update(deltaTime);
			}
		}
	}

	void Application::Shutdown()
	{
		// Unload possible loaded modules
		ModuleManager::UnloadAllModules();

		// Shut app global time
		Time::Shutdown();

		// Shut input
		Input::Shutdown();

		// Shut sounds
		SoundManager::Shutdown();

		// Shut physics
		Physics2D::Shutdown();

		// Shut renderer
		Renderer2D::Shutdown();

		// Destroy window
		m_Window.reset();

		// Shut commands
		CommandManager::Shutdown();
	}

	void Application::HandleEvent(Event& event)
	{
		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (event.IsHandled())
				break;

			(*it)->HandleEvent(event);
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
	}

	void Application::PopLayer(Layer* layer)
	{
		m_LayerStack.PopLayer(layer);
	}

	void Application::PopOverlay(Layer* layer)
	{
		m_LayerStack.PopOverlay(layer);
	}

	void Application::Quit()
	{
		m_IsRunning = false;
	}

	void Application::ManageCommandArguments(const HashMap<std::string, Array<std::string>>& commandArguments)
	{
		m_CommandArguments = commandArguments;

		InitApplicationCommands();
		for (auto& [key, values] : commandArguments)
		{
			CommandManager::ExecuteCommand(key, values);
		}
	}
}