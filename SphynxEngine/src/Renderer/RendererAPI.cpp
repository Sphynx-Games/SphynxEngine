#include "spxpch.h"
#include "RendererAPI.h"
#include "Logging/Log.h"
#include "Platform/SDL/SDLRendererAPI.h"
#include <stdio.h>

namespace Sphynx
{
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::SDL;

	RendererAPI* RendererAPI::Create()
	{
		switch (s_API)
		{
			case RendererAPI::API::None: SPX_CORE_LOG_ERROR("RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::SDL:  return new SDLRendererAPI();
		}

		SPX_CORE_LOG_ERROR("Unknown RendererAPI!");
		return nullptr;
	}
}