#pragma once

#include "Core/Core.h"
#include "Renderer/Framebuffer.h"


namespace Sphynx
{
	class SPHYNX_API OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual Color ReadPixel(uint32_t attachmentIndex, uint32_t x, uint32_t y) override;

		virtual void ClearAttachment(uint32_t attachmentIndex, uint32_t value) override;

		virtual void* GetColorAttachment(uint32_t index = 0) const override { return (void*)m_ColorAttachments[index]; }
		virtual void* GetDepthAttachment() const override { return (void*)m_DepthAttachment; }

		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }
	private:
		uint32_t m_RendererID = 0;
		FramebufferSpecification m_Specification;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
		FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment = 0;
	};
}
