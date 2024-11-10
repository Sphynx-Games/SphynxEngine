#pragma once

#include "EditorLayer.h"


struct SDL_Window;
struct SDL_Renderer;

namespace Sphynx
{
	class SDLEditorLayer : public EditorLayer
	{
	public:
		SDLEditorLayer();

		virtual void Attach() override;
		virtual void Detach() override;
		virtual void HandleEvent(Event& event) override;

		virtual void Begin() override;
		virtual void End() override;
		virtual void RenderGUI() override;

	private:
		bool OnWindowMouseEnter(WindowMouseEnterEvent& event);
		bool OnWindowMouseExit(WindowMouseExitEvent& event);
		bool OnWindowFocusGained(WindowFocusGainedEvent& event);
		bool OnWindowFocusLost(WindowFocusLostEvent& event);
		bool OnWindowClosed(WindowClosedEvent& event);
		bool OnWindowResized(WindowResizedEvent& event);
		bool OnWindowMoved(WindowMovedEvent& event);
		bool OnMouseMoved(MouseMovedEvent& event);
		bool OnMouseScrolled(MouseScrolledEvent& event);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& event);
		bool OnMouseButtonReleased(MouseButtonReleasedEvent& event);
		bool OnKeyPressed(KeyPressedEvent& event);
		bool OnKeyReleased(KeyReleasedEvent& event);
		bool OnKeyTyped(KeyTypedEvent& event);

	private:
		SDL_Window* m_Window;
		SDL_Renderer* m_Renderer;

	};
}
