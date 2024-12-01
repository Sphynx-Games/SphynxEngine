#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Platform/D3D11/D3D11VertexArray.h"


namespace Sphynx
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE: return nullptr;
		case RendererAPI::API::OPENGL: return new OpenGLVertexArray();
		case RendererAPI::API::D3D11: return new D3D11VertexArray();
		default: return nullptr;
		}
		return nullptr;
	}
}
