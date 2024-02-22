#include "RendererAPI.h"
#include "Platform/SLD/SLDRendererAPI.h"
#include <stdio.h>

namespace Sphynx
{
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::SDL;

	RendererAPI* RendererAPI::Create()
	{
		switch (s_API)
		{
			case RendererAPI::API::None: printf("RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::SDL:  return new SDLRendererAPI();
		}

		printf("Unknown RendererAPI!");
		return nullptr;
	}
}