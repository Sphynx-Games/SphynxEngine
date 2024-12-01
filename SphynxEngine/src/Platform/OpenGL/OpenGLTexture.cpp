#include "OpenGLTexture.h"

#include <glad/glad.h>
#include "stb_image.h"
#include "OpenGLAssert.h"


namespace Sphynx
{
	OpenGLTexture::OpenGLTexture(void* data, const Vector2u& size) :
		Texture(size),
		m_RendererID(0)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID); CHECK_GL_ERRORS;

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR); CHECK_GL_ERRORS;
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST); CHECK_GL_ERRORS;

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT); CHECK_GL_ERRORS;
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT); CHECK_GL_ERRORS;

		glTextureStorage2D(m_RendererID, 1, GL_RGBA8, size.X, size.Y); CHECK_GL_ERRORS;
		glTextureSubImage2D(m_RendererID, 0, 0, 0, size.X, size.Y, GL_RGBA, GL_UNSIGNED_BYTE, data); CHECK_GL_ERRORS;
	}

	OpenGLTexture::OpenGLTexture(uint32_t id, const Vector2u& size) :
		Texture(size),
		m_RendererID(id)
	{

	}

	OpenGLTexture::~OpenGLTexture()
	{
		glDeleteTextures(1, &m_RendererID); CHECK_GL_ERRORS;
	}

	void* OpenGLTexture::GetNativeTexture() const
	{
		return (void*)m_RendererID;
	}

	//void OpenGLTexture::SetData(void* data)
	//{
	//	glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	//}

	void OpenGLTexture::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID); CHECK_GL_ERRORS;
	}

	void OpenGLTexture::Unbind() const
	{
		glBindTextureUnit(0, 0); CHECK_GL_ERRORS;
	}

	Sphynx::OpenGLTexture* OpenGLTextureLoader::Load(const std::filesystem::path& path)
	{
		GLuint rendererID = 0;
		int width, height, channels;
		//stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.string().c_str(), &width, &height, &channels, 0);

		if (data == nullptr)
		{
			SPX_CORE_LOG_ERROR("Failed to load image {}", path.string());
			return nullptr;
		}

		uint32_t internalFormat = channels == 4 ? GL_RGBA8 : channels == 3 ? GL_RGB8 : GL_R8;
		uint32_t format = channels == 4 ? GL_RGBA : channels == 3 ? GL_RGB : GL_RED;
		
		glCreateTextures(GL_TEXTURE_2D, 1, &rendererID); CHECK_GL_ERRORS;

		glTextureParameteri(rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR); CHECK_GL_ERRORS;
		glTextureParameteri(rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST); CHECK_GL_ERRORS;

		glTextureParameteri(rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT); CHECK_GL_ERRORS;
		glTextureParameteri(rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT); CHECK_GL_ERRORS;

		glTextureStorage2D(rendererID, 1, internalFormat, width, height); CHECK_GL_ERRORS;
		glTextureSubImage2D(rendererID, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data); CHECK_GL_ERRORS;

		stbi_image_free(data);

		return new OpenGLTexture(rendererID, Vector2u{ (uint32_t)width, (uint32_t)height });
	}
}