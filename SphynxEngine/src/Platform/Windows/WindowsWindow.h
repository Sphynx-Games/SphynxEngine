#pragma once

#include "Renderer/Window.h"


union SDL_Event;
struct SDL_Window;
struct SDL_Surface;

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

		void ProcessEvent(SDL_Event& event);
		bool TryProcessWindowEvent(SDL_Event& event);
		bool TryProcessKeyboardEvent(SDL_Event& event);
		bool TryProcessMouseEvent(SDL_Event& event);

	private:
		WindowParams m_Params;
		EventCallbackFunction m_CallbackFunction;

		SDL_Window* m_Window;
	};
}

