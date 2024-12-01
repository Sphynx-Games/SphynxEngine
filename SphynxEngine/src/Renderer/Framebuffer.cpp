#include "spxpch.h"
#include "Framebuffer.h"

#include "Renderer.h"
#include "Platform/SDL/SDLFramebuffer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Sphynx
{
	Framebuffer* Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE:	return nullptr;
		case RendererAPI::API::SDL:		return new SDLFramebuffer(spec);
		case RendererAPI::API::OPENGL:	return new OpenGLFramebuffer(spec);
		}

		return nullptr;
	}
}
