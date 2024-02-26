#include "Engine.h"
#include "Logging/Log.h"


namespace Sphynx
{
	void Engine::Init()
	{
		Log::Init();
		SPX_LOG_CORE_DISPLAY("Sphynx Engine initializing...");
	}

	void Engine::Shutdown()
	{
		SPX_LOG_CORE_DISPLAY("Sphynx Engine shutting down...");
		Log::Shutdown();
	}
}
