#include "WindowsWindow.h"
#include "Events/WindowEvent.h"


namespace Sphynx
{
	static uint32_t s_SDLWindowCount = 0;

	WindowsWindow::WindowsWindow(const WindowParams& params) :
		m_Params(),
		m_Window(nullptr),
		m_Surface(nullptr)
	{
		Init(params);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Update()
	{
		// TODO: Process the rest of the events too
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (m_CallbackFunction == nullptr) continue;

			if (event.type == SDL_EventType::SDL_EVENT_QUIT)
			{
				WindowClosedEvent windowClosedEvent;
				m_CallbackFunction(windowClosedEvent);
			}
		}
	}

	uint32_t WindowsWindow::GetWidth() const
	{
		return m_Params.Width;
	}

	uint32_t WindowsWindow::GetHeight() const
	{
		return m_Params.Height;
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		m_Params.IsVSync = enabled;
		// TODO: activate VSync in SDLWindow
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Params.IsVSync;
	}

	void* WindowsWindow::GetNativeWindow() const
	{
		return m_Window;
	}

	void WindowsWindow::SetEventCallbackFunction(EventCallbackFunction function)
	{
		m_CallbackFunction = function;
	}

	void WindowsWindow::Init(const WindowParams& params)
	{
		m_Params = params;

		// TODO: change printf when we have a LOG system
		printf("Creating window %s (%d, %d)\n", m_Params.Title, m_Params.Width, m_Params.Height);

		if (s_SDLWindowCount == 0)
		{
			// Init SDL
			if (SDL_Init(SDL_INIT_VIDEO) < 0)
			{
				printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
			}
			// Create window
			uint32_t flags = (m_Params.IsFullscreen ? SDL_WINDOW_FULLSCREEN : 0);

			m_Window = SDL_CreateWindow(m_Params.Title, m_Params.Width, m_Params.Height, flags);
			if (m_Window == nullptr)
			{
				printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			}
		}

		if (m_Window != nullptr)
		{
			++s_SDLWindowCount;

			// Get window surface
			m_Surface = SDL_GetWindowSurface(m_Window);

			// Fill the surface white
			SDL_FillSurfaceRect(m_Surface, NULL, SDL_MapRGB(m_Surface->format, 0xFF, 0xFF, 0xFF));

			// Update the surface
			SDL_UpdateWindowSurface(m_Window);
		}

		SetVSync(true);
	}

	void WindowsWindow::Shutdown()
	{
		// Destroy window
		SDL_DestroyWindow(m_Window);
		--s_SDLWindowCount;

		// Quit SDL subsystems
		if (s_SDLWindowCount == 0)
		{
			SDL_Quit();
		}
	}
}
