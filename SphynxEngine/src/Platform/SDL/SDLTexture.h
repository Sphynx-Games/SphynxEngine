#pragma once

#include "Renderer/Texture.h"

struct SDL_Texture;

namespace Sphynx
{
	class SDLTexture : public Texture
	{
	public:
		SDLTexture(const std::string& path);
		virtual ~SDLTexture();

		bool Load();

		SDL_Texture* GetTexture() const { return m_Texture; };
		bool IsLoaded() const { return m_IsLoaded; }

	private:
		SDL_Texture* m_Texture;
		bool m_IsLoaded;
	};
}