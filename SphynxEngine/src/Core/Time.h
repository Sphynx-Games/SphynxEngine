#pragma once

#include "Core/Core.h"

#include <chrono>


namespace Sphynx
{
	class SPHYNX_API Time
	{
	public:
		static void Init();
		static void Tick();
		static void Shutdown();

		static float GetTimeScale();
		static void SetTimeScale(float timeScale);

		static float GetDeltaTime();
		static float GetSeconds();
		static float GetMilliseconds();

		static float GetScaledDeltaTime();
		static float GetScaledSeconds();
		static float GetScaledMilliseconds();

	private:
		static std::chrono::steady_clock::time_point s_LastTime;
		static float s_DeltaTime;
		static float s_TimeScale;

		static bool s_IsInitialized;
	};
}