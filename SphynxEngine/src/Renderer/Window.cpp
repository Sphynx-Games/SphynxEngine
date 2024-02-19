#include "Window.h"

#ifdef SPX_PLATFORM_WINDOWS

#include "Platform/Windows/WindowsWindow.h"

Sphynx::Window* Sphynx::Window::Create(const WindowParams& params)
{
	return new WindowsWindow(params);
}

#endif