#pragma once

#include "Renderer/Framebuffer.h"


struct SDL_Renderer;
struct SDL_Texture;

namespace Sphynx
{
	class SDLFramebuffer : public Framebuffer
	{
	public:
		SDLFramebuffer(const FramebufferSpecification& spec);
		virtual ~SDLFramebuffer();

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual Color ReadPixel(uint32_t attachmentIndex, uint32_t x, uint32_t y) override;

		virtual void ClearAttachment(uint32_t attachmentIndex, uint32_t value) override;

		virtual void* GetColorAttachment(uint32_t index = 0u) const override;
		virtual void* GetDepthAttachment() const override;

		virtual const FramebufferSpecification& GetSpecification() const override;

	private:
		SDL_Renderer* m_Renderer;

		FramebufferSpecification m_Specification;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
		FramebufferTextureSpecification m_DepthAttachmentSpecification;

		std::vector<SDL_Texture*> m_ColorAttachments;
		SDL_Texture* m_DepthAttachment;
	};
}
