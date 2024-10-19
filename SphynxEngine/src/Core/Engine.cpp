#include "spxpch.h"
#include "Engine.h"
#include "Logging/Log.h"
#include "Asset/AssetManager.h"

#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"
#include "SDL3_ttf/SDL_ttf.h"


namespace Sphynx
{
	void Engine::Init()
	{
		// First initialize core engine systems
		Log::Init();
		SPX_CORE_LOG_DISPLAY("Initializing Sphynx Engine");

		// External libraries
		SPX_CORE_LOG_DISPLAY("Initializing External Libraries");
		{
			// Init SDL main
			if (!SDL_Init(0))
			{
				SPX_CORE_LOG_ERROR("SDL could not initialize! SDL_Error: {}", SDL_GetError());
			}

			// Init SDL Image
			if (IMG_Init(~0) == 0)
			{
				SPX_CORE_LOG_ERROR("SDL_image could not initialize! SDL_Error: {}", SDL_GetError());
			}

			// Init SDL_ttf
			if (!TTF_Init())
			{
				SPX_CORE_LOG_ERROR("SDL_ttf could not initialize! TTF_Error: {}", SDL_GetError());
			}
		}

		AssetManager::Init();
	}

	void Engine::Shutdown()
	{
		AssetManager::Shutdown();

		// External libraries
		{
			TTF_Quit();
			IMG_Quit();
			SDL_Quit();
		}
		SPX_CORE_LOG_DISPLAY("Shutting down External Libraries");

		SPX_CORE_LOG_DISPLAY("Shutting down Sphynx Engine");
		Log::Shutdown();
	}
}
