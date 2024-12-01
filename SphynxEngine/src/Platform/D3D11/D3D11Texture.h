#pragma once

#include "Core/Core.h"
#include "Renderer/Texture.h"


struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;

namespace Sphynx
{
	class SPHYNX_API D3D11Texture : public Texture
	{
	public:
		D3D11Texture(const std::string& path);
		D3D11Texture(uint32_t width, uint32_t height);
		D3D11Texture(void* data, uint32_t width, uint32_t height);
		virtual ~D3D11Texture() override;

		/*virtual void SetData(void* data) override;
		virtual void Bind(uint32_t slot = 0) const override;*/

		virtual uint32_t GetWidth() const override;
		virtual uint32_t GetHeight() const override;

	private:
		void GenerateResource();

	private:
		ID3D11Texture2D* m_Texture;
		ID3D11ShaderResourceView* m_TextureResourceView;
		std::string m_Path;
		uint32_t m_Width;
		uint32_t m_Height;
	};
}
