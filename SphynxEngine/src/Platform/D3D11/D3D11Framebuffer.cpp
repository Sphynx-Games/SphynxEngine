#include "spxpch.h"
#include "D3D11Framebuffer.h"

#include <d3d11.h>
#include "D3D11RendererAPI.h"
#include "D3D11Utils.h"
#include "D3D11Assert.h"


namespace Sphynx
{
	static const uint32_t s_MaxFramebufferSize = 8192;

	namespace Utils
	{
		static bool IsDepthFormat(FramebufferTextureFormat format)
		{
			switch (format)
			{
			case FramebufferTextureFormat::DEPTH24STENCIL8:		return true;
			}

			return false;
		}

		static DXGI_FORMAT FBTextureFormatToD3D11(FramebufferTextureFormat format)
		{
			switch (format)
			{
			case FramebufferTextureFormat::RGBA8:				return DXGI_FORMAT_R8G8B8A8_UNORM;
			case FramebufferTextureFormat::RED_INTEGER:			return DXGI_FORMAT_R8_UNORM;
			case FramebufferTextureFormat::DEPTH24STENCIL8:		return DXGI_FORMAT_D24_UNORM_S8_UINT;
			}

			return (DXGI_FORMAT)0;
		}
	}

	D3D11Framebuffer::D3D11Framebuffer(const FramebufferSpecification& spec) :
		m_Specification(spec),
		m_ColorAttachmentSpecifications(),
		m_DepthAttachmentSpecification(FramebufferTextureFormat::None),
		m_ColorAttachments(),
		m_DepthAttachment(nullptr),
		m_DepthAttachmentResource(nullptr)
	{
		for (const auto& spec : m_Specification.Attachments.Attachments)
		{
			if (!Utils::IsDepthFormat(spec.TextureFormat))
				m_ColorAttachmentSpecifications.emplace_back(spec);
			else
				m_DepthAttachmentSpecification = spec;
		}

		Invalidate();
	}

	D3D11Framebuffer::~D3D11Framebuffer()
	{
		for (ID3D11RenderTargetView* RTV : m_ColorAttachments)
		{
			ID3D11Resource* resource = nullptr;
			RTV->GetResource(&resource);
			resource->Release();
			RTV->Release();
		}
		m_ColorAttachments.clear();

		if (m_DepthAttachment != nullptr)
		{
			ID3D11Resource* resource = nullptr;
			m_DepthAttachment->GetResource(&resource);
			resource->Release();
			m_DepthAttachment->Release();
			m_DepthAttachment = nullptr;
		}
	}

	void D3D11Framebuffer::Invalidate()
	{
		ID3D11Device* device = GetD3D11Device();

		for (ID3D11ShaderResourceView* SRV : m_ColorAttachmentResources)
		{
			ID3D11Resource* resource = nullptr;
			SRV->GetResource(&resource);
			resource->Release();
			SRV->Release();
		}
		m_ColorAttachmentResources.clear();

		for (ID3D11RenderTargetView* RTV : m_ColorAttachments)
		{
			ID3D11Resource* resource = nullptr;
			RTV->GetResource(&resource);
			resource->Release();
			RTV->Release();
		}
		m_ColorAttachmentResources.clear();

		if (m_DepthAttachment != nullptr)
		{
			ID3D11Resource* resource = nullptr;
			m_DepthAttachment->GetResource(&resource);
			resource->Release();
			m_DepthAttachment->Release();
			m_DepthAttachment = nullptr;
		}

		if (m_DepthAttachmentResource != nullptr)
		{
			ID3D11Resource* resource = nullptr;
			m_DepthAttachmentResource->GetResource(&resource);
			resource->Release();
			m_DepthAttachmentResource->Release();
			m_DepthAttachmentResource = nullptr;
		}

		// Attachments
		m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());
		m_ColorAttachmentResources.resize(m_ColorAttachmentSpecifications.size());
		for (size_t i = 0; i < m_ColorAttachmentSpecifications.size(); i++)
		{
			FramebufferTextureSpecification& spec = m_ColorAttachmentSpecifications[i];

			ID3D11Texture2D* renderTargetTexture = nullptr;
			D3D11_TEXTURE2D_DESC textureDesc{};
			textureDesc.Width = m_Specification.Width;
			textureDesc.Height = m_Specification.Height;
			textureDesc.MipLevels = spec.MipLevels;
			textureDesc.ArraySize = 1; // Single texture
			textureDesc.Format = Utils::FBTextureFormatToD3D11(spec.TextureFormat);
			textureDesc.SampleDesc.Count = m_Specification.Samples;
			textureDesc.Usage = D3D11_USAGE_DEFAULT; // Default usage
			textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE; // Render target and possibly as a texture later

			CHECK_D3D11_ERRORS(device->CreateTexture2D(&textureDesc, nullptr, &renderTargetTexture));
			CHECK_D3D11_ERRORS(device->CreateRenderTargetView(renderTargetTexture, nullptr, &m_ColorAttachments[i]));
			CHECK_D3D11_ERRORS(device->CreateShaderResourceView(renderTargetTexture, nullptr, &m_ColorAttachmentResources[i]));
		}

		if (m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None)
		{
			ID3D11Texture2D* depthStencilTexture = nullptr;
			D3D11_TEXTURE2D_DESC depthStencilDesc{};
			depthStencilDesc.Width = m_Specification.Width;
			depthStencilDesc.Height = m_Specification.Height;
			depthStencilDesc.MipLevels = m_DepthAttachmentSpecification.MipLevels;
			depthStencilDesc.ArraySize = 1;
			depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
			depthStencilDesc.SampleDesc.Count = 1;
			depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
			depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

			CHECK_D3D11_ERRORS(device->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencilTexture));

			D3D11_DEPTH_STENCIL_VIEW_DESC dsDesc{};
			dsDesc.Format = Utils::FBTextureFormatToD3D11(m_DepthAttachmentSpecification.TextureFormat);
			dsDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			dsDesc.Texture2D.MipSlice = 0;
			dsDesc.Flags = 0;

			CHECK_D3D11_ERRORS(device->CreateDepthStencilView(depthStencilTexture, &dsDesc, &m_DepthAttachment));

			D3D11_SHADER_RESOURCE_VIEW_DESC srDesc{};
			srDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
			srDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srDesc.Texture2D.MostDetailedMip = 0;
			srDesc.Texture2D.MipLevels = -1;
			CHECK_D3D11_ERRORS(device->CreateShaderResourceView(depthStencilTexture, &srDesc, &m_DepthAttachmentResource));
		}
	}

	void D3D11Framebuffer::Bind()
	{
		ID3D11DeviceContext* deviceContext = GetD3D11DeviceContext();
		deviceContext->OMSetRenderTargets((UINT)m_ColorAttachments.size(), m_ColorAttachments.data(), m_DepthAttachment);
	}

	void D3D11Framebuffer::Unbind()
	{
		ID3D11DeviceContext* deviceContext = GetD3D11DeviceContext();
		deviceContext->OMSetRenderTargets(0, nullptr, nullptr);
		// TODO: restore previous render targets (not nullptr)
	}

	void D3D11Framebuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
		{
			SPX_CORE_LOG_ERROR("Attempted to resize framebuffer to {}, {}", width, height);
			return;
		}
		m_Specification.Width = width;
		m_Specification.Height = height;

		Invalidate();
	}

	Color D3D11Framebuffer::ReadPixel(uint32_t attachmentIndex, uint32_t x, uint32_t y)
	{
		if (attachmentIndex >= m_ColorAttachments.size())
		{
			__debugbreak();
		}

		// TODO
		int pixelData = 0;
		return Color::FromHex(pixelData);
	}

	void D3D11Framebuffer::ClearAttachment(uint32_t attachmentIndex, uint32_t value)
	{
		if (attachmentIndex >= m_ColorAttachments.size())
		{
			__debugbreak();
		}

		const float clearColor[4]
		{ 
			(value >> 24) / 255.0f, 
			((value >> 16) & 0xFF) / 255.0f, 
			((value >> 8) & 0xFF) / 255.0f, 
			(value & 0xFF) / 255.0f 
		}; // RGBA

		ID3D11DeviceContext* deviceContext = GetD3D11DeviceContext();
		deviceContext->ClearRenderTargetView(m_ColorAttachments[attachmentIndex], clearColor);

		// TODO: clear depth buffer
		//deviceContext->ClearDepthStencilView(m_DepthAttachment, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u);
	}
}
