#pragma once

#include "Core/Core.h"
#include "Renderer/Buffer.h"


namespace Sphynx
{
	class SPHYNX_API OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(void* vertices, size_t size, const BufferLayout& layout);
		virtual ~OpenGLVertexBuffer() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const BufferLayout& GetLayout() const override;
		virtual void SetLayout(const BufferLayout& layout) override;

	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};

	class SPHYNX_API OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetCount() const override;

	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};
}
