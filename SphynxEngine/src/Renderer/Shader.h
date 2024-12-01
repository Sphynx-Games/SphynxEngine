#pragma once

#include "Core/Core.h"
#include "string"
#include <memory>
#include <glm/glm.hpp>
#include <unordered_map>
#include "Math/Vector.h"


namespace Sphynx
{
	class SPHYNX_API Shader
	{
	public:
		virtual ~Shader() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetInt(const std::string& name, int32_t value) const = 0;
		virtual void SetInt2(const std::string& name, const Vector2i& value) const = 0;
		virtual void SetInt3(const std::string& name, const Vector3i& value) const = 0;
		virtual void SetInt4(const std::string& name, const Vector4i& value) const = 0;

		virtual void SetFloat(const std::string& name, float value) const = 0;
		virtual void SetFloat2(const std::string& name, const Vector2f& value) const = 0;
		virtual void SetFloat3(const std::string& name, const Vector3f& value) const = 0;
		virtual void SetFloat4(const std::string& name, const Vector4f& value) const = 0;

		virtual void SetMat3(const std::string& name, const glm::mat3& matrix) const = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& matrix) const = 0;

		virtual const std::string& GetName() const = 0;

		static Shader* Create(const std::string& filepath);
		static Shader* Create(const std::string& name, const std::string& vertexSrc, const std::string& fragSrc);
	};

	class ShaderLibrary
	{
	public:
		void Add(const std::string& name, const std::shared_ptr<Shader>& shader);
		void Add(const std::shared_ptr<Shader>& shader);

		std::shared_ptr<Shader> Load(const std::string& filepath);
		std::shared_ptr<Shader> Load(const std::string& name, const std::string& filepath);

		std::shared_ptr<Shader> Get(const std::string& name);

		bool Exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;
	};
}
