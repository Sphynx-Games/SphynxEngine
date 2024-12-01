#pragma once

#include "Renderer/Texture.h"

struct SDL_Texture;

namespace Sphynx
{
	class SDLTexture : public Texture
	{
	public:
		SDLTexture(void* data, const Vector2u& size);
		virtual ~SDLTexture();

	private:
		SDLTexture(SDL_Texture* texture);

	public:
		SDL_Texture* GetTexture() const { return m_Texture; }
		virtual void* GetNativeTexture() const override { return GetTexture(); }

		virtual void Bind(uint32_t slot = 0) const override {};
		virtual void Unbind() const override {};

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