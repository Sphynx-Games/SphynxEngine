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
		virtual void HandleEvent(Sphynx::Event& event) override;

		virtual void Begin() override;
		virtual void End() override;
		virtual void RenderGUI() override;

	private:
		bool OnWindowMouseEnter(Sphynx::WindowMouseEnterEvent& event);
		bool OnWindowMouseExit(Sphynx::WindowMouseExitEvent& event);
		bool OnWindowFocusGained(Sphynx::WindowFocusGainedEvent& event);
		bool OnWindowFocusLost(Sphynx::WindowFocusLostEvent& event);
		bool OnWindowClosed(Sphynx::WindowClosedEvent& event);
		bool OnWindowResized(Sphynx::WindowResizedEvent& event);
		bool OnWindowMoved(Sphynx::WindowMovedEvent& event);
		bool OnMouseMoved(Sphynx::MouseMovedEvent& event);
		bool OnMouseScrolled(Sphynx::MouseScrolledEvent& event);
		bool OnMouseButtonPressed(Sphynx::MouseButtonPressedEvent& event);
		bool OnMouseButtonReleased(Sphynx::MouseButtonReleasedEvent& event);
		bool OnKeyPressed(Sphynx::KeyPressedEvent& event);
		bool OnKeyReleased(Sphynx::KeyReleasedEvent& event);
		bool OnKeyTyped(Sphynx::KeyTypedEvent& event);

	private:
		SDL_Window* m_Window;
		SDL_Renderer* m_Renderer;

	};
}
