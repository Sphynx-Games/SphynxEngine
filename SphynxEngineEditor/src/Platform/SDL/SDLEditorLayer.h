#pragma once

#include "EditorLayer.h"


struct SDL_Window;
struct SDL_Renderer;

namespace Sphynx
{
	class SDLEditorLayer
	{
	public:
		static void Attach();
		static void Detach();
		static void HandleEvent(Event& event);

		static void Begin();
		static void End();
		static void RenderGUI();

	private:
		static bool OnWindowMouseEnter(WindowMouseEnterEvent& event);
		static bool OnWindowMouseExit(WindowMouseExitEvent& event);
		static bool OnWindowFocusGained(WindowFocusGainedEvent& event);
		static bool OnWindowFocusLost(WindowFocusLostEvent& event);
		static bool OnWindowClosed(WindowClosedEvent& event);
		static bool OnWindowResized(WindowResizedEvent& event);
		static bool OnWindowMoved(WindowMovedEvent& event);
		static bool OnMouseMoved(MouseMovedEvent& event);
		static bool OnMouseScrolled(MouseScrolledEvent& event);
		static bool OnMouseButtonPressed(MouseButtonPressedEvent& event);
		static bool OnMouseButtonReleased(MouseButtonReleasedEvent& event);
		static bool OnKeyPressed(KeyPressedEvent& event);
		static bool OnKeyReleased(KeyReleasedEvent& event);
		static bool OnKeyTyped(KeyTypedEvent& event);

	private:
		inline static SDL_Window* m_Window{ nullptr };
		inline static SDL_Renderer* m_Renderer{ nullptr };

	};
}
