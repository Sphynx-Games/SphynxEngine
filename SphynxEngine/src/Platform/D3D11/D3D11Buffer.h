#pragma once

#include "Core/Core.h"
#include "Renderer/Buffer.h"


struct ID3D11Buffer;

namespace Sphynx
{
	class SPHYNX_API D3D11VertexBuffer : public VertexBuffer
	{
	public:
		D3D11VertexBuffer(void* vertices, size_t size, const BufferLayout& layout);
		virtual ~D3D11VertexBuffer() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const BufferLayout& GetLayout() const override;
		virtual void SetLayout(const BufferLayout& layout) override;

		ID3D11Buffer* GetBuffer() const;

	private:
		void Invalidate();

	private:
		void* m_Data;
		size_t m_Size;
		BufferLayout m_Layout;

		ID3D11Buffer* m_VertexBuffer;
	};

	class SPHYNX_API D3D11IndexBuffer : public IndexBuffer
	{
	public:
		D3D11IndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~D3D11IndexBuffer() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetCount() const override;

	private:
		ID3D11Buffer* m_IndexBuffer;
		uint32_t m_Count;
	};
}