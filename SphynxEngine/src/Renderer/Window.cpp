#include "spxpch.h"
#include "Window.h"

#ifdef SPX_PLATFORM_WINDOWS

#include "Platform/Windows/WindowsWindow.h"

namespace Sphynx
{
	Window* Window::Create(const WindowParams& params)
	{
		return new WindowsWindow(params);
	}
}

#endif