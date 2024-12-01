#pragma once

#include "Core/Core.h"
#include "Renderer/GraphicsContext.h"


struct ID3D11Device;
struct IDXGISwapChain;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;

namespace Sphynx
{
	class SPHYNX_API D3D11GraphicsContext : public GraphicsContext
	{
	public:
		D3D11GraphicsContext(Window* window);
		~D3D11GraphicsContext();

		virtual void Init() override;
		virtual void Shutdown() override;

		virtual void SwapBuffers() override;

	public:
		ID3D11Device* GetDevice() const;
		IDXGISwapChain* GetSwapChain() const;
		ID3D11DeviceContext* GetContext() const;
		ID3D11RenderTargetView* GetRenderTargetView() const;

	private:
		ID3D11Device* m_Device;
		IDXGISwapChain* m_SwapChain;
		ID3D11DeviceContext* m_Context;
		ID3D11RenderTargetView* m_RenderTargetView;

	};
}
