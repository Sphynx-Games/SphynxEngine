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

			TryProcessWindowEvent(event);
			TryProcessKeyboardEvent(event);
			TryProcessMouseEvent(event);
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
			uint32_t flags = (m_Params.IsFullscreen ? SDL_WINDOW_FULLSCREEN : 0) | SDL_WINDOW_RESIZABLE;

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
	void WindowsWindow::TryProcessWindowEvent(SDL_Event& event)
	{
		switch (event.type)
		{
		case SDL_EventType::SDL_EVENT_QUIT:
		{
			WindowClosedEvent windowClosedEvent;
			m_CallbackFunction(windowClosedEvent);
		}
		break;
		case SDL_EventType::SDL_EVENT_WINDOW_SHOWN:
		{
			WindowShownEvent windowShownEvent;
			m_CallbackFunction(windowShownEvent);
		}
		break;
		case SDL_EventType::SDL_EVENT_WINDOW_HIDDEN:
		{
			WindowHiddenEvent windowHiddenEvent;
			m_CallbackFunction(windowHiddenEvent);
		}
		break;
		case SDL_EventType::SDL_EVENT_WINDOW_EXPOSED:
		{
			// not supported
		}
		break;
		case SDL_EventType::SDL_EVENT_WINDOW_MOVED:
		{
			WindowMovedEvent windowMovedEvent(event.window.data1, event.window.data2);
			m_CallbackFunction(windowMovedEvent);
		}
		break;
		case SDL_EventType::SDL_EVENT_WINDOW_RESIZED:
		{
			WindowResizedEvent windowResizedEvent(event.window.data1, event.window.data2);
			m_CallbackFunction(windowResizedEvent);
		}
		break;
		case SDL_EventType::SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
		{
			// not supported
		}
		break;
		case SDL_EventType::SDL_EVENT_WINDOW_MINIMIZED:
		{
			WindowMinimizedEvent windowMinimizedEvent;
			m_CallbackFunction(windowMinimizedEvent);
		}
		break;
		case SDL_EventType::SDL_EVENT_WINDOW_MAXIMIZED:
		{
			WindowMaximizedEvent windowMaximizedEvent;
			m_CallbackFunction(windowMaximizedEvent);
		}
		break;
		case SDL_EventType::SDL_EVENT_WINDOW_RESTORED:
		{
			WindowRestoredEvent windowRestoredEvent;
			m_CallbackFunction(windowRestoredEvent);
		}
		break;
		case SDL_EventType::SDL_EVENT_WINDOW_MOUSE_ENTER:
		{
			WindowMouseEnterEvent windowMouseEnterEvent;
			m_CallbackFunction(windowMouseEnterEvent);
		}
		break;
		case SDL_EventType::SDL_EVENT_WINDOW_MOUSE_LEAVE:
		{
			WindowMouseExitEvent windowMouseExitEvent;
			m_CallbackFunction(windowMouseExitEvent);
		}
		break;
		case SDL_EventType::SDL_EVENT_WINDOW_FOCUS_GAINED:
		{
			WindowFocusGainedEvent windowFocusGainedEvent;
			m_CallbackFunction(windowFocusGainedEvent);
		}
		break;
		case SDL_EventType::SDL_EVENT_WINDOW_FOCUS_LOST:
		{
			WindowFocusLostEvent windowFocusLostEvent;
			m_CallbackFunction(windowFocusLostEvent);
		}
		break;
		case SDL_EventType::SDL_EVENT_WINDOW_CLOSE_REQUESTED:
		{
			// not supported
			// NOTE: this might be important to look
		}
		break;
		case SDL_EventType::SDL_EVENT_WINDOW_TAKE_FOCUS:
		{
			// not supported
		}
		break;
		case SDL_EventType::SDL_EVENT_WINDOW_HIT_TEST:
		{
			// not supported
		}
		break;
		case SDL_EventType::SDL_EVENT_WINDOW_ICCPROF_CHANGED:
		{
			// not supported
		}
		break;
		case SDL_EventType::SDL_EVENT_WINDOW_DISPLAY_CHANGED:
		{
			// not supported
		}
		break;
		case SDL_EventType::SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED:
		{
			// not supported
		}
		break;
		case SDL_EventType::SDL_EVENT_WINDOW_OCCLUDED:
		{
			// not supported
		}
		break;
		case SDL_EventType::SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
		{
			WindowFullscreenChangedEvent windowFullscreenEvent(true);
			m_CallbackFunction(windowFullscreenEvent);
		}
		break;
		case SDL_EventType::SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
		{
			WindowFullscreenChangedEvent windowFullscreenEvent(false);
			m_CallbackFunction(windowFullscreenEvent);
		}
		break;
		case SDL_EventType::SDL_EVENT_WINDOW_DESTROYED:
		{
			// not supported
		}
		break;
		}
	}

	void WindowsWindow::TryProcessKeyboardEvent(SDL_Event& event)
	{
		// TODO: process keyboard events
	}

	void WindowsWindow::TryProcessMouseEvent(SDL_Event& event)
	{
		// TODO: process mouse events
	}
}
