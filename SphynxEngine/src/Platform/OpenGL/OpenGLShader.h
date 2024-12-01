#pragma once

#include "Core/Core.h"
#include "Renderer/Shader.h"

#include <glm/glm.hpp>
#include <glad/glad.h>


namespace Sphynx
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragSrc);
		virtual ~OpenGLShader() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetInt(const std::string& name, int value) const override;
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
		void UploadUniformInt(const std::string& name, int value) const;
		void UploadUniformInt2(const std::string& name, const glm::ivec2& value) const;
		void UploadUniformInt3(const std::string& name, const glm::ivec3& value) const;
		void UploadUniformInt4(const std::string& name, const glm::ivec4& value) const;

		void UploadUniformFloat(const std::string& name, float value) const;
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value) const;
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value) const;
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value) const;

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix) const;
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const;

	private:
		std::string ReadFile(const std::string& filepath) const;
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source) const;
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);


	private:
		uint32_t m_RendererID;
		std::string m_Name;
	};
}
