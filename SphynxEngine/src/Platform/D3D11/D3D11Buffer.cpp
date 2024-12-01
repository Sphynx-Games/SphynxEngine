#include "spxpch.h"
#include "D3D11Buffer.h"

#include "D3D11Utils.h"
#include "D3D11Assert.h"

#include <d3d11.h>


namespace Sphynx
{
	D3D11VertexBuffer::D3D11VertexBuffer(void* vertices, size_t size, const BufferLayout& layout) :
		m_Data(vertices),
		m_Size(size),
		m_Layout(layout),
		m_VertexBuffer(nullptr)
	{
		Invalidate();
	}

	D3D11VertexBuffer::~D3D11VertexBuffer()
	{
		if (m_VertexBuffer != nullptr)
		{
			m_VertexBuffer->Release();
			m_VertexBuffer = nullptr;
		}
	}

	void D3D11VertexBuffer::Bind() const
	{
		const UINT stride = m_Layout.GetStride();
		const UINT offset = 0;

		ID3D11DeviceContext* context = GetD3D11DeviceContext();
		ID3D11Buffer *const * buffer = &m_VertexBuffer;

		context->IASetVertexBuffers(0u, 1u, buffer, &stride, &offset);
	}

	void D3D11VertexBuffer::Unbind() const
	{
		ID3D11Buffer *const buffer = nullptr;
		const UINT stride = 0;
		const UINT offset = 0;

		ID3D11DeviceContext* context = GetD3D11DeviceContext();
		context->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
	}

	const BufferLayout& D3D11VertexBuffer::GetLayout() const
	{
		return m_Layout;
	}

	void D3D11VertexBuffer::SetLayout(const BufferLayout& layout)
	{
		m_Layout = layout;
		Invalidate();
	}

	ID3D11Buffer* D3D11VertexBuffer::GetBuffer() const
	{
		return m_VertexBuffer;
	}

	void D3D11VertexBuffer::Invalidate()
	{
		if (m_VertexBuffer != nullptr)
		{
			m_VertexBuffer->Release();
			m_VertexBuffer = nullptr;
		}

		D3D11_BUFFER_DESC desc{};
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = (UINT)m_Size;
		desc.StructureByteStride = m_Layout.GetStride();
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA initData{};
		initData.pSysMem = m_Data;

		ID3D11Device* device = GetD3D11Device();
		CHECK_D3D11_ERRORS(device->CreateBuffer(&desc, &initData, &m_VertexBuffer));
	}

	D3D11IndexBuffer::D3D11IndexBuffer(uint32_t* indices, uint32_t count) :
		m_Count(count)
	{
		D3D11_BUFFER_DESC desc{};
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = count * sizeof(uint32_t);
		desc.StructureByteStride = sizeof(uint32_t);
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA initData{};
		initData.pSysMem = indices;

		ID3D11Device* device = GetD3D11Device();
		CHECK_D3D11_ERRORS(device->CreateBuffer(&desc, &initData, &m_IndexBuffer));
	}

	D3D11IndexBuffer::~D3D11IndexBuffer()
	{
		if (m_IndexBuffer != nullptr)
		{
			m_IndexBuffer->Release();
			m_IndexBuffer = nullptr;
		}
	}

	void D3D11IndexBuffer::Bind() const
	{
		ID3D11DeviceContext* context = GetD3D11DeviceContext();
		context->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	}

	void D3D11IndexBuffer::Unbind() const
	{
		ID3D11DeviceContext* context = GetD3D11DeviceContext();
		context->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0);
	}

	uint32_t D3D11IndexBuffer::GetCount() const
	{
		return m_Count;
	}
}
