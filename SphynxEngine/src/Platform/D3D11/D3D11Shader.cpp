#include "spxpch.h"
#include "D3D11Shader.h"

#include "D3D11RendererAPI.h"
#include "D3D11Utils.h"
#include "D3D11Assert.h"

#include <vector>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")


namespace Sphynx
{
	namespace Utils
	{
		static LPCSTR EntryPointFromString(const std::string& type)
		{
			if (type == "vertex")
				return "VSMain";
			if (type == "geometry" || type == "geo")
				return "GSMain";
			if (type == "fragment" || type == "pixel")
				return "PSMain";

			SPX_CORE_LOG_ERROR("Unknown shader type '{}'", type.c_str());
			return 0;
		}

		static LPCSTR ShaderTargetFromString(const std::string& type)
		{
			if (type == "vertex")
				return "vs_5_0";
			if (type == "geometry" || type == "geo")
				return "gs_5_0";
			if (type == "fragment" || type == "pixel")
				return "ps_5_0";

			SPX_CORE_LOG_ERROR("Unknown shader type '{}'", type.c_str());
			return "";
		}
	}

	D3D11Shader::D3D11Shader(const std::string& filepath) :
		m_VertexShader(nullptr),
		m_PixelShader(nullptr),
		m_InputLayout(nullptr),
		m_Name()
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

	D3D11Shader::D3D11Shader(const std::string& name, const std::string& vertexSrc, const std::string& pixelSrc) :
		m_VertexShader(nullptr),
		m_PixelShader(nullptr),
		m_InputLayout(nullptr),
		m_Name()
	{
		std::unordered_map<std::string, std::string> sources;
		sources["vertex"] = vertexSrc;
		sources["pixel"] = pixelSrc;

		Compile(sources);
	}

	D3D11Shader::~D3D11Shader()
	{
		if (m_VertexShader != nullptr)
		{
			m_VertexShader->Release();
			m_VertexShader = nullptr;
		}

		if (m_PixelShader != nullptr)
		{
			m_PixelShader->Release();
			m_PixelShader = nullptr;
		}

		if (m_InputLayout != nullptr)
		{
			m_InputLayout->Release();
			m_InputLayout = nullptr;
		}
	}

	void D3D11Shader::Bind() const
	{
		ID3D11DeviceContext* deviceContext = GetD3D11DeviceContext();

		if (m_InputLayout != nullptr)
		{
			deviceContext->IASetInputLayout(m_InputLayout);
		}

		if (m_VertexShader != nullptr)
		{
			deviceContext->VSSetShader(m_VertexShader, nullptr, 0);
		}

		if (m_PixelShader != nullptr)
		{
			deviceContext->PSSetShader(m_PixelShader, nullptr, 0);
		}
	}

	void D3D11Shader::Unbind() const
	{
		// TODO: bind last binded items
		ID3D11DeviceContext* deviceContext = GetD3D11DeviceContext();
		deviceContext->IASetInputLayout(nullptr);
		deviceContext->VSSetShader(nullptr, nullptr, 0);
		deviceContext->PSSetShader(nullptr, nullptr, 0);
	}

	void D3D11Shader::SetInt(const std::string& name, int32_t value) const
	{

	}

	void D3D11Shader::SetInt2(const std::string& name, const Vector2i& value) const
	{

	}

	void D3D11Shader::SetInt3(const std::string& name, const Vector3i& value) const
	{

	}

	void D3D11Shader::SetInt4(const std::string& name, const Vector4i& value) const
	{

	}

	void D3D11Shader::SetFloat(const std::string& name, float value) const
	{

	}

	void D3D11Shader::SetFloat2(const std::string& name, const Vector2f& value) const
	{

	}

	void D3D11Shader::SetFloat3(const std::string& name, const Vector3f& value) const
	{

	}

	void D3D11Shader::SetFloat4(const std::string& name, const Vector4f& value) const
	{

	}

	void D3D11Shader::SetMat3(const std::string& name, const glm::mat3& matrix) const
	{

	}

	void D3D11Shader::SetMat4(const std::string& name, const glm::mat4& matrix) const
	{

	}

	std::string D3D11Shader::ReadFile(const std::string& filepath) const
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

	std::unordered_map<std::string, std::string> D3D11Shader::PreProcess(const std::string& source) const
	{
		std::unordered_map<std::string, std::string> shaderSources;
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
			/*if (!Utils::ShaderTypeFromString(type))
			{
				SPX_CORE_LOG_ERROR("Invalid shader type specified");
			}*/

			size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
			if (nextLinePos == std::string::npos)
			{
				SPX_CORE_LOG_ERROR("Syntax error");
			}
			pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line

			shaderSources[type] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	void D3D11Shader::Compile(const std::unordered_map<std::string, std::string>& shaderSources)
	{
		if (shaderSources.size() == 0) return;

		std::unordered_map<std::string, ID3DBlob*> shaderBlobs;
		for (auto& kv : shaderSources)
		{
			const std::string& type = kv.first;
			const std::string& source = kv.second;

			// create vertex shader handle
			ID3DBlob* errorBlob = nullptr;
			if (CHECK_D3D11_FAILED(D3DCompile(source.data(), source.size(), nullptr, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, Utils::EntryPointFromString(type), Utils::ShaderTargetFromString(type), 0, 0, &shaderBlobs[type], nullptr)))
			{
				// check compilation state
				if (errorBlob != nullptr)
				{
					OutputDebugStringA((char*)errorBlob->GetBufferPointer());
					errorBlob->Release();
					break;
				}
			}
		}

		// Vertex and fragment shaders are successfully compiled.
		ID3D11Device* device = GetD3D11Device();

		// Create the vertex shader
		if (CHECK_D3D11_FAILED(device->CreateVertexShader(shaderBlobs["vertex"]->GetBufferPointer(), shaderBlobs["vertex"]->GetBufferSize(), nullptr, &m_VertexShader))) {
			if (shaderBlobs["vertex"] != nullptr)
			{
				shaderBlobs["vertex"]->Release();
				shaderBlobs["vertex"] = nullptr;
			}
			return;
		}

		// Create the pixel shader
		if (CHECK_D3D11_FAILED(device->CreatePixelShader(shaderBlobs["pixel"]->GetBufferPointer(), shaderBlobs["pixel"]->GetBufferSize(), nullptr, &m_PixelShader))) {
			if (shaderBlobs["pixel"] != nullptr)
			{
				shaderBlobs["pixel"]->Release();
				shaderBlobs["pixel"] = nullptr;
			}
			return;
		}

		// Reflect the shader to get input information
		ID3D11ShaderReflection* shaderReflection = nullptr;
		CHECK_D3D11_ERRORS(
			D3DReflect(
				shaderBlobs["vertex"]->GetBufferPointer(),
				shaderBlobs["vertex"]->GetBufferSize(),
				IID_ID3D11ShaderReflection,
				(void**)&shaderReflection
			)
		);

		// Get shader description
		D3D11_SHADER_DESC desc{};
		shaderReflection->GetDesc(&desc);

		// Create input layout description
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
		for (UINT i = 0; i < desc.InputParameters; ++i)
		{
			D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
			shaderReflection->GetInputParameterDesc(i, &paramDesc);

			D3D11_INPUT_ELEMENT_DESC elementDesc = {};
			elementDesc.SemanticName = paramDesc.SemanticName;
			elementDesc.SemanticIndex = paramDesc.SemanticIndex;
			elementDesc.InputSlot = 0;
			elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			elementDesc.InstanceDataStepRate = 0;

			if (paramDesc.Mask == 1)
			{
				elementDesc.Format = (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) ? DXGI_FORMAT_R32_FLOAT : DXGI_FORMAT_R32_UINT;
			}
			else if (paramDesc.Mask <= 3)
			{
				elementDesc.Format = (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) ? DXGI_FORMAT_R32G32_FLOAT : DXGI_FORMAT_R32G32_UINT;
			}
			else if (paramDesc.Mask <= 7)
			{
				elementDesc.Format = (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) ? DXGI_FORMAT_R32G32B32_FLOAT : DXGI_FORMAT_R32G32B32_UINT;
			}
			else if (paramDesc.Mask <= 15)
			{
				elementDesc.Format = (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) ? DXGI_FORMAT_R32G32B32A32_FLOAT : DXGI_FORMAT_R32G32B32A32_UINT;
			}

			inputLayoutDesc.push_back(elementDesc);
		}

		// Create the input layout
		CHECK_D3D11_ERRORS(
			device->CreateInputLayout(
				inputLayoutDesc.data(),
				(UINT)inputLayoutDesc.size(),
				shaderBlobs["vertex"]->GetBufferPointer(),
				shaderBlobs["vertex"]->GetBufferSize(),
				&m_InputLayout
			)
		);
	}
}
