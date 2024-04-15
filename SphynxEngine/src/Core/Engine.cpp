#include "Engine.h"
#include "Logging/Log.h"
#include "Asset/AssetManager.h"


namespace Sphynx
{
	void Engine::Init()
	{
		Log::Init();
		AssetManager::Init();
		SPX_CORE_LOG_DISPLAY("Sphynx Engine initializing...");
	}

	void Engine::Shutdown()
	{
		SPX_CORE_LOG_DISPLAY("Sphynx Engine shutting down...");
		AssetManager::Shutdown();
		Log::Shutdown();
	}
}
