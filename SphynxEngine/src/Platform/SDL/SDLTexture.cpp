#include "SDLTexture.h"
#include "Renderer/Renderer2D.h"
#include "SDLRendererAPI.h"
#include "Logging/Log.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

namespace Sphynx 
{
	SDLTexture::SDLTexture(const std::string& path) : m_Texture(nullptr), m_IsLoaded(false)
	{
		m_Path = path;
		Load();
	}

	SDLTexture::~SDLTexture()
	{
		if (m_IsLoaded) 
		{
			SDL_DestroyTexture(m_Texture);
			m_Texture = nullptr;
			m_IsLoaded = false;
		}
	}

	bool SDLTexture::Load()
	{
		if (m_IsLoaded) return true;

		const SDLRendererAPI* renderer = dynamic_cast<const SDLRendererAPI*>(Renderer2D::GetRendererAPI());
		if (renderer == nullptr)
		{
			SPX_LOG_CORE_ERROR("RendererAPI is not of SDL type");
			return false;
		}

		// load image
		m_Texture = IMG_LoadTexture(renderer->GetSDLRenderer(), m_Path.c_str());
		if (m_Texture == nullptr)
		{
			SPX_LOG_CORE_ERROR("Unable to create texture from {}! SDL Error: {}", m_Path.c_str(), SDL_GetError());
			return false;
		}
		
		// store texture size
		SDL_QueryTexture(m_Texture, NULL, NULL, &m_Size.X, &m_Size.Y);

		m_IsLoaded = true;
		return true;
	}
}