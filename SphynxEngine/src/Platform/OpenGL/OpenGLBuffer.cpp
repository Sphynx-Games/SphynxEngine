#include "spxpch.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>


namespace Sphynx
{
	OpenGLVertexBuffer::OpenGLVertexBuffer(void* vertices, size_t size, const BufferLayout& layout) :
		m_RendererID(0),
		m_Layout(layout)
	{
		glCreateBuffers(1, &m_RendererID);
		//glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		//glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
		glNamedBufferData(m_RendererID, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	const BufferLayout& OpenGLVertexBuffer::GetLayout() const
	{
		return m_Layout;
	}

	void OpenGLVertexBuffer::SetLayout(const BufferLayout& layout)
	{
		m_Layout = layout;
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) :
		m_RendererID(0),
		m_Count(count)
	{
		glCreateBuffers(1, &m_RendererID);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
		glNamedBufferData(m_RendererID, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	uint32_t OpenGLIndexBuffer::GetCount() const
	{
		return m_Count;
	}
}
