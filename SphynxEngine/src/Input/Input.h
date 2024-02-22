#pragma once

#include "Core/Core.h"


namespace Sphynx
{
	class SPHYNX_API Input
	{
	public:
		/* Initialization */
		static void Init();
		static void Update(); // should be called before every event has been processed
		static void Shutdown();

		/* Keyboard */
		static bool IsKeyPressed(int32_t keycode);
		static bool IsKeyReleased(int32_t keycode);
		static bool IsKeyDown(int32_t keycode);
		static bool IsKeyUp(int32_t keycode);

		/* Mouse */
		static bool IsMouseButtonPressed(int32_t button);
		static bool IsMouseButtonReleased(int32_t button);
		static bool IsMouseButtonDown(int32_t button);
		static bool IsMouseButtonUp(int32_t button);
		static std::pair<int64_t, int64_t> GetMousePosition();
		static int64_t GetMousePositionX();
		static int64_t GetMousePositionY();

		/* TODO: Gamepad */

	private:
		/* Platform-specific defined implementations */
		virtual void InitImpl() = 0;
		virtual void UpdateImpl() = 0;
		virtual void ShutdownImpl() = 0;

		/* Keyboard */
		virtual bool IsKeyPressedImpl(int32_t keycode) = 0;
		virtual bool IsKeyReleasedImpl(int32_t keycode) = 0;
		virtual bool IsKeyDownImpl(int32_t keycode) = 0;
		virtual bool IsKeyUpImpl(int32_t keycode) = 0;

		/* Mouse */
		virtual bool IsMouseButtonPressedImpl(int32_t button) = 0;
		virtual bool IsMouseButtonReleasedImpl(int32_t button) = 0;
		virtual bool IsMouseButtonDownImpl(int32_t button) = 0;
		virtual bool IsMouseButtonUpImpl(int32_t button) = 0;
		virtual std::pair<int64_t, int64_t> GetMousePositionImpl() = 0;
		virtual int64_t GetMousePositionXImpl() = 0;
		virtual int64_t GetMousePositionYImpl() = 0;

		/* TODO: Gamepad */

	private:
		/* Instance */
		static Input* s_Instance;
	};
}