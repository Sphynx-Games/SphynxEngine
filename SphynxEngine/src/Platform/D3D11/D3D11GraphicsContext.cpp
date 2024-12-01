#include "spxpch.h"
#include "D3D11GraphicsContext.h"

#include <dxgi.h>

#include "Platform/Windows/WindowsWindow.h"

#include "D3D11RendererAPI.h"
#include "D3D11Utils.h"
#include "D3D11Assert.h"



namespace Sphynx
{
	D3D11GraphicsContext::D3D11GraphicsContext(Window* window) :
		GraphicsContext(window),
		m_Device(nullptr),
		m_SwapChain(nullptr),
		m_Context(nullptr),
		m_RenderTargetView(nullptr)
	{
	}

	D3D11GraphicsContext::~D3D11GraphicsContext()
	{
	}

	void D3D11GraphicsContext::Init()
	{
		const WindowsWindow* window = static_cast<WindowsWindow*>(m_Window);

		// Create swap chain
		DXGI_SWAP_CHAIN_DESC desc{};
		desc.BufferCount = 1;
		desc.BufferDesc.Width = window->GetWidth();
		desc.BufferDesc.Height = window->GetHeight();
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.RefreshRate.Numerator = 60; // 60 Hz
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.OutputWindow = window->GetHWND();
		desc.Windowed = true;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		desc.Flags = 0;

		D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			D3D11_CREATE_DEVICE_DEBUG,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&desc,
			&m_SwapChain,
			&m_Device,
			nullptr,
			&m_Context);

		// Create RenderTarget
		ID3D11Texture2D* backBuffer = nullptr;
		CHECK_D3D11_ERRORS(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer));
		CHECK_D3D11_ERRORS(m_Device->CreateRenderTargetView(backBuffer, nullptr, &m_RenderTargetView));
		backBuffer->Release();

		m_Context->OMSetRenderTargets(1, &m_RenderTargetView, nullptr);
	}

	void D3D11GraphicsContext::Shutdown()
	{
		if (m_RenderTargetView != nullptr)
		{
			m_RenderTargetView->Release();
			m_RenderTargetView = nullptr;
		}

		if (m_SwapChain != nullptr)
		{
			m_SwapChain->Release();
			m_SwapChain = nullptr;
		}

		if (m_Context != nullptr)
		{
			m_Context->Release();
			m_Context = nullptr;
		}

		if (m_Device != nullptr)
		{
			m_Device->Release();
			m_Device = nullptr;
		}
	}

	void D3D11GraphicsContext::SwapBuffers()
	{
		IDXGISwapChain* swapChain = GetD3D11SwapChain();
		swapChain->Present((UINT)m_Window->IsVSync(), 0);
	}

	ID3D11Device* D3D11GraphicsContext::GetDevice() const
	{
		return m_Device;
	}

	IDXGISwapChain* D3D11GraphicsContext::GetSwapChain() const
	{
		return m_SwapChain;
	}

	ID3D11DeviceContext* D3D11GraphicsContext::GetContext() const
	{
		return m_Context;
	}

	ID3D11RenderTargetView* D3D11GraphicsContext::GetRenderTargetView() const
	{
		return m_RenderTargetView;
	}

}
