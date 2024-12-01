#include "spxpch.h"
#include "Texture.h"

#include "Logging/Log.h"
#include "Renderer/Renderer.h"
#include "Renderer/RendererAPI.h"
#include "Platform/SDL/SDLFont.h"

namespace Sphynx
{
	Font* Font::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE:    SPX_CORE_LOG_ERROR("RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::SDL:     return new SDLFont();
		}

		SPX_CORE_LOG_ERROR("Unknown RendererAPI!");
		return nullptr;
	}
}