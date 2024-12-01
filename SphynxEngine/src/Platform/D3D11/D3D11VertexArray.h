#pragma once

#include "Core/Core.h"
#include "Renderer/VertexArray.h"
#include <d3d11.h>


namespace Sphynx
{
	class D3D11VertexArray : public VertexArray
	{
	public:
		D3D11VertexArray();
		virtual ~D3D11VertexArray() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override;
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override;

	private:
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;

		std::vector<struct ID3D11Buffer*> m_Buffers;
		std::vector<UINT> m_Strides;
		std::vector<UINT> m_Offsets;
	};
}
