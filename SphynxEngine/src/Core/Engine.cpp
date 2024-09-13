#include "spxpch.h"
#include "Engine.h"
#include "Logging/Log.h"
#include "Asset/AssetManager.h"


namespace Sphynx
{
	void Engine::Init()
	{
		// First initialize core engine systems
		Log::Init();
		SPX_CORE_LOG_DISPLAY("Initializing Sphynx Engine");

		AssetManager::Init();
	}

	void Engine::Shutdown()
	{
		AssetManager::Shutdown();

		SPX_CORE_LOG_DISPLAY("Shutting down Sphynx Engine");
		Log::Shutdown();
	}
}
