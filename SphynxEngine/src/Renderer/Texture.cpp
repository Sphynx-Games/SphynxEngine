#include "Texture.h"
#include "Logging/Log.h"
#include "Renderer/Renderer.h"
#include "Renderer/RendererAPI.h"
#include "Platform/SDL/SDLTexture.h"

namespace Sphynx 
{
	Texture* Texture::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    SPX_LOG_CORE_ERROR("RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::SDL:     return new SDLTexture(path);
		}

		SPX_LOG_CORE_ERROR("Unknown RendererAPI!");
		return nullptr;
	}

	/*Texture2D* Texture2D::Create(const TextureSpecification& specification)
	{
		switch (Renderer::GetAPI())
		{
		     case RendererAPI::API::None:    printf("RendererAPI::None is currently not supported!"); return nullptr;
			 case RendererAPI::API::SDL:  return new SDLTexture2D(specification);
		}

		printf("Unknown RendererAPI!");
		return nullptr;
	}*/
}