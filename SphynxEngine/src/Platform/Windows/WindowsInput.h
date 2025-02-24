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
		virtual std::pair<int64_t, int64_t> GetMousePositionImpl() override;
		virtual int64_t GetMousePositionXImpl() override;
		virtual int64_t GetMousePositionYImpl() override;
		virtual bool GetMouseGrabImpl() override;
		virtual void SetMouseGrabImpl(bool grab) override;
		virtual void GetMouseGrabRectImpl(uint32_t& x, uint32_t& y, uint32_t& width, uint32_t& height) override;
		virtual void SetMouseGrabRectImpl(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual bool GetCursorVisibleImpl() override;
		virtual void SetCursorVisibleImpl(bool visible) override;

	private:
		const bool* m_PrevKeys;
		const bool* m_CurrKeys;
		int32_t m_KeyCount;
		std::vector<uint8_t> m_PrevKeysCache;

		uint32_t m_PrevMouseState;
	};
}