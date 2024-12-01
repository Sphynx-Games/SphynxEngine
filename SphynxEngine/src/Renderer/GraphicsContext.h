#pragma once

#include "Core/Core.h"


namespace Sphynx
{
	class Window;

	class SPHYNX_API GraphicsContext
	{
	public:
		GraphicsContext(Window* window) : m_Window(window) {}
		virtual ~GraphicsContext() = default;

		virtual void Init() = 0;
		virtual void Shutdown() = 0;

		virtual void OnBeforeWindowCreation() {};
		virtual void OnAfterWindowCreation() {};

		virtual void SwapBuffers() = 0;

		static GraphicsContext* Create(Window* window);

	protected:
		Window* m_Window;

	};
}
