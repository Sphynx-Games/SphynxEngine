#include "spxpch.h"
#include "WindowsInput.h"
#include <SDL3/SDL.h>


namespace Sphynx
{
	void WindowsInput::InitImpl()
	{
		m_KeyCount = 0;
		m_CurrKeys = SDL_GetKeyboardState(&m_KeyCount);

		m_PrevKeysCache.resize(m_KeyCount);
		m_PrevKeys = (bool*) m_PrevKeysCache.data();

		m_PrevMouseState = SDL_GetMouseState(nullptr, nullptr);
	}

	void WindowsInput::UpdateImpl()
	{
		// save previous state
		memcpy((void*)m_PrevKeys, m_CurrKeys, sizeof(uint8_t) * m_KeyCount);
		m_PrevMouseState = SDL_GetMouseState(nullptr, nullptr);
	}

	void WindowsInput::ShutdownImpl()
	{
		m_KeyCount = 0;
		m_CurrKeys = nullptr;

		m_PrevKeysCache.clear();
		m_PrevKeys = nullptr;
	}

	bool WindowsInput::IsKeyPressedImpl(int32_t keycode)
	{
		SDL_Scancode scancode = SDL_GetScancodeFromKey(keycode, nullptr);
		return m_CurrKeys[scancode] && !m_PrevKeys[scancode];
	}

	bool WindowsInput::IsKeyReleasedImpl(int32_t keycode)
	{
		SDL_Scancode scancode = SDL_GetScancodeFromKey(keycode, nullptr);
		return !m_CurrKeys[scancode] && m_PrevKeys[scancode];
	}

	bool WindowsInput::IsKeyDownImpl(int32_t keycode)
	{
		SDL_Scancode scancode = SDL_GetScancodeFromKey(keycode, nullptr);
		if (scancode < 0 || scancode >= m_KeyCount) return false;

		return m_CurrKeys[scancode] != 0;
	}

	bool WindowsInput::IsKeyUpImpl(int32_t keycode)
	{
		return !IsKeyDownImpl(keycode);
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int32_t button)
	{
		return !(m_PrevMouseState & SDL_BUTTON(button)) && IsMouseButtonDownImpl(button);
	}

	bool WindowsInput::IsMouseButtonReleasedImpl(int32_t button)
	{
		return (m_PrevMouseState & SDL_BUTTON(button)) && !IsMouseButtonDownImpl(button);
	}

	bool WindowsInput::IsMouseButtonDownImpl(int32_t button)
	{
		uint32_t state = SDL_GetMouseState(nullptr, nullptr);
		return state & SDL_BUTTON(button);
	}

	bool WindowsInput::IsMouseButtonUpImpl(int32_t button)
	{
		return !IsMouseButtonDownImpl(button);
	}

	std::pair<int64_t, int64_t> WindowsInput::GetMousePositionImpl()
	{
		float x, y;
		SDL_GetMouseState(&x, &y);

		return { (int64_t)x, (int64_t)y };
	}

	int64_t WindowsInput::GetMousePositionXImpl()
	{
		float x;
		SDL_GetMouseState(&x, nullptr);

		return (int64_t)x;
	}

	int64_t WindowsInput::GetMousePositionYImpl()
	{
		float y;
		SDL_GetMouseState(nullptr, &y);

		return (int64_t)y;
	}
}