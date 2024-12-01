#include "spxpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/D3D11/D3D11Shader.h"


namespace Sphynx
{
	Shader* Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE:	return nullptr;
		case RendererAPI::API::OPENGL:	return new OpenGLShader(filepath);
		case RendererAPI::API::D3D11:	return new D3D11Shader(filepath);
		}
		return nullptr;
	}

	Shader* Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE:	return nullptr;
		case RendererAPI::API::OPENGL:	return new OpenGLShader(name, vertexSrc, fragSrc);
		case RendererAPI::API::D3D11:	return new D3D11Shader(name, vertexSrc, fragSrc);
		}
		return nullptr;
	}

	void ShaderLibrary::Add(const std::string& name, const std::shared_ptr<Shader>& shader)
	{
		if (Exists(name))
		{
			SPX_CORE_LOG_ERROR("Shader already exists!");
		}

		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(const std::shared_ptr<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}

	std::shared_ptr<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		std::shared_ptr<Shader> shaderRef;
		shaderRef.reset(shader);
		Add(shaderRef);
		return shaderRef;
	}

	std::shared_ptr<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		std::shared_ptr<Shader> shaderRef;
		shaderRef.reset(shader);
		Add(name, shaderRef);
		return shaderRef;
	}

	std::shared_ptr<Shader> ShaderLibrary::Get(const std::string& name)
	{
		if (!Exists(name))
		{
			SPX_CORE_LOG_ERROR("Shader not found!");
		}
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}
