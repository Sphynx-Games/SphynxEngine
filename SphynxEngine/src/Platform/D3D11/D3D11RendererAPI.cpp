#include "spxpch.h"
#include "D3D11RendererAPI.h"
#include "Core/Application.h"
#include "Renderer/Window.h"

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include "D3D11Assert.h"


namespace Sphynx
{
	D3D11RendererAPI::D3D11RendererAPI() :
		m_ClearColor{ 1.0f, 1.0f, 1.0f, 1.0f }
	{
	}

	D3D11RendererAPI::~D3D11RendererAPI()
	{
	}

	void D3D11RendererAPI::Init()
	{
		const Window* window = Application::GetInstance()->GetWindow();
		SetViewport(Vector2i::Zero, Vector2i{ (int32_t)window->GetWidth(), (int32_t)window->GetHeight() });
	}

	void D3D11RendererAPI::Shutdown()
	{
		
	}

	//void D3D11RendererAPI::OnWindowResized(uint32_t width, uint32_t height)
	//{
	//	ID3D11RenderTargetView* renderTarget = nullptr;
	//	m_Context->OMGetRenderTargets(1, &renderTarget, nullptr);
	//	bool isMainRenderTarget = renderTarget == m_RenderTargetView;
	//	renderTarget->Release();

	//	if (isMainRenderTarget)
	//	{
	//		m_Context->OMSetRenderTargets(0, nullptr, nullptr);
	//	}

	//	if (m_RenderTargetView != nullptr)
	//	{
	//		m_RenderTargetView->Release();
	//		m_RenderTargetView = nullptr;
	//	}

	//	// Resize the swap chain buffers
	//	CHECK_D3D11_ERRORS(m_SwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_UNKNOWN, 0));

	//	// Recreate the back buffer
	//	ID3D11Texture2D* backBuffer = nullptr;
	//	CHECK_D3D11_ERRORS(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer));
	//	CHECK_D3D11_ERRORS(m_Device->CreateRenderTargetView(backBuffer, nullptr, &m_RenderTargetView));
	//	backBuffer->Release();

	//	if (isMainRenderTarget)
	//	{
	//		m_Context->OMSetRenderTargets(1, &m_RenderTargetView, nullptr);
	//	}
	//}

	//void D3D11RendererAPI::SetClearColor(float r, float g, float b, float a) const
	//{
	//	m_ClearColor[0] = r;
	//	m_ClearColor[1] = g;
	//	m_ClearColor[2] = b;
	//	m_ClearColor[3] = a;
	//}

	//void D3D11RendererAPI::Clear() const
	//{
	//	ID3D11RenderTargetView* renderTargetViews[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT]{ nullptr };
	//	ID3D11DepthStencilView* depthStencilView = nullptr;
	//	m_Context->OMGetRenderTargets(1, renderTargetViews, &depthStencilView);

	//	for (ID3D11RenderTargetView* view : renderTargetViews)
	//	{
	//		if (view == nullptr) continue;
	//		m_Context->ClearRenderTargetView(view, m_ClearColor);
	//		view->Release();
	//	}

	//	if (depthStencilView != nullptr)
	//	{
	//		m_Context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//		depthStencilView->Release();
	//	}
	//}

	//void D3D11RendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) const
	//{
	//	m_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//	m_Context->DrawIndexed((UINT)vertexArray->GetIndexBuffer()->GetCount(), 0u, 0u);
	//}

	//void D3D11RendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const
	//{
	//	D3D11_VIEWPORT viewport{};
	//	viewport.Width = (FLOAT)width;
	//	viewport.Height = (FLOAT)height;
	//	viewport.MinDepth = 0.0f;
	//	viewport.MaxDepth = 1.0f;
	//	viewport.TopLeftX = (FLOAT)x;
	//	viewport.TopLeftY = (FLOAT)y;

	//	m_Context->RSSetViewports(1, &viewport);
	//}
}
