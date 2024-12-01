#include "spxpch.h"
#include "RendererAPI.h"

#include "Platform/SDL/SDLRendererAPI.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Sphynx
{
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::SDL;

	RendererAPI* RendererAPI::Create()
	{
		switch (s_API)
		{
			case RendererAPI::API::NONE: SPX_CORE_LOG_ERROR("RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::SDL:  return new SDLRendererAPI();
			case RendererAPI::API::OPENGL:  return new OpenGLRendererAPI();
		}

		SPX_CORE_LOG_ERROR("Unknown RendererAPI!");
		return nullptr;
	}
}