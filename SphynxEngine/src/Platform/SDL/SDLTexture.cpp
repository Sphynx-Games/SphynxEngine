#include "SDLTexture.h"
#include "Renderer/Renderer2D.h"
#include "SDLRendererAPI.h"
#include "Logging/Log.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

namespace Sphynx 
{
	SDLTexture::SDLTexture(void* data, Vector2i size)
	{
		m_Size = size;

		const SDLRendererAPI* renderer = dynamic_cast<const SDLRendererAPI*>(Renderer2D::GetRendererAPI());
		if (renderer == nullptr)
		{
			SPX_LOG_CORE_ERROR("RendererAPI is not of SDL type");
			return;
		}

		m_Texture = SDL_CreateTexture(renderer->GetSDLRenderer(), SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, size.X, size.Y);

		if (SDL_UpdateTexture(m_Texture, nullptr, data, m_Size.X * sizeof(uint8_t)) < 0)
		{
			SPX_LOG_CORE_ERROR("Unable to create texture! SDL Error: {}", SDL_GetError());
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
		SDL_QueryTexture(texture, nullptr, nullptr, &m_Size.X, &m_Size.Y);
	}

	SDLTexture* SDLTextureLoader::Load(const std::filesystem::path& path)
	{
		const SDLRendererAPI* renderer = dynamic_cast<const SDLRendererAPI*>(Renderer2D::GetRendererAPI());
		if (renderer == nullptr)
		{
			SPX_LOG_CORE_ERROR("RendererAPI is not of SDL type");
			return nullptr;
		}

		// load image
		SDL_Texture* texture = IMG_LoadTexture(renderer->GetSDLRenderer(), path.string().c_str());
		if (texture == nullptr)
		{
			SPX_LOG_CORE_ERROR("Unable to load texture from {}! SDL Error: {}", path.string().c_str(), SDL_GetError());
			return nullptr;
		}

		return new SDLTexture(texture);
	}
}