#pragma once

#include "Core/Core.h"
#include "LayerStack.h"
#include <memory>
#include <Windows.h>
#include <tchar.h>


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
		void LoadProject(const std::filesystem::path& path);

		Window* GetWindow() const;

		virtual void Init();
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
		void UnloadProject();

	protected:
		static Application* s_Application;
		HMODULE m_ProjectDLL; // game project dll

	private:
		bool m_IsRunning;
		std::unique_ptr<Window> m_Window;
		LayerStack m_LayerStack;

	};

	// Should be defined in client
	Application* CreateApplication();
}