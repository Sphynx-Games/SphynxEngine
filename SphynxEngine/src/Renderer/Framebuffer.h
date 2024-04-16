#pragma once

#include <cstdint>
#include <vector>


namespace Sphynx
{
	enum class FramebufferTextureFormat
	{
		None = 0,

		// Color
		RGBA8,
		RED_INTEGER,

		// Depth/stencil
		DEPTH24STENCIL8,

		// Defaults
		Depth = DEPTH24STENCIL8
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			: TextureFormat(format) {}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
		// TODO: filtering/wrap
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			: Attachments(attachments) {}

		std::vector<FramebufferTextureSpecification> Attachments;
	};

	struct FramebufferSpecification
	{
		uint32_t Width = 0;
		uint32_t Height = 0;
		FramebufferAttachmentSpecification Attachments;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind(uint32_t index = 0u) = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual int ReadPixel(uint32_t attachmentIndex, uint32_t x, uint32_t y) = 0;

		virtual void ClearAttachment(uint32_t attachmentIndex, uint32_t value) = 0;

		virtual void* GetColorAttachment(uint32_t index = 0u) const = 0;
		virtual void* GetDepthAttachment() const = 0;

		virtual const FramebufferSpecification& GetSpecification() const = 0;

		static Framebuffer* Create(const FramebufferSpecification& spec);
	};
}
