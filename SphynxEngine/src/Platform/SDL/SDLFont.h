#pragma once

#include "Renderer/Font.h"

#include <memory>


struct TTF_Font;
struct SDL_IOStream;

namespace Sphynx
{
	class SDLFont : public Font
	{
	public:
		SDLFont();
		virtual ~SDLFont();

	private:
		SDLFont(TTF_Font* font);

	public:
		TTF_Font* GetFont() const { return m_Font; };

	private:
		TTF_Font* m_Font;

	private:
		friend class SDLFontLoader;
	};

	class SDLFontLoader
	{
	public:
		static SDLFont* Load(const std::filesystem::path& path);
	};
}