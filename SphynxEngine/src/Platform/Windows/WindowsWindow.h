#pragma once
#include "Renderer/Window.h"
#include <memory>
#include <SDL.h>

namespace Sphynx
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowParams& params);
		virtual ~WindowsWindow() override;

		virtual void Update() override;

		virtual uint32_t GetWidth() const override;
		virtual uint32_t GetHeight() const override;

		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() const override;

		virtual void* GetNativeWindow() const override;

		virtual void SetEventCallbackFunction(EventCallbackFunction function) override;

	private:
		void Init(const WindowParams& params);
		void Shutdown();

	private:
		WindowParams m_Params;
		EventCallbackFunction m_CallbackFunction;

		SDL_Window* m_Window;
		SDL_Surface* m_Surface;
	};
}

