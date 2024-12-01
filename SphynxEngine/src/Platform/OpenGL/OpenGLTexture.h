#pragma once

#include "Core/Core.h"
#include "Renderer/Texture.h"


namespace Sphynx
{
	class SPHYNX_API OpenGLTexture : public Texture
	{
	public:
		OpenGLTexture(void* data, const Vector2u& size);
		virtual ~OpenGLTexture() override;
	
	private:
		OpenGLTexture(uint32_t id, const Vector2u& size);

	public:
		void* GetNativeTexture() const override;

		//virtual void SetData(void* data) override;
		virtual void Bind(uint32_t slot = 0) const override;
		virtual void Unbind() const override;

	private:
		uint32_t m_RendererID;

	private:
		friend class OpenGLTextureLoader;

	};

	class OpenGLTextureLoader
	{
	public:
		static OpenGLTexture* Load(const std::filesystem::path& path);
	};
}
