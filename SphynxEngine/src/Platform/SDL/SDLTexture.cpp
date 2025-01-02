#include "spxpch.h"
#include "SDLTexture.h"
#include "Renderer/Renderer2D.h"
#include "SDLRendererAPI.h"
#include "Logging/Log.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

namespace Sphynx 
{
	SDLTexture::SDLTexture(void* data, const Vector2u& size) :
		Texture(size),
		m_Texture(nullptr)
	{
		const SDLRendererAPI* renderer = static_cast<const SDLRendererAPI*>(Renderer2D::GetRendererAPI());
		if (renderer == nullptr)
		{
			SPX_CORE_LOG_ERROR("RendererAPI is not of SDL type");
			return;
		}

		m_Texture = SDL_CreateTexture(renderer->GetRootSDLRenderer(), SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, size.X, size.Y);

		if (!SDL_UpdateTexture(m_Texture, nullptr, data, m_Size.X * sizeof(uint8_t)))
		{
			SPX_CORE_LOG_ERROR("Unable to create texture! SDL Error: {}", SDL_GetError());
			return;
		}
	}

	SDLTexture::~SDLTexture()
	{
		if (m_Texture != nullptr)
		{
			SDL_DestroyTexture(m_Texture);
			m_Texture = nullptr;
		}
	}

	SDLTexture::SDLTexture(SDL_Texture* texture) :
		m_Texture(texture)
	{
		float width = 0;
		float height = 0;

		SDL_GetTextureSize(texture, &width, &height);
		m_Size.X = (uint32_t)width;
		m_Size.Y = (uint32_t)height;
	}

	SDLTexture* SDLTextureLoader::Load(const std::filesystem::path& path)
	{
		const SDLRendererAPI* renderer = static_cast<const SDLRendererAPI*>(Renderer2D::GetRendererAPI());
		if (renderer == nullptr)
		{
			SPX_CORE_LOG_ERROR("RendererAPI is not of SDL type");
			return nullptr;
		}

		// load image
		SDL_Texture* texture = IMG_LoadTexture(renderer->GetRootSDLRenderer(), path.string().c_str());
		if (texture == nullptr)
		{
			SPX_CORE_LOG_ERROR("Unable to load texture from {}! SDL Error: {}", path.string().c_str(), SDL_GetError());
			return nullptr;
		}

		return new SDLTexture(texture);
	}
}