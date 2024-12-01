#include "GraphicsContext.h"

#include "Renderer.h"

#include "Platform/SDL/SDLGraphicsContext.h"
#include "Platform/OpenGL/OpenGLGraphicsContext.h"
#include "Platform/D3D11/D3D11GraphicsContext.h"


namespace Sphynx
{
	GraphicsContext* GraphicsContext::Create(Window* window)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE: return nullptr;
		case RendererAPI::API::SDL: return new SDLGraphicsContext(window);
		case RendererAPI::API::OPENGL: return new OpenGLGraphicsContext(window);
		//case RendererAPI::API::D3D11: return new D3D11GraphicsContext(window);
		}

		return nullptr;
	}
}
