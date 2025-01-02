#pragma once

#include "EditorLayer.h"


struct SDL_Window;
struct SDL_Renderer;

namespace Sphynx
{
	class OpenGLEditorLayer
	{
	public:
		static void Attach();
		static void Detach();
		static void HandleEvent(Event& event);

		static void Begin();
		static void End();
		static void RenderGUI();

	private:
		static bool OnWindowMouseEnter(class WindowMouseEnterEvent& event);
		static bool OnWindowMouseExit(class WindowMouseExitEvent& event);
		static bool OnWindowFocusGained(class WindowFocusGainedEvent& event);
		static bool OnWindowFocusLost(class WindowFocusLostEvent& event);
		static bool OnWindowClosed(class WindowClosedEvent& event);
		static bool OnWindowResized(class WindowResizedEvent& event);
		static bool OnWindowMoved(class WindowMovedEvent& event);
		static bool OnMouseMoved(class MouseMovedEvent& event);
		static bool OnMouseScrolled(class MouseScrolledEvent& event);
		static bool OnMouseButtonPressed(class MouseButtonPressedEvent& event);
		static bool OnMouseButtonReleased(class MouseButtonReleasedEvent& event);
		static bool OnKeyPressed(class KeyPressedEvent& event);
		static bool OnKeyReleased(class KeyReleasedEvent& event);
		static bool OnKeyTyped(class KeyTypedEvent& event);

	private:
		inline static SDL_Window* m_Window{ nullptr };

	};
}
