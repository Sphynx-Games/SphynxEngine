#include "spxpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/D3D11/D3D11Buffer.h"


namespace Sphynx
{
	VertexBuffer* VertexBuffer::Create(void* vertices, size_t size, const BufferLayout& layout)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE: return nullptr;
		case RendererAPI::API::OPENGL: return new OpenGLVertexBuffer(vertices, size, layout);
		case RendererAPI::API::D3D11: return new D3D11VertexBuffer(vertices, size, layout);
		default: return nullptr;
		}
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE: return nullptr;
		case RendererAPI::API::OPENGL: return new OpenGLIndexBuffer(indices, count);
		case RendererAPI::API::D3D11: return new D3D11IndexBuffer(indices, count);
		default: return nullptr;
		}
		return nullptr;
	}
}
