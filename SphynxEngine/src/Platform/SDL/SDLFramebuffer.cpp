#include "SDLFramebuffer.h"

#include <SDL3/SDL.h>
#include <Sphynx.h>
#include "Renderer/Renderer.h"
#include "Platform/SDL/SDLRendererAPI.h"


namespace Sphynx
{

	static const uint32_t s_MaxFramebufferSize = 8192;

	namespace Utils
	{
		static SDL_PixelFormatEnum SphynxTextureFormatToSDL(FramebufferTextureFormat format)
		{
			switch (format)
			{
			case FramebufferTextureFormat::RGBA8:       return SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA8888;
			case FramebufferTextureFormat::RED_INTEGER: return SDL_PixelFormatEnum::SDL_PIXELFORMAT_INDEX8;
			}

			return SDL_PixelFormatEnum::SDL_PIXELFORMAT_UNKNOWN;
		}

		static void CreateTextures(SDL_Texture** textures, FramebufferTextureSpecification* specs, uint32_t count, uint32_t width, uint32_t height)
		{
			SDL_Renderer* renderer = static_cast<const Sphynx::SDLRendererAPI*>(Sphynx::Renderer2D::GetRendererAPI())->GetSDLRenderer();

			for (uint32_t i = 0; i < count; ++i)
			{
				textures[i] = SDL_CreateTexture(
					renderer,
					SphynxTextureFormatToSDL(specs[i].TextureFormat),
					SDL_TEXTUREACCESS_TARGET,
					width,
					height
				);
			}
		}

		static void BindTexture(SDL_Texture* texture)
		{
			SDL_SetRenderTarget(SDL_GetRendererFromTexture(texture), texture);
		}

		static bool IsDepthFormat(FramebufferTextureFormat format)
		{
			switch (format)
			{
			case FramebufferTextureFormat::DEPTH24STENCIL8:  return true;
			}

			return false;
		}

	}

	SDLFramebuffer::SDLFramebuffer(const FramebufferSpecification& spec) : 
		m_Specification(spec),
		m_ColorAttachmentSpecifications(),
		m_DepthAttachmentSpecification(FramebufferTextureFormat::None),
		m_ColorAttachments(),
		m_DepthAttachment(nullptr)
	{
		for (auto spec : m_Specification.Attachments.Attachments)
		{
			if (!Utils::IsDepthFormat(spec.TextureFormat))
				m_ColorAttachmentSpecifications.emplace_back(spec);
			else
				m_DepthAttachmentSpecification = spec;
		}

		Invalidate();
	}

	SDLFramebuffer::~SDLFramebuffer()
	{
		for (SDL_Texture* texture : m_ColorAttachments)
		{
			SDL_DestroyTexture(texture);
		}
		m_ColorAttachments.clear();

		if (m_DepthAttachment != nullptr)
		{
			SDL_DestroyTexture(m_DepthAttachment);
			m_DepthAttachment = nullptr;
		}
	}

	void SDLFramebuffer::Invalidate()
	{
		for (SDL_Texture* texture : m_ColorAttachments)
		{
			SDL_DestroyTexture(texture);
		}
		m_ColorAttachments.clear();

		if (m_DepthAttachment != nullptr)
		{
			SDL_DestroyTexture(m_DepthAttachment);
			m_DepthAttachment = nullptr;
		}

		// Attachments
		if (m_ColorAttachmentSpecifications.size())
		{
			m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());
			Utils::CreateTextures(
				m_ColorAttachments.data(), 
				m_ColorAttachmentSpecifications.data(), 
				(uint32_t)m_ColorAttachments.size(), 
				m_Specification.Width, 
				m_Specification.Height
			);
		}

		if (m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None)
		{
			Utils::CreateTextures(
				&m_DepthAttachment, 
				&m_DepthAttachmentSpecification, 
				1u,
				m_Specification.Width,
				m_Specification.Height
			);
		}
	}

	void SDLFramebuffer::Bind(uint32_t index)
	{
		SDL_SetRenderTarget(SDL_GetRendererFromTexture(m_ColorAttachments[index]), m_ColorAttachments[index]);
	}

	void SDLFramebuffer::Unbind()
	{
		SDL_Renderer* renderer = static_cast<const Sphynx::SDLRendererAPI*>(Sphynx::Renderer2D::GetRendererAPI())->GetSDLRenderer();
		SDL_SetRenderTarget(renderer, nullptr);
	}

	void SDLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
		{
			SPX_CORE_LOG_ERROR("Attempted to resize framebuffer to %d, %d", width, height);
			return;
		}
		m_Specification.Width = width;
		m_Specification.Height = height;

		Invalidate();
	}

	int SDLFramebuffer::ReadPixel(uint32_t attachmentIndex, uint32_t x, uint32_t y)
	{
		SPX_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size());
		// TODO
		return 0;
	}

	void SDLFramebuffer::ClearAttachment(uint32_t attachmentIndex, uint32_t value)
	{
		SPX_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size());
		SDL_Renderer* renderer = static_cast<const Sphynx::SDLRendererAPI*>(Sphynx::Renderer2D::GetRendererAPI())->GetSDLRenderer();

		SDL_Texture* texture = m_ColorAttachments[attachmentIndex];
		SPX_CORE_ASSERT(SDL_GetRenderTarget(renderer) == texture);

		// save current color
		Color prevColor = Color::White;
		SDL_GetRenderDrawColor(renderer, &prevColor.R, &prevColor.G, &prevColor.G, &prevColor.A);

		// set color input
		Color currColor = Color::FromHex(value);
		SDL_SetRenderDrawColor(renderer, currColor.R, currColor.G, currColor.B, currColor.A);

		// clear
		SDL_RenderClear(SDL_GetRendererFromTexture(texture));

		// reset color
		SDL_SetRenderDrawColor(renderer, prevColor.R, prevColor.G, prevColor.B, prevColor.A);

	}
}