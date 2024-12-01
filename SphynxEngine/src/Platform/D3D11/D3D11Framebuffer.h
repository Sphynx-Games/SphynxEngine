#pragma once

#include "Core/Core.h"
#include "Renderer/Framebuffer.h"


struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11ShaderResourceView;

namespace Sphynx
{
	class SPHYNX_API D3D11Framebuffer : public Framebuffer
	{
	public:
		D3D11Framebuffer(const FramebufferSpecification& spec);
		virtual ~D3D11Framebuffer();

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual Color ReadPixel(uint32_t attachmentIndex, uint32_t x, uint32_t y) override;

		virtual void ClearAttachment(uint32_t attachmentIndex, uint32_t value) override;

		virtual void* GetColorAttachment(uint32_t index = 0) const override { return (void*)m_ColorAttachmentResources[index]; }
		virtual void* GetDepthAttachment() const override { return (void*)m_DepthAttachmentResource; }

		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }

	private:
		FramebufferSpecification m_Specification;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
		FramebufferTextureSpecification m_DepthAttachmentSpecification;

		std::vector<ID3D11RenderTargetView*> m_ColorAttachments;
		std::vector<ID3D11ShaderResourceView*> m_ColorAttachmentResources;
		ID3D11DepthStencilView* m_DepthAttachment;
		ID3D11ShaderResourceView* m_DepthAttachmentResource;
	};
}
