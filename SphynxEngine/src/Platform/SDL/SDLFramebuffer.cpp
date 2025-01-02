#include "spxpch.h"
#include "SDLFramebuffer.h"
#include "Renderer/Renderer.h"
#include "Platform/SDL/SDLRendererAPI.h"

#include <SDL3/SDL.h>
#include <Sphynx.h>


namespace Sphynx
{

	static const uint32_t s_MaxFramebufferSize = 8192;

	namespace Utils
	{
		static SDL_PixelFormat SphynxTextureFormatToSDL(FramebufferTextureFormat format)
		{
			switch (format)
			{
			case FramebufferTextureFormat::RGBA8:       return SDL_PixelFormat::SDL_PIXELFORMAT_RGBA8888;
			case FramebufferTextureFormat::RED_INTEGER: return SDL_PixelFormat::SDL_PIXELFORMAT_INDEX8;
			}

			return SDL_PixelFormat::SDL_PIXELFORMAT_UNKNOWN;
		}

		static void CreateTextures(SDL_Texture** textures, FramebufferTextureSpecification* specs, uint32_t count, uint32_t width, uint32_t height)
		{
			SDL_Renderer* renderer = static_cast<const Sphynx::SDLRendererAPI*>(Sphynx::Renderer2D::GetRendererAPI())->GetRootSDLRenderer();

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
		m_Renderer(nullptr),
		m_Specification(spec),
		m_ColorAttachmentSpecifications(),
		m_DepthAttachmentSpecification(FramebufferTextureFormat::None),
		m_ColorAttachments(),
		m_DepthAttachment(nullptr)
	{
		SDL_Renderer* renderer = static_cast<const Sphynx::SDLRendererAPI*>(Sphynx::Renderer2D::GetRendererAPI())->GetRootSDLRenderer();
		SPX_CORE_ASSERT(renderer != nullptr);

		m_Renderer = renderer;

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

	void SDLFramebuffer::Bind()
	{
		// SDL only supports a single attachment
		const int32_t index = 0;
		SDL_SetRenderTarget(SDL_GetRendererFromTexture(m_ColorAttachments[index]), m_ColorAttachments[index]);

		SDL_Rect rect;
		rect.x = rect.y = 0;
		rect.w = m_Specification.Width;
		rect.h = m_Specification.Height;

		SDL_SetRenderViewport(SDL_GetRendererFromTexture(m_ColorAttachments[index]), &rect);
	}

	void SDLFramebuffer::Unbind()
	{
		SDL_SetRenderTarget(m_Renderer, nullptr);
		SDL_SetRenderViewport(m_Renderer, nullptr);
	}

	void SDLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
		{
			SPX_CORE_LOG_ERROR("Attempted to resize framebuffer to {}, {}", width, height);
			return;
		}
		m_Specification.Width = width;
		m_Specification.Height = height;

		Invalidate();
	}

	Sphynx::Color SDLFramebuffer::ReadPixel(uint32_t attachmentIndex, uint32_t x, uint32_t y)
	{
		SPX_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size());

		SDL_Rect rect;
		rect.x = x;
		rect.y = y;
		rect.w = rect.h = 1;

		if (SDL_Surface* surface = SDL_RenderReadPixels(m_Renderer, &rect))
		{
			Color color;
			SDL_GetRGBA(*(uint32_t*)surface->pixels, SDL_GetPixelFormatDetails(surface->format), nullptr, &color.R, &color.G, &color.B, &color.A);
			return color;
		}

		return Color::Black;
	}

	void SDLFramebuffer::ClearAttachment(uint32_t attachmentIndex, uint32_t value)
	{
		SPX_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size());
		SDL_Renderer* renderer = static_cast<const Sphynx::SDLRendererAPI*>(Sphynx::Renderer2D::GetRendererAPI())->GetRootSDLRenderer();

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

	void* SDLFramebuffer::GetColorAttachment(uint32_t index) const
	{
		return m_ColorAttachments[index];
	}

	void* SDLFramebuffer::GetDepthAttachment() const
	{
		return m_DepthAttachment;
	}

	const FramebufferSpecification& SDLFramebuffer::GetSpecification() const
	{
		return m_Specification;
	}
}