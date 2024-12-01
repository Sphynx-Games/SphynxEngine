#include "D3D11VertexArray.h"

#include "D3D11Utils.h"
#include "D3D11RendererAPI.h"
#include "D3D11Buffer.h"


namespace Sphynx
{
	D3D11VertexArray::D3D11VertexArray() :
		m_VertexBuffers(),
		m_IndexBuffer()
	{
	}

	D3D11VertexArray::~D3D11VertexArray()
	{
	}

	void D3D11VertexArray::Bind() const
	{
		ID3D11DeviceContext* context = GetD3D11DeviceContext();
		context->IASetVertexBuffers(0, (UINT)m_Buffers.size(), m_Buffers.data(), m_Strides.data(), m_Offsets.data());

		if (m_IndexBuffer != nullptr)
		{
			m_IndexBuffer->Bind();
		}
	}

	void D3D11VertexArray::Unbind() const
	{
		const std::vector<ID3D11Buffer*> nullBuffers(m_Buffers.size(), nullptr);
		const std::vector<UINT> zeros(m_Buffers.size(), 0);

		ID3D11DeviceContext* context = GetD3D11DeviceContext();
		context->IASetVertexBuffers(0, (UINT)m_Buffers.size(), nullBuffers.data(), zeros.data(), zeros.data());

		if (m_IndexBuffer != nullptr)
		{
			m_IndexBuffer->Unbind();
		}
	}

	void D3D11VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		m_VertexBuffers.push_back(vertexBuffer);
		m_Buffers.push_back(static_cast<D3D11VertexBuffer*>(vertexBuffer.get())->GetBuffer());
		m_Strides.push_back(vertexBuffer->GetLayout().GetStride());
		m_Offsets.push_back(0);
	}

	void D3D11VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		m_IndexBuffer = indexBuffer;
	}

	const std::vector<std::shared_ptr<VertexBuffer>>& D3D11VertexArray::GetVertexBuffers() const
	{
		return m_VertexBuffers;
	}

	const std::shared_ptr<IndexBuffer>& D3D11VertexArray::GetIndexBuffer() const
	{
		return m_IndexBuffer;
	}
}
