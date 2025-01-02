#include "spxpch.h"
#include "WindowsWindow.h"
#include "Logging/Log.h"

#include <SDL3/SDL.h>

#include "Renderer/GraphicsContext.h"
#include "Events/WindowEvent.h"
#include "Events/InputEvent.h"


namespace Sphynx
{
	static uint32_t s_SDLWindowCount = 0;

	WindowsWindow::WindowsWindow(const WindowParams& params) :
		m_Params(),
		m_Window(nullptr),
		m_WindowHandle(nullptr)
	{
		Init(params);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Update()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (m_CallbackFunction == nullptr) continue;
			ProcessEvent(event);
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

	HWND WindowsWindow::GetHWND() const
	{
		return m_WindowHandle;
	}

	void WindowsWindow::Init(const WindowParams& params)
	{
		m_Params = params;

		SPX_CORE_LOG_DISPLAY("Creating window {} ({}, {})", m_Params.Title, m_Params.Width, m_Params.Height);

		if (s_SDLWindowCount == 0)
		{
			// Init SDL
			if (!SDL_InitSubSystem(SDL_INIT_VIDEO))
			{
				SPX_CORE_LOG_ERROR("SDL could not initialize! SDL_Error: {}", SDL_GetError());
			}
		}

		// Create graphics context
		m_GraphicsContext = GraphicsContext::Create(this);
		if (m_GraphicsContext != nullptr)
		{
			m_GraphicsContext->OnBeforeWindowCreation();
		}

		// Create window
		uint32_t flags = (m_Params.IsFullscreen ? SDL_WINDOW_FULLSCREEN : 0) | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;
		m_Window = SDL_CreateWindow(m_Params.Title, m_Params.Width, m_Params.Height, flags);
		if (m_Window == nullptr)
		{
			SPX_CORE_LOG_ERROR("Window could not be created! SDL_Error: {}", SDL_GetError());
		}
			
		// Get Windows window handle
		m_WindowHandle = (HWND)SDL_GetPointerProperty(SDL_GetWindowProperties(m_Window), SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);

		if (m_Window != nullptr)
		{
			++s_SDLWindowCount;
		}

		SetVSync(true);

		if (m_GraphicsContext != nullptr)
		{
			m_GraphicsContext->OnAfterWindowCreation();
			m_GraphicsContext->Init();
		}
	}

	void WindowsWindow::Shutdown()
	{
		if (m_GraphicsContext != nullptr)
		{
			m_GraphicsContext->Shutdown();
			delete m_GraphicsContext;
			m_GraphicsContext = nullptr;
		}

		// Destroy window
		SDL_DestroyWindow(m_Window);
		--s_SDLWindowCount;

		// Quit SDL subsystem
		if (s_SDLWindowCount == 0)
		{
			SDL_QuitSubSystem(SDL_INIT_VIDEO);
		}
	}

	void WindowsWindow::ProcessEvent(SDL_Event& event)
	{
		if (TryProcessWindowEvent(event)) return;
		if (TryProcessKeyboardEvent(event)) return;
		if (TryProcessMouseEvent(event)) return;
	}

	bool WindowsWindow::TryProcessWindowEvent(SDL_Event& event)
	{
		switch (event.type)
		{
		case SDL_EventType::SDL_EVENT_QUIT:
		{
			WindowClosedEvent windowClosedEvent{ event.window.windowID };
			m_CallbackFunction(windowClosedEvent);
			return true;
		}
		case SDL_EventType::SDL_EVENT_WINDOW_SHOWN:
		{
			WindowShownEvent windowShownEvent{ event.window.windowID };
			m_CallbackFunction(windowShownEvent);
			return true;
		}
		case SDL_EventType::SDL_EVENT_WINDOW_HIDDEN:
		{
			WindowHiddenEvent windowHiddenEvent{ event.window.windowID };
			m_CallbackFunction(windowHiddenEvent);
			return true;
		}
		case SDL_EventType::SDL_EVENT_WINDOW_EXPOSED:
		{
			// not supported
		}
		break;
		case SDL_EventType::SDL_EVENT_WINDOW_MOVED:
		{
			WindowMovedEvent windowMovedEvent{ event.window.windowID, (uint32_t)event.window.data1, (uint32_t)event.window.data2 };
			m_CallbackFunction(windowMovedEvent);
			return true;
		}
		case SDL_EventType::SDL_EVENT_WINDOW_RESIZED:
		{
			WindowResizedEvent windowResizedEvent{ event.window.windowID, (uint32_t)event.window.data1, (uint32_t)event.window.data2 };
			m_Params.Width = (uint32_t)event.window.data1;
			m_Params.Height = (uint32_t)event.window.data2;
			m_CallbackFunction(windowResizedEvent);
			return true;
		}
		case SDL_EventType::SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
		{
			// not supported
		}
		break;
		case SDL_EventType::SDL_EVENT_WINDOW_MINIMIZED:
		{
			WindowMinimizedEvent windowMinimizedEvent{ event.window.windowID };
			m_CallbackFunction(windowMinimizedEvent);
			return true;
		}
		case SDL_EventType::SDL_EVENT_WINDOW_MAXIMIZED:
		{
			WindowMaximizedEvent windowMaximizedEvent{ event.window.windowID };
			m_CallbackFunction(windowMaximizedEvent);
			return true;
		}
		case SDL_EventType::SDL_EVENT_WINDOW_RESTORED:
		{
			WindowRestoredEvent windowRestoredEvent{ event.window.windowID };
			m_CallbackFunction(windowRestoredEvent);
			return true;
		}
		case SDL_EventType::SDL_EVENT_WINDOW_MOUSE_ENTER:
		{
			WindowMouseEnterEvent windowMouseEnterEvent{ event.window.windowID };
			m_CallbackFunction(windowMouseEnterEvent);
			return true;
		}
		case SDL_EventType::SDL_EVENT_WINDOW_MOUSE_LEAVE:
		{
			WindowMouseExitEvent windowMouseExitEvent{ event.window.windowID };
			m_CallbackFunction(windowMouseExitEvent);
			return true;
		}
		case SDL_EventType::SDL_EVENT_WINDOW_FOCUS_GAINED:
		{
			WindowFocusGainedEvent windowFocusGainedEvent{ event.window.windowID };
			m_CallbackFunction(windowFocusGainedEvent);
			return true;
		}
		case SDL_EventType::SDL_EVENT_WINDOW_FOCUS_LOST:
		{
			WindowFocusLostEvent windowFocusLostEvent{ event.window.windowID };
			m_CallbackFunction(windowFocusLostEvent);
			return true;
		}
		case SDL_EventType::SDL_EVENT_WINDOW_CLOSE_REQUESTED:
		{
			// not supported
			// NOTE: this might be important to look
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
			WindowFullscreenChangedEvent windowFullscreenEvent{ event.window.windowID, true };
			m_CallbackFunction(windowFullscreenEvent);
			return true;
		}
		case SDL_EventType::SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
		{
			WindowFullscreenChangedEvent windowFullscreenEvent{ event.window.windowID, false };
			m_CallbackFunction(windowFullscreenEvent);
			return true;
		}
		case SDL_EventType::SDL_EVENT_WINDOW_DESTROYED:
		{
			// not supported
		}
		break;
		}

		return false;
	}

	bool WindowsWindow::TryProcessKeyboardEvent(SDL_Event& event)
	{
		switch (event.type)
		{
		case SDL_EVENT_KEY_DOWN:
		{
			KeyPressedEvent keyPressedEvent{ event.window.windowID, (int32_t)event.key.key, event.key.repeat != 0 };
			m_CallbackFunction(keyPressedEvent);
			return true;
		}
		case SDL_EVENT_KEY_UP:
		{
			KeyReleasedEvent keyReleasedEvent{ event.window.windowID, (int32_t)event.key.key };
			m_CallbackFunction(keyReleasedEvent);
			return true;
		}
		case SDL_EVENT_TEXT_EDITING:
		{
			KeyTypedEvent keyTypedEvent{ event.window.windowID, (int32_t)event.key.key, event.text.text };
			m_CallbackFunction(keyTypedEvent);
			return true;
		}
		case SDL_EVENT_TEXT_INPUT:
		{
			KeyTypedEvent keyTypedEvent{ event.window.windowID, (int32_t)event.key.key, event.text.text };
			m_CallbackFunction(keyTypedEvent);
			return true;
		}
		case SDL_EVENT_KEYMAP_CHANGED:
		{
			// not supported
		}
		break;
		}

		return false;
	}

	bool WindowsWindow::TryProcessMouseEvent(SDL_Event& event)
	{
		switch (event.type)
		{
		case SDL_EVENT_MOUSE_MOTION:
		{
			MouseMovedEvent mouseMovedEvent{ event.window.windowID, event.motion.x, event.motion.y };
			m_CallbackFunction(mouseMovedEvent);
			return true;
		}
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
		{
			MouseButtonPressedEvent mouseButtonPressedEvent{ event.window.windowID, event.button.button };
			m_CallbackFunction(mouseButtonPressedEvent);
			return true;
		}
		case SDL_EVENT_MOUSE_BUTTON_UP:
		{
			MouseButtonReleasedEvent mouseButtonReleasedEvent{ event.window.windowID, event.button.button };
			m_CallbackFunction(mouseButtonReleasedEvent);
			return true;
		}
		case SDL_EVENT_MOUSE_WHEEL:
		{
			MouseScrolledEvent mouseScrolledEvent{ event.window.windowID, event.wheel.x, event.wheel.y };
			m_CallbackFunction(mouseScrolledEvent);
			return true;
		}
		}

		return false;
	}
}
