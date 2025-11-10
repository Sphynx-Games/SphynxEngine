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

	};
}