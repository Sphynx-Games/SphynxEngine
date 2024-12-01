#pragma once

#include "Core/Core.h"
#include "Renderer/RendererAPI.h"


namespace Sphynx
{
	class SPHYNX_API D3D11RendererAPI : public RendererAPI
	{
	public:
		D3D11RendererAPI();
		virtual ~D3D11RendererAPI() override;

		virtual void Init() override;
		virtual void Shutdown() override;

		//virtual void OnWindowResized(uint32_t width, uint32_t height) override;

		//virtual void SetClearColor(float r, float g, float b, float a) const override;
		//virtual void Clear() const override;

		//virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) const override;

		//virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const override;

	private:
		mutable float m_ClearColor[4];

	};
}
