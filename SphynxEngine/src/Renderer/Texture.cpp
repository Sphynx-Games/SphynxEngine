#include "spxpch.h"
#include "Texture.h"

#include "Logging/Log.h"
#include "Renderer/Renderer.h"
#include "Renderer/RendererAPI.h"
#include "Platform/SDL/SDLTexture.h"

namespace Sphynx 
{
	Texture* Texture::Create(void* data, Vector2i size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    SPX_CORE_LOG_ERROR("RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::SDL:     return new SDLTexture(data, size);
		}

		SPX_CORE_LOG_ERROR("Unknown RendererAPI!");
		return nullptr;
	}
}