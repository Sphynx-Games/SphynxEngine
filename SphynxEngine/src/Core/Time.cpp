#include "Time.h"
#include "Logging/Log.h"


namespace Sphynx
{
	std::chrono::steady_clock::time_point Time::s_LastTime = std::chrono::steady_clock::now();
	float Time::s_DeltaTime = 0.0f;
	float Time::s_TimeScale = 1.0f;
	bool Time::s_IsInitialized = false;

	void Time::Init()
	{
		if (s_IsInitialized)
		{
			SPX_LOG_CORE_WARNING("Trying to initialize global time when is already initialized");
			return;
		}

		s_IsInitialized = true;
		s_LastTime = std::chrono::steady_clock::now();
		s_DeltaTime = 0.0f;
		s_TimeScale = 1.0f;
	}

	void Time::Tick()
	{
		if (!s_IsInitialized)
		{
			SPX_LOG_CORE_WARNING("Trying to tick global time when it is not initialized");
			return;
		}

		std::chrono::steady_clock::time_point current = std::chrono::steady_clock::now();
		std::chrono::duration<float> elapsed = std::chrono::duration_cast<std::chrono::duration<float>>(current - s_LastTime);
		s_DeltaTime = elapsed.count();
		s_LastTime = current;
	}

	void Time::Shutdown()
	{
		if (!s_IsInitialized)
		{
			SPX_LOG_CORE_WARNING("Trying to shutdown global time when it is not initialized");
			return;
		}

		s_IsInitialized = false;
	}

	float Time::GetTimeScale()
	{
		return s_TimeScale;
	}

	void Time::SetTimeScale(float timeScale)
	{
		s_TimeScale = timeScale;
	}

	float Time::GetDeltaTime()
	{
		return s_DeltaTime;
	}

	float Time::GetSeconds()
	{
		return s_DeltaTime;
	}

	float Time::GetMilliseconds()
	{
		return s_DeltaTime * 1000.f;
	}

	float Time::GetScaledDeltaTime()
	{
		return s_DeltaTime * s_TimeScale;
	}

	float Time::GetScaledSeconds()
	{
		return s_DeltaTime * s_TimeScale;
	}

	float Time::GetScaledMilliseconds()
	{
		return s_DeltaTime * s_TimeScale * 1000.0f;
	}
}