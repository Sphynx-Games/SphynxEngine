#include "spxpch.h"
#include "Framebuffer.h"

#include "Renderer.h"
#include "Platform/SDL/SDLFramebuffer.h"

namespace Sphynx
{
	Framebuffer* Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	return nullptr;
		case RendererAPI::API::SDL:		return new SDLFramebuffer(spec);
		}

		return nullptr;
	}
}
