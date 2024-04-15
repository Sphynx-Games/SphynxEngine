#pragma once

#include "Renderer/Framebuffer.h"


struct SDL_Texture;

namespace Sphynx
{
	class SDLFramebuffer : public Framebuffer
	{
	public:
		SDLFramebuffer(const FramebufferSpecification& spec);
		virtual ~SDLFramebuffer();

		void Invalidate();

		virtual void Bind(uint32_t index = 0u) override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual int ReadPixel(uint32_t attachmentIndex, uint32_t x, uint32_t y) override;

		virtual void ClearAttachment(uint32_t attachmentIndex, uint32_t value) override;

		virtual void* GetColorAttachment(uint32_t index = 0) const override { return m_ColorAttachments[index]; }
		virtual void* GetDepthAttachment() const override { return m_DepthAttachment; }

		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }
	private:
		FramebufferSpecification m_Specification;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
		FramebufferTextureSpecification m_DepthAttachmentSpecification;

		std::vector<SDL_Texture*> m_ColorAttachments;
		SDL_Texture* m_DepthAttachment;
	};
}
