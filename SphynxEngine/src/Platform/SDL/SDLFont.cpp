#include "spxpch.h"
#include "SDLFont.h"
#include "Logging/Log.h"
#include "Renderer/Font.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_iostream.h>


namespace Sphynx
{
	SDLFont::SDLFont() : m_Font(nullptr)
	{
	}

	SDLFont::~SDLFont()
	{
		if (m_Font != nullptr)
		{
			TTF_CloseFont(m_Font);
			m_Font = nullptr;
		}
	}

	SDLFont::SDLFont(TTF_Font* font) : m_Font(font)
	{
	}

	SDLFont* SDLFontLoader::Load(const std::filesystem::path& path)
	{
		constexpr const int defaultSize = 16;
		TTF_Font* font = TTF_OpenFont(path.string().c_str(), defaultSize);

		if (font == nullptr)
		{
			SPX_CORE_LOG_ERROR("Unable to load font from {}! SDL_ttf Error: {}", path.string().c_str(), SDL_GetError());
			return nullptr;
		}

		return new SDLFont(font);
	}
}