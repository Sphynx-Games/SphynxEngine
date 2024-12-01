#pragma once

#include "Core/Core.h"
#include "Renderer/Shader.h"
#include <glm/glm.hpp>


struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;

namespace Sphynx
{
	class SPHYNX_API D3D11Shader : public Shader
	{
	public:
		D3D11Shader(const std::string& filepath);
		D3D11Shader(const std::string& name, const std::string& vertexSrc, const std::string& fragSrc);
		virtual ~D3D11Shader() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetInt(const std::string& name, int32_t value) const override;
		virtual void SetInt2(const std::string& name, const Vector2i& value) const override;
		virtual void SetInt3(const std::string& name, const Vector3i& value) const override;
		virtual void SetInt4(const std::string& name, const Vector4i& value) const override;

		virtual void SetFloat(const std::string& name, float value) const override;
		virtual void SetFloat2(const std::string& name, const Vector2f& value) const override;
		virtual void SetFloat3(const std::string& name, const Vector3f& value) const override;
		virtual void SetFloat4(const std::string& name, const Vector4f& value) const override;

		virtual void SetMat3(const std::string& name, const glm::mat3& matrix) const override;
		virtual void SetMat4(const std::string& name, const glm::mat4& matrix) const override;

		virtual const std::string& GetName() const override { return m_Name; };

	private:
		std::string ReadFile(const std::string& filepath) const;
		std::unordered_map<std::string, std::string> PreProcess(const std::string& source) const;
		void Compile(const std::unordered_map<std::string, std::string>& shaderSources);

	private:
		ID3D11VertexShader* m_VertexShader;
		ID3D11PixelShader* m_PixelShader;
		ID3D11InputLayout* m_InputLayout;

		std::string m_Name;
	};
}
