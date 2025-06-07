#include "spxpch.h"
#include "Texture.h"

#include "Logging/Log.h"
#include "Renderer/Renderer.h"
#include "Renderer/RendererAPI.h"
#include "Platform/SDL/SDLTexture.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Sphynx 
{
	Texture* Texture::Create(void* data, const Vector2u& size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE:		SPX_CORE_LOG_ERROR("RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::SDL:			return new SDLTexture(data, size);
		case RendererAPI::API::OPENGL:		return new OpenGLTexture(data, size);
		}

		SPX_CORE_LOG_ERROR("Unknown RendererAPI!");
		return nullptr;
	}

	Texture* TextureLoader::Load(const std::filesystem::path& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE:		SPX_CORE_LOG_ERROR("RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::SDL:			return SDLTextureLoader::Load(path);
		case RendererAPI::API::OPENGL:		return OpenGLTextureLoader::Load(path);
		}

		SPX_CORE_LOG_ERROR("Unknown RendererAPI!");
		return nullptr;
	}
}