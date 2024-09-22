#pragma once

#include "Input/Input.h"


namespace Sphynx
{
	class WindowsInput : public Input
	{
	private:
		virtual void InitImpl() override;
		virtual void UpdateImpl() override;
		virtual void ShutdownImpl() override;

		virtual bool IsKeyPressedImpl(int32_t keycode) override;
		virtual bool IsKeyReleasedImpl(int32_t keycode) override;
		virtual bool IsKeyDownImpl(int32_t keycode) override;
		virtual bool IsKeyUpImpl(int32_t keycode) override;

		virtual bool IsMouseButtonPressedImpl(int32_t button) override;
		virtual bool IsMouseButtonReleasedImpl(int32_t button) override;
		virtual bool IsMouseButtonDownImpl(int32_t button) override;
		virtual bool IsMouseButtonUpImpl(int32_t button) override;
		std::pair<int64_t, int64_t> GetMousePositionImpl() override;
		int64_t GetMousePositionXImpl() override;
		int64_t GetMousePositionYImpl() override;

	private:
		const bool* m_PrevKeys;
		const bool* m_CurrKeys;
		int32_t m_KeyCount;
		std::vector<uint8_t> m_PrevKeysCache;

		uint32_t m_PrevMouseState;
	};
}