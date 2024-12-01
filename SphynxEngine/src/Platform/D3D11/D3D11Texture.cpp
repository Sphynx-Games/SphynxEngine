#include "spxpch.h"
#include "D3D11Texture.h"

#include <d3d11.h>
#include "D3D11RendererAPI.h"
#include "D3D11Utils.h"
#include "D3D11Assert.h"

#include <stb_image.h>


namespace Sphynx
{
	D3D11Texture::D3D11Texture(const std::string& path) :
		m_Texture(nullptr),
		m_TextureResourceView(nullptr),
		m_Path(path),
		m_Width(0),
		m_Height(0)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

		if (data == nullptr)
		{
			SPX_CORE_LOG_ERROR("Failed to load image {}", path.c_str());
			return;
		}

		m_Width = width;
		m_Height = height;

		DXGI_FORMAT format = channels == 4 || channels == 3 ? DXGI_FORMAT_R8G8B8A8_UNORM : DXGI_FORMAT_R8_UNORM;

		D3D11_TEXTURE2D_DESC textureDesc{};
		textureDesc.Width = m_Width;
		textureDesc.Height = m_Height;
		textureDesc.MipLevels = 1; // Single Mip Level
		textureDesc.ArraySize = 1; // Single texture
		textureDesc.Format = format;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT; // Default usage
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		ID3D11Device* device = GetD3D11Device();
		CHECK_D3D11_ERRORS(device->CreateTexture2D(&textureDesc, nullptr, &m_Texture));

		stbi_image_free(data);
	}

	D3D11Texture::D3D11Texture(uint32_t width, uint32_t height) :
		m_Texture(nullptr),
		m_TextureResourceView(nullptr),
		m_Path(),
		m_Width(width),
		m_Height(height)
	{
		D3D11_TEXTURE2D_DESC textureDesc{};
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1; // Single Mip Level
		textureDesc.ArraySize = 1; // Single texture
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT; // Default usage
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		ID3D11Device* device = GetD3D11Device();
		CHECK_D3D11_ERRORS(device->CreateTexture2D(&textureDesc, nullptr, &m_Texture));
	}

	D3D11Texture::D3D11Texture(void* data, uint32_t width, uint32_t height) :
		m_Texture(nullptr),
		m_TextureResourceView(nullptr),
		m_Path(),
		m_Width(width),
		m_Height(height)
	{
		D3D11_TEXTURE2D_DESC textureDesc{};
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1; // Single Mip Level
		textureDesc.ArraySize = 1; // Single texture
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT; // Default usage
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		D3D11_SUBRESOURCE_DATA subData{};
		subData.pSysMem = data;
		subData.SysMemPitch = 4 * width;
		subData.SysMemSlicePitch = 0;

		ID3D11Device* device = GetD3D11Device();
		CHECK_D3D11_ERRORS(device->CreateTexture2D(&textureDesc, &subData, &m_Texture));
	}

	D3D11Texture::~D3D11Texture()
	{
		if (m_Texture != nullptr)
		{
			m_Texture->Release();
			m_Texture = nullptr;
		}

		if (m_TextureResourceView != nullptr)
		{
			m_TextureResourceView->Release();
			m_TextureResourceView = nullptr;
		}
	}

	/*void D3D11Texture::SetData(void* data)
	{
		ID3D11DeviceContext* deviceContext = GetD3D11DeviceContext();
		deviceContext->UpdateSubresource(m_Texture, 0, nullptr, data, m_Width * 4, 0);
	}

	void D3D11Texture::Bind(uint32_t slot) const
	{
		ID3D11DeviceContext* deviceContext = GetD3D11DeviceContext();
		deviceContext->VSSetShaderResources(slot, 1, &m_TextureResourceView);
		deviceContext->PSSetShaderResources(slot, 1, &m_TextureResourceView);
	}*/

	uint32_t D3D11Texture::GetWidth() const
	{
		return m_Width;
	}

	uint32_t D3D11Texture::GetHeight() const
	{
		return m_Height;
	}

	void D3D11Texture::GenerateResource()
	{
		if (m_TextureResourceView != nullptr)
		{
			m_TextureResourceView->Release();
			m_TextureResourceView = nullptr;
		}

		D3D11_TEXTURE2D_DESC textureDesc{};
		m_Texture->GetDesc(&textureDesc);

		D3D11_SHADER_RESOURCE_VIEW_DESC desc{};
		desc.Format = textureDesc.Format;
		desc.Texture2D.MipLevels = 1;
		desc.Texture2D.MostDetailedMip = 0;

		ID3D11Device* device = GetD3D11Device();
		CHECK_D3D11_ERRORS(device->CreateShaderResourceView(m_Texture, &desc, &m_TextureResourceView));
	}
}
