#include "spxpch.h"
#include "Input.h"

#ifdef SPX_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsInput.h"

Sphynx::Input* Sphynx::Input::s_Instance = new Sphynx::WindowsInput();

#endif // SPX_PLATFORM_WINDOWS

namespace Sphynx
{
	void Input::Init()
	{
		s_Instance->InitImpl();
	}

	void Input::Update()
	{
		s_Instance->UpdateImpl();
	}

	void Input::Shutdown()
	{
		s_Instance->ShutdownImpl();
	}

	bool Input::IsKeyPressed(int32_t keycode)
	{
		return s_Instance->IsKeyPressedImpl(keycode);
	}

	bool Input::IsKeyReleased(int32_t keycode)
	{
		return s_Instance->IsKeyReleasedImpl(keycode);
	}

	bool Input::IsKeyDown(int32_t keycode)
	{
		return s_Instance->IsKeyDownImpl(keycode);
	}

	bool Input::IsKeyUp(int32_t keycode)
	{
		return s_Instance->IsKeyUpImpl(keycode);
	}

	bool Input::IsMouseButtonPressed(int32_t button)
	{
		return s_Instance->IsMouseButtonPressedImpl(button);
	}

	bool Input::IsMouseButtonReleased(int32_t button)
	{
		return s_Instance->IsMouseButtonReleasedImpl(button);
	}

	bool Input::IsMouseButtonDown(int32_t button)
	{
		return s_Instance->IsMouseButtonDownImpl(button);
	}

	bool Input::IsMouseButtonUp(int32_t button)
	{
		return s_Instance->IsMouseButtonUpImpl(button);
	}

	std::pair<int64_t, int64_t> Input::GetMousePosition()
	{
		return s_Instance->GetMousePositionImpl();
	}

	int64_t Input::GetMousePositionX()
	{
		return s_Instance->GetMousePositionXImpl();
	}

	int64_t Input::GetMousePositionY()
	{
		return s_Instance->GetMousePositionYImpl();
	}
}