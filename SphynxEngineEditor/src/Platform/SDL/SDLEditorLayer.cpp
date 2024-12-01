#include "spxpch.h"
#include "SDLEditorLayer.h"

#include <Platform/SDL/SDLRendererAPI.h>

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include <stdio.h>
#include <SDL3/SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL3/SDL_opengles2.h>
#else
#include <SDL3/SDL_opengl.h>
#include "SDLEditorLayer.h"
#endif


//NOTE: CHANGE EVERYTIME IMGUI IMPL CHANGES
// Copied from imgui_impl_sdl3.cpp
struct ImGui_ImplSDL3_Data
{
	SDL_Window* Window;
	SDL_WindowID            WindowID;
	SDL_Renderer* Renderer;
	Uint64                  Time;
	char* ClipboardTextData;
	bool                    UseVulkan;
	bool                    WantUpdateMonitors;

	// IME handling
	SDL_Window* ImeWindow;

	// Mouse handling
	Uint32                  MouseWindowID;
	int                     MouseButtonsDown;
	SDL_Cursor* MouseCursors[ImGuiMouseCursor_COUNT];
	SDL_Cursor* MouseLastCursor;
	int                     MousePendingLeaveFrame;
	bool                    MouseCanUseGlobalState;
	bool                    MouseCanReportHoveredViewport;  // This is hard to use/unreliable on SDL so we'll set ImGuiBackendFlags_HasMouseHoveredViewport dynamically based on state.

	// Gamepad handling
	ImVector<SDL_Gamepad*>  Gamepads;
	ImGui_ImplSDL3_GamepadMode  GamepadMode;
	bool                    WantUpdateGamepadsList;

	ImGui_ImplSDL3_Data() { memset((void*)this, 0, sizeof(*this)); }
};

static ImGuiKey ImGui_ImplSDL3_KeycodeToImGuiKey(int keycode)
{
	switch (keycode)
	{
	case SDLK_TAB: return ImGuiKey_Tab;
	case SDLK_LEFT: return ImGuiKey_LeftArrow;
	case SDLK_RIGHT: return ImGuiKey_RightArrow;
	case SDLK_UP: return ImGuiKey_UpArrow;
	case SDLK_DOWN: return ImGuiKey_DownArrow;
	case SDLK_PAGEUP: return ImGuiKey_PageUp;
	case SDLK_PAGEDOWN: return ImGuiKey_PageDown;
	case SDLK_HOME: return ImGuiKey_Home;
	case SDLK_END: return ImGuiKey_End;
	case SDLK_INSERT: return ImGuiKey_Insert;
	case SDLK_DELETE: return ImGuiKey_Delete;
	case SDLK_BACKSPACE: return ImGuiKey_Backspace;
	case SDLK_SPACE: return ImGuiKey_Space;
	case SDLK_RETURN: return ImGuiKey_Enter;
	case SDLK_ESCAPE: return ImGuiKey_Escape;
	case SDLK_APOSTROPHE: return ImGuiKey_Apostrophe;
	case SDLK_COMMA: return ImGuiKey_Comma;
	case SDLK_MINUS: return ImGuiKey_Minus;
	case SDLK_PERIOD: return ImGuiKey_Period;
	case SDLK_SLASH: return ImGuiKey_Slash;
	case SDLK_SEMICOLON: return ImGuiKey_Semicolon;
	case SDLK_EQUALS: return ImGuiKey_Equal;
	case SDLK_LEFTBRACKET: return ImGuiKey_LeftBracket;
	case SDLK_BACKSLASH: return ImGuiKey_Backslash;
	case SDLK_RIGHTBRACKET: return ImGuiKey_RightBracket;
	case SDLK_GRAVE: return ImGuiKey_GraveAccent;
	case SDLK_CAPSLOCK: return ImGuiKey_CapsLock;
	case SDLK_SCROLLLOCK: return ImGuiKey_ScrollLock;
	case SDLK_NUMLOCKCLEAR: return ImGuiKey_NumLock;
	case SDLK_PRINTSCREEN: return ImGuiKey_PrintScreen;
	case SDLK_PAUSE: return ImGuiKey_Pause;
	case SDLK_KP_0: return ImGuiKey_Keypad0;
	case SDLK_KP_1: return ImGuiKey_Keypad1;
	case SDLK_KP_2: return ImGuiKey_Keypad2;
	case SDLK_KP_3: return ImGuiKey_Keypad3;
	case SDLK_KP_4: return ImGuiKey_Keypad4;
	case SDLK_KP_5: return ImGuiKey_Keypad5;
	case SDLK_KP_6: return ImGuiKey_Keypad6;
	case SDLK_KP_7: return ImGuiKey_Keypad7;
	case SDLK_KP_8: return ImGuiKey_Keypad8;
	case SDLK_KP_9: return ImGuiKey_Keypad9;
	case SDLK_KP_PERIOD: return ImGuiKey_KeypadDecimal;
	case SDLK_KP_DIVIDE: return ImGuiKey_KeypadDivide;
	case SDLK_KP_MULTIPLY: return ImGuiKey_KeypadMultiply;
	case SDLK_KP_MINUS: return ImGuiKey_KeypadSubtract;
	case SDLK_KP_PLUS: return ImGuiKey_KeypadAdd;
	case SDLK_KP_ENTER: return ImGuiKey_KeypadEnter;
	case SDLK_KP_EQUALS: return ImGuiKey_KeypadEqual;
	case SDLK_LCTRL: return ImGuiKey_LeftCtrl;
	case SDLK_LSHIFT: return ImGuiKey_LeftShift;
	case SDLK_LALT: return ImGuiKey_LeftAlt;
	case SDLK_LGUI: return ImGuiKey_LeftSuper;
	case SDLK_RCTRL: return ImGuiKey_RightCtrl;
	case SDLK_RSHIFT: return ImGuiKey_RightShift;
	case SDLK_RALT: return ImGuiKey_RightAlt;
	case SDLK_RGUI: return ImGuiKey_RightSuper;
	case SDLK_APPLICATION: return ImGuiKey_Menu;
	case SDLK_0: return ImGuiKey_0;
	case SDLK_1: return ImGuiKey_1;
	case SDLK_2: return ImGuiKey_2;
	case SDLK_3: return ImGuiKey_3;
	case SDLK_4: return ImGuiKey_4;
	case SDLK_5: return ImGuiKey_5;
	case SDLK_6: return ImGuiKey_6;
	case SDLK_7: return ImGuiKey_7;
	case SDLK_8: return ImGuiKey_8;
	case SDLK_9: return ImGuiKey_9;
	case SDLK_A: return ImGuiKey_A;
	case SDLK_B: return ImGuiKey_B;
	case SDLK_C: return ImGuiKey_C;
	case SDLK_D: return ImGuiKey_D;
	case SDLK_E: return ImGuiKey_E;
	case SDLK_F: return ImGuiKey_F;
	case SDLK_G: return ImGuiKey_G;
	case SDLK_H: return ImGuiKey_H;
	case SDLK_I: return ImGuiKey_I;
	case SDLK_J: return ImGuiKey_J;
	case SDLK_K: return ImGuiKey_K;
	case SDLK_L: return ImGuiKey_L;
	case SDLK_M: return ImGuiKey_M;
	case SDLK_N: return ImGuiKey_N;
	case SDLK_O: return ImGuiKey_O;
	case SDLK_P: return ImGuiKey_P;
	case SDLK_Q: return ImGuiKey_Q;
	case SDLK_R: return ImGuiKey_R;
	case SDLK_S: return ImGuiKey_S;
	case SDLK_T: return ImGuiKey_T;
	case SDLK_U: return ImGuiKey_U;
	case SDLK_V: return ImGuiKey_V;
	case SDLK_W: return ImGuiKey_W;
	case SDLK_X: return ImGuiKey_X;
	case SDLK_Y: return ImGuiKey_Y;
	case SDLK_Z: return ImGuiKey_Z;
	case SDLK_F1: return ImGuiKey_F1;
	case SDLK_F2: return ImGuiKey_F2;
	case SDLK_F3: return ImGuiKey_F3;
	case SDLK_F4: return ImGuiKey_F4;
	case SDLK_F5: return ImGuiKey_F5;
	case SDLK_F6: return ImGuiKey_F6;
	case SDLK_F7: return ImGuiKey_F7;
	case SDLK_F8: return ImGuiKey_F8;
	case SDLK_F9: return ImGuiKey_F9;
	case SDLK_F10: return ImGuiKey_F10;
	case SDLK_F11: return ImGuiKey_F11;
	case SDLK_F12: return ImGuiKey_F12;
	case SDLK_F13: return ImGuiKey_F13;
	case SDLK_F14: return ImGuiKey_F14;
	case SDLK_F15: return ImGuiKey_F15;
	case SDLK_F16: return ImGuiKey_F16;
	case SDLK_F17: return ImGuiKey_F17;
	case SDLK_F18: return ImGuiKey_F18;
	case SDLK_F19: return ImGuiKey_F19;
	case SDLK_F20: return ImGuiKey_F20;
	case SDLK_F21: return ImGuiKey_F21;
	case SDLK_F22: return ImGuiKey_F22;
	case SDLK_F23: return ImGuiKey_F23;
	case SDLK_F24: return ImGuiKey_F24;
	case SDLK_AC_BACK: return ImGuiKey_AppBack;
	case SDLK_AC_FORWARD: return ImGuiKey_AppForward;
	}
	return ImGuiKey_None;
}

namespace Sphynx
{
	void SDLEditorLayer::Attach()
	{
		// Enable native IME.
		SDL_SetHint(SDL_HINT_IME_IMPLEMENTED_UI, "1");

		// Setup Platform/Renderer backends
		Sphynx::Application* application = Sphynx::Application::GetInstance();
		m_Window = static_cast<SDL_Window*>(application->GetWindow()->GetNativeWindow());
		m_Renderer = static_cast<const Sphynx::SDLRendererAPI*>(Sphynx::Renderer2D::GetRendererAPI())->GetSDLRenderer();
		ImGui_ImplSDL3_InitForSDLRenderer(m_Window, m_Renderer);
		ImGui_ImplSDLRenderer3_Init(m_Renderer);
	}

	void SDLEditorLayer::Detach()
	{
		// Cleanup
		ImGui_ImplSDLRenderer3_Shutdown();
		ImGui_ImplSDL3_Shutdown();
	}

	void SDLEditorLayer::HandleEvent(Event& event)
	{
		//ImGui_ImplSDL3_ProcessEvent(&event);
		Sphynx::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowMouseEnterEvent>([](auto& e) { return OnWindowMouseEnter(e); });
		dispatcher.Dispatch<WindowMouseExitEvent>([](auto& e) { return OnWindowMouseExit(e); });
		dispatcher.Dispatch<WindowFocusGainedEvent>([](auto& e) { return OnWindowFocusGained(e); });
		dispatcher.Dispatch<WindowFocusLostEvent>([](auto& e) { return OnWindowFocusLost(e); });
		dispatcher.Dispatch<WindowClosedEvent>([](auto& e) { return OnWindowClosed(e); });
		dispatcher.Dispatch<WindowResizedEvent>([](auto& e) { return OnWindowResized(e); });
		dispatcher.Dispatch<WindowMovedEvent>([](auto& e) { return OnWindowMoved(e); });
		dispatcher.Dispatch<MouseMovedEvent>([](auto& e) { return OnMouseMoved(e); });
		dispatcher.Dispatch<MouseScrolledEvent>([](auto& e) { return OnMouseScrolled(e); });
		dispatcher.Dispatch<MouseButtonPressedEvent>([](auto& e) { return OnMouseButtonPressed(e); });
		dispatcher.Dispatch<MouseButtonReleasedEvent>([](auto& e) { return OnMouseButtonReleased(e); });
		dispatcher.Dispatch<KeyPressedEvent>([](auto& e) { return OnKeyPressed(e); });
		dispatcher.Dispatch<KeyReleasedEvent>([](auto& e) { return OnKeyReleased(e); });
		dispatcher.Dispatch<KeyTypedEvent>([](auto& e) { return OnKeyTyped(e); });
	}

	void SDLEditorLayer::Begin()
	{
		// Start the Dear ImGui frame
		ImGui_ImplSDLRenderer3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
	}

	void SDLEditorLayer::End()
	{
		// Rendering
		//SDL_RenderSetScale(m_Renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
		//SDL_SetRenderDrawColor(m_Renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
		SDL_RenderClear(m_Renderer);

		ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_Renderer);
		SDL_RenderPresent(m_Renderer);
	}

	void SDLEditorLayer::RenderGUI()
	{

	}

	bool SDLEditorLayer::OnWindowMouseEnter(Sphynx::WindowMouseEnterEvent& event)
	{
		if (ImGui::FindViewportByPlatformHandle((void*)(uintptr_t)event.GetWindowID()) == nullptr)
		{
			return false;
		}

		ImGui_ImplSDL3_Data* bd = ImGui::GetCurrentContext() ? (ImGui_ImplSDL3_Data*)ImGui::GetIO().BackendPlatformUserData : nullptr;
		SPX_CORE_ASSERT(bd != nullptr);

		bd->MouseWindowID = event.GetWindowID();
		bd->MousePendingLeaveFrame = 0;
		return true;
	}

	bool SDLEditorLayer::OnWindowMouseExit(Sphynx::WindowMouseExitEvent& event)
	{
		if (ImGui::FindViewportByPlatformHandle((void*)(uintptr_t)event.GetWindowID()) == nullptr)
		{
			return false;
		}

		ImGui_ImplSDL3_Data* bd = ImGui::GetCurrentContext() ? (ImGui_ImplSDL3_Data*)ImGui::GetIO().BackendPlatformUserData : nullptr;
		SPX_CORE_ASSERT(bd != nullptr);

		bd->MousePendingLeaveFrame = ImGui::GetFrameCount() + 1;
		return true;
	}

	bool SDLEditorLayer::OnWindowFocusGained(Sphynx::WindowFocusGainedEvent& event)
	{
		if (ImGui::FindViewportByPlatformHandle((void*)(uintptr_t)event.GetWindowID()) == nullptr)
		{
			return false;
		}
		ImGuiIO& io = ImGui::GetIO();

		io.AddFocusEvent(true);
		return true;
	}

	bool SDLEditorLayer::OnWindowFocusLost(Sphynx::WindowFocusLostEvent& event)
	{
		if (ImGui::FindViewportByPlatformHandle((void*)(uintptr_t)event.GetWindowID()) == nullptr)
		{
			return false;
		}
		ImGuiIO& io = ImGui::GetIO();

		io.AddFocusEvent(true);
		return true;
	}

	bool SDLEditorLayer::OnWindowClosed(Sphynx::WindowClosedEvent& event)
	{
		ImGui_ImplSDL3_Data* bd = ImGui::GetCurrentContext() ? (ImGui_ImplSDL3_Data*)ImGui::GetIO().BackendPlatformUserData : nullptr;
		SPX_CORE_ASSERT(bd != nullptr);

		if (ImGuiViewport* viewport = ImGui::FindViewportByPlatformHandle((void*)(uintptr_t)event.GetWindowID()))
		{
			viewport->PlatformRequestClose = true;
			return true;
		}
		return false;
	}

	bool SDLEditorLayer::OnWindowResized(Sphynx::WindowResizedEvent& event)
	{
		ImGui_ImplSDL3_Data* bd = ImGui::GetCurrentContext() ? (ImGui_ImplSDL3_Data*)ImGui::GetIO().BackendPlatformUserData : nullptr;
		SPX_CORE_ASSERT(bd != nullptr);

		if (ImGuiViewport* viewport = ImGui::FindViewportByPlatformHandle((void*)(uintptr_t)event.GetWindowID()))
		{
			viewport->PlatformRequestResize = true;
			return true;
		}
		return false;
	}

	bool SDLEditorLayer::OnWindowMoved(Sphynx::WindowMovedEvent& event)
	{
		ImGui_ImplSDL3_Data* bd = ImGui::GetCurrentContext() ? (ImGui_ImplSDL3_Data*)ImGui::GetIO().BackendPlatformUserData : nullptr;
		SPX_CORE_ASSERT(bd != nullptr);

		if (ImGuiViewport* viewport = ImGui::FindViewportByPlatformHandle((void*)(uintptr_t)event.GetWindowID()))
		{
			viewport->PlatformRequestMove = true;
			return true;
		}
		return false;
	}

	bool SDLEditorLayer::OnMouseMoved(Sphynx::MouseMovedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();

		if (ImGui::FindViewportByPlatformHandle((void*)(uintptr_t)event.GetWindowID()) == nullptr)
		{
			return false;
		}

		ImVec2 mouse_pos(event.GetX(), event.GetY());
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			int window_x, window_y;
			SDL_GetWindowPosition(SDL_GetWindowFromID(event.GetWindowID()), &window_x, &window_y);
			mouse_pos.x += window_x;
			mouse_pos.y += window_y;
		}
		io.AddMouseSourceEvent(ImGuiMouseSource_Mouse);
		io.AddMousePosEvent(mouse_pos.x, mouse_pos.y);

		return false;
	}

	bool SDLEditorLayer::OnMouseScrolled(Sphynx::MouseScrolledEvent& event)
	{
		if (ImGui::FindViewportByPlatformHandle((void*)(uintptr_t)event.GetWindowID()) == nullptr)
		{
			return false;
		}

		ImGuiIO& io = ImGui::GetIO();
		float wheel_x = -event.GetX();
		float wheel_y = event.GetY();
#ifdef __EMSCRIPTEN__
		wheel_x /= 100.0f;
#endif
		io.AddMouseSourceEvent(ImGuiMouseSource_Mouse);
		io.AddMouseWheelEvent(wheel_x, wheel_y);
		return true;
	}

	bool SDLEditorLayer::OnMouseButtonPressed(Sphynx::MouseButtonPressedEvent& event)
	{
		if (ImGui::FindViewportByPlatformHandle((void*)(uintptr_t)event.GetWindowID()) == nullptr)
		{
			return false;
		}

		ImGui_ImplSDL3_Data* bd = ImGui::GetCurrentContext() ? (ImGui_ImplSDL3_Data*)ImGui::GetIO().BackendPlatformUserData : nullptr;
		SPX_CORE_ASSERT(bd != nullptr);

		ImGuiIO& io = ImGui::GetIO();

		int32_t button = -1;
		if (event.GetButton() == SPX_BUTTON_LEFT) { button = 0; }
		if (event.GetButton() == SPX_BUTTON_RIGHT) { button = 1; }
		if (event.GetButton() == SPX_BUTTON_MIDDLE) { button = 2; }
		if (event.GetButton() == SPX_BUTTON_X1) { button = 3; }
		if (event.GetButton() == SPX_BUTTON_X2) { button = 4; }
		if (button == -1)
		{
			return false;
		}

		io.AddMouseSourceEvent(ImGuiMouseSource_Mouse);
		io.AddMouseButtonEvent(button, true);
		bd->MouseButtonsDown = (bd->MouseButtonsDown | (1 << button));
		return true;
	}

	bool SDLEditorLayer::OnMouseButtonReleased(Sphynx::MouseButtonReleasedEvent& event)
	{
		if (ImGui::FindViewportByPlatformHandle((void*)(uintptr_t)event.GetWindowID()) == nullptr)
		{
			return false;
		}

		ImGui_ImplSDL3_Data* bd = ImGui::GetCurrentContext() ? (ImGui_ImplSDL3_Data*)ImGui::GetIO().BackendPlatformUserData : nullptr;
		SPX_CORE_ASSERT(bd != nullptr);

		ImGuiIO& io = ImGui::GetIO();

		int32_t button = -1;
		if (event.GetButton() == SPX_BUTTON_LEFT) { button = 0; }
		if (event.GetButton() == SPX_BUTTON_RIGHT) { button = 1; }
		if (event.GetButton() == SPX_BUTTON_MIDDLE) { button = 2; }
		if (event.GetButton() == SPX_BUTTON_X1) { button = 3; }
		if (event.GetButton() == SPX_BUTTON_X2) { button = 4; }
		if (button == -1)
		{
			return false;
		}

		io.AddMouseSourceEvent(ImGuiMouseSource_Mouse);
		io.AddMouseButtonEvent(button, false);
		bd->MouseButtonsDown = (bd->MouseButtonsDown & ~(1 << button));
		return true;
	}

	bool SDLEditorLayer::OnKeyPressed(Sphynx::KeyPressedEvent& event)
	{
		if (ImGui::FindViewportByPlatformHandle((void*)(uintptr_t)event.GetWindowID()) == nullptr)
		{
			return false;
		}

		uint16_t modMask = SDL_KMOD_NONE;
		if (Sphynx::Input::IsKeyDown(Sphynx::SPX_KEY_LCTRL) || Sphynx::Input::IsKeyDown(Sphynx::SPX_KEY_RCTRL))
		{
			modMask |= SDL_KMOD_CTRL;
		}
		else if (Sphynx::Input::IsKeyDown(Sphynx::SPX_KEY_LSHIFT) || Sphynx::Input::IsKeyDown(Sphynx::SPX_KEY_RSHIFT))
		{
			modMask |= SDL_KMOD_SHIFT;
		}
		else if (Sphynx::Input::IsKeyDown(Sphynx::SPX_KEY_LALT) || Sphynx::Input::IsKeyDown(Sphynx::SPX_KEY_RALT))
		{
			modMask |= SDL_KMOD_ALT;
		}
		else if (Sphynx::Input::IsKeyDown(Sphynx::SPX_KEY_LGUI) || Sphynx::Input::IsKeyDown(Sphynx::SPX_KEY_RGUI))
		{
			modMask |= SDL_KMOD_GUI;
		}

		ImGuiIO& io = ImGui::GetIO();

		io.AddKeyEvent(ImGuiMod_Ctrl, (modMask & SDL_KMOD_CTRL) != 0);
		io.AddKeyEvent(ImGuiMod_Shift, (modMask & SDL_KMOD_SHIFT) != 0);
		io.AddKeyEvent(ImGuiMod_Alt, (modMask & SDL_KMOD_ALT) != 0);
		io.AddKeyEvent(ImGuiMod_Super, (modMask & SDL_KMOD_GUI) != 0);

		ImGuiKey key = ImGui_ImplSDL3_KeycodeToImGuiKey(event.GetKeycode());
		io.AddKeyEvent(key, true);
		io.SetKeyEventNativeData(
			key,
			event.GetKeycode(),
			SDL_GetScancodeFromKey(event.GetKeycode(), nullptr),
			SDL_GetScancodeFromKey(event.GetKeycode(), nullptr)
		); // To support legacy indexing (<1.87 user code). Legacy backend uses SDLK_*** as indices to IsKeyXXX() functions.
		return true;
	}

	bool SDLEditorLayer::OnKeyReleased(Sphynx::KeyReleasedEvent& event)
	{
		if (ImGui::FindViewportByPlatformHandle((void*)(uintptr_t)event.GetWindowID()) == nullptr)
		{
			return false;
		}

		uint16_t modMask = SDL_KMOD_NONE;
		if (Sphynx::Input::IsKeyDown(Sphynx::SPX_KEY_LCTRL) || Sphynx::Input::IsKeyDown(Sphynx::SPX_KEY_RCTRL))
		{
			modMask |= SDL_KMOD_CTRL;
		}
		else if (Sphynx::Input::IsKeyDown(Sphynx::SPX_KEY_LSHIFT) || Sphynx::Input::IsKeyDown(Sphynx::SPX_KEY_RSHIFT))
		{
			modMask |= SDL_KMOD_SHIFT;
		}
		else if (Sphynx::Input::IsKeyDown(Sphynx::SPX_KEY_LALT) || Sphynx::Input::IsKeyDown(Sphynx::SPX_KEY_RALT))
		{
			modMask |= SDL_KMOD_ALT;
		}
		else if (Sphynx::Input::IsKeyDown(Sphynx::SPX_KEY_LGUI) || Sphynx::Input::IsKeyDown(Sphynx::SPX_KEY_RGUI))
		{
			modMask |= SDL_KMOD_GUI;
		}

		ImGuiIO& io = ImGui::GetIO();

		io.AddKeyEvent(ImGuiMod_Ctrl, (modMask & SDL_KMOD_CTRL) != 0);
		io.AddKeyEvent(ImGuiMod_Shift, (modMask & SDL_KMOD_SHIFT) != 0);
		io.AddKeyEvent(ImGuiMod_Alt, (modMask & SDL_KMOD_ALT) != 0);
		io.AddKeyEvent(ImGuiMod_Super, (modMask & SDL_KMOD_GUI) != 0);

		ImGuiKey key = ImGui_ImplSDL3_KeycodeToImGuiKey(event.GetKeycode());
		io.AddKeyEvent(key, false);
		io.SetKeyEventNativeData(
			key,
			event.GetKeycode(),
			SDL_GetScancodeFromKey(event.GetKeycode(), nullptr),
			SDL_GetScancodeFromKey(event.GetKeycode(), nullptr)
		); // To support legacy indexing (<1.87 user code). Legacy backend uses SDLK_*** as indices to IsKeyXXX() functions.
		return true;
	}

	bool SDLEditorLayer::OnKeyTyped(Sphynx::KeyTypedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		if (ImGui::FindViewportByPlatformHandle((void*)(uintptr_t)event.GetWindowID()) == nullptr)
		{
			return false;
		}

		io.AddInputCharactersUTF8(event.GetText().c_str());
		return true;
	}
}
