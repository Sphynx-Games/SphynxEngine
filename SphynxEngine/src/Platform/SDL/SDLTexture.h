#pragma once

#include "Renderer/Texture.h"

struct SDL_Texture;

namespace Sphynx
{
	class SDLTexture : public Texture
	{
	public:
		SDLTexture(void* data, Vector2i size);
		virtual ~SDLTexture();

	private:
		SDLTexture(SDL_Texture* texture);

	public:
		SDL_Texture* GetTexture() const { return m_Texture; };

	private:
		SDL_Texture* m_Texture;

	private:
		friend class SDLTextureLoader;
	};

	class SDLTextureLoader
	{
	public:
		static SDLTexture* Load(const std::filesystem::path& path);
	};
}