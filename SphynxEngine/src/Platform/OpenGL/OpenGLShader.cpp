#include "OpenGLShader.h"

#include <vector>
#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "OpenGLAssert.h"


namespace Sphynx
{
	namespace Utils
	{
		static GLenum ShaderTypeFromString(const std::string& type)
		{
			if (type == "vertex")
				return GL_VERTEX_SHADER;
			if (type == "geometry" || type == "geo")
				return GL_GEOMETRY_SHADER;
			if (type == "fragment" || type == "pixel")
				return GL_FRAGMENT_SHADER;

			SPX_CORE_LOG_ERROR("Unknown shader type '{}'", type.c_str());
			return 0;
		}
	}

	OpenGLShader::OpenGLShader(const std::string& filepath) :
		m_RendererID(0)
	{
		std::string source = ReadFile(filepath);
		auto sources = PreProcess(source);
		Compile(sources);

		// Extract name from filepath
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragSrc) :
		m_RendererID(0),
		m_Name(name)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragSrc;

		Compile(sources);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string& name, int value) const
	{
		UploadUniformInt(name, value);
	}

	void OpenGLShader::SetInt2(const std::string& name, const Vector2i& value) const
	{
		UploadUniformInt2(name, { value.X, value.Y });
	}

	void OpenGLShader::SetInt3(const std::string& name, const Vector3i& value) const
	{
		UploadUniformInt3(name, { value.X, value.Y, value.Z });
	}

	void OpenGLShader::SetInt4(const std::string& name, const Vector4i& value) const
	{
		UploadUniformInt4(name, { value.X, value.Y, value.Z, value.W });
	}

	void OpenGLShader::SetFloat(const std::string& name, float value) const
	{
		UploadUniformFloat(name, value);
	}

	void OpenGLShader::SetFloat2(const std::string& name, const Vector2f& value) const
	{
		UploadUniformFloat2(name, { value.X, value.Y });
	}

	void OpenGLShader::SetFloat3(const std::string& name, const Vector3f& value) const
	{
		UploadUniformFloat3(name, { value.X, value.Y, value.Z });
	}

	void OpenGLShader::SetFloat4(const std::string& name, const Vector4f& value) const
	{
		UploadUniformFloat4(name, { value.X, value.Y, value.Z, value.W });
	}

	void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& matrix) const
	{
		UploadUniformMat3(name, matrix);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& matrix) const
	{
		UploadUniformMat4(name, matrix);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int value) const
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location < 0)
			return;

		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformInt2(const std::string& name, const glm::ivec2& value) const
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location < 0)
			return;

		glUniform2iv(location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::UploadUniformInt3(const std::string& name, const glm::ivec3& value) const
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location < 0)
			return;

		glUniform3iv(location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::UploadUniformInt4(const std::string& name, const glm::ivec4& value) const
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location < 0)
			return;

		glUniform4iv(location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float value) const
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location < 0)
			return;

		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value) const
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location < 0)
			return;

		glUniform2fv(location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value) const
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location < 0)
			return;

		glUniform3fv(location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value) const
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location < 0)
			return;

		glUniform4fv(location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix) const
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location < 0)
			return;

		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location < 0)
			return;

		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath) const
	{
		std::ifstream in(filepath, std::ios::in, std::ios::binary);
		std::string result;
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.length());
			in.close();
		}
		else
		{
			SPX_CORE_LOG_ERROR("Could not open file: {}", filepath.c_str());
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source) const
	{
		std::unordered_map<GLenum, std::string> shaderSources;
		const char* typeToken = "#type";

		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0); //Start of shader type declaration line
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
			if (eol == std::string::npos)
			{
				SPX_CORE_LOG_ERROR("Syntax error");
			}
			size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
			std::string type = source.substr(begin, eol - begin);
			if (!Utils::ShaderTypeFromString(type))
			{
				SPX_CORE_LOG_ERROR("Invalid shader type specified");
			}

			size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
			if (nextLinePos == std::string::npos)
			{
				SPX_CORE_LOG_ERROR("Syntax error");
			}
			pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line

			shaderSources[Utils::ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		if (shaderSources.size() == 0) return;

		GLuint program = glCreateProgram(); CHECK_GL_ERRORS;
		std::vector<GLenum> glShaderIDs;
		glShaderIDs.reserve(shaderSources.size());

		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;

			// create vertex shader handle
			GLuint shader = glCreateShader(type); CHECK_GL_ERRORS;

			// send source code to GL and compile
			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, NULL); CHECK_GL_ERRORS;
			glCompileShader(shader); CHECK_GL_ERRORS;

			// check compilation state
			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled); CHECK_GL_ERRORS;
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength); CHECK_GL_ERRORS;

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]); CHECK_GL_ERRORS;

				// We don't need the shader anymore.
				glDeleteShader(shader); CHECK_GL_ERRORS;

				SPX_CORE_LOG_ERROR("Shader compilation failure!");
				SPX_CORE_LOG_ERROR("{}", infoLog.data());
				break;
			}
			glAttachShader(program, shader); CHECK_GL_ERRORS;
			glShaderIDs.push_back(shader);
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		m_RendererID = program;

		// Link our program
		glLinkProgram(program); CHECK_GL_ERRORS;

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked); CHECK_GL_ERRORS;
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength); CHECK_GL_ERRORS;

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]); CHECK_GL_ERRORS;

			// We don't need the program anymore.
			glDeleteProgram(program); CHECK_GL_ERRORS;
			// Don't leak shaders either.
			for (auto id : glShaderIDs)
			{
				glDeleteShader(id); CHECK_GL_ERRORS;
			}

			SPX_CORE_LOG_ERROR("Shader link failure!");
			SPX_CORE_LOG_ERROR("{}", infoLog.data());
			return;
		}

		// Always detach shaders after a successful link.
		for (auto id : glShaderIDs)
		{
			glDetachShader(program, id); CHECK_GL_ERRORS;
		}
	}
}
