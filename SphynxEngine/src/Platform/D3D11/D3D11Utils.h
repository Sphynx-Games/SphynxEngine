#pragma once

#include "Core/Application.h"
#include "Renderer/Window.h"
#include "Renderer/Buffer.h"
#include "Renderer/Renderer2D.h"
#include "D3D11GraphicsContext.h"
#include <d3d11.h>


namespace Sphynx
{
	class D3D11RendererAPI;

	inline static DXGI_FORMAT DataTypeToDGXIFormat(DataType type)
	{
		switch (type)
		{
		case DataType::FLOAT:		return DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
		case DataType::FLOAT2:		return DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
		case DataType::FLOAT3:		return DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
		case DataType::FLOAT4:		return DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
		case DataType::INT:			return DXGI_FORMAT::DXGI_FORMAT_R32_SINT;
		case DataType::INT2:		return DXGI_FORMAT::DXGI_FORMAT_R32_SINT;
		case DataType::INT3:		return DXGI_FORMAT::DXGI_FORMAT_R32_SINT;
		case DataType::INT4:		return DXGI_FORMAT::DXGI_FORMAT_R32_SINT;
		case DataType::MAT3:		return DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
		case DataType::MAT4:		return DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
		case DataType::BOOL:		return DXGI_FORMAT::DXGI_FORMAT_R8_UINT;
		}
		return DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	}

	inline static D3D11GraphicsContext* GetD3D11GraphicsContext()
	{
		return static_cast<D3D11GraphicsContext*>(Application::GetInstance()->GetWindow()->GetGraphicsContext());
	}

	inline static ID3D11DeviceContext* GetD3D11DeviceContext()
	{
		return GetD3D11GraphicsContext()->GetContext();
	}

	inline static ID3D11Device* GetD3D11Device()
	{
		return GetD3D11GraphicsContext()->GetDevice();
	}

	inline static IDXGISwapChain* GetD3D11SwapChain()
	{
		return GetD3D11GraphicsContext()->GetSwapChain();
	}

	inline static ID3D11RenderTargetView* GetD3D11RenderTargetView()
	{
		return GetD3D11GraphicsContext()->GetRenderTargetView();
	}
}
