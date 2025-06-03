#pragma once

#include "Core/Core.h"
#include "LayerStack.h"
#include "Container/Map.h"
#include "Core/Commands.h"
#include <memory>


namespace Sphynx
{
	class Window;
	class Layer;

	class SPHYNX_API Application
	{
	public:
		Application(const Application& other) = delete;
		virtual ~Application();
		void operator=(const Application&) = delete;

	public:
		static Application* GetInstance();

		Window* GetWindow() const;

		virtual void Init(const HashMap<CommandArgument, Array<std::string>>& options);
		virtual void Run();
		virtual void Shutdown();

		virtual void HandleEvent(class Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* layer);

		void Quit();

	protected:
		Application();

	private:
		void ManageOptions(const HashMap<CommandArgument, Array<std::string>>& options);

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