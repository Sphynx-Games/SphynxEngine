#include "spxpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>
#include "OpenGLShader.h"
#include "OpenGLTexture.h"
#include "OpenGLBuffer.h"
#include "OpenGLVertexArray.h"
#include "Math/Matrix.h"
#include "Math/Math.h"
#include <memory>
#include "Renderer/Sprite.h"
#include "OpenGLAssert.h"


namespace Sphynx
{
	static constexpr char* sDefaultVertexShaderSrc = R"(
		#version 330 core
		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec4 aColor;
		layout (location = 2) in vec2 aTexCoord;

		uniform mat4 MVP;
		
		out vec4 vColor;
		out vec2 vTexCoord;

		void main()
		{
			gl_Position = MVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);
			vColor = aColor;
			vTexCoord = aTexCoord;
		}
	)";
	static constexpr char* sDefaultFragShaderSrc = R"(
		#version 330 core
		in vec4 vColor;
		in vec2 vTexCoord;

		uniform sampler2D mTexture;

		out vec4 FragColor;

		void main()
		{
			FragColor = texture(mTexture, vTexCoord) * vColor;
			if (FragColor.a < 0.5) discard;
		};
	)";

	OpenGLRendererAPI::OpenGLRendererAPI() :
		m_DefaultShader(nullptr),
		m_WhiteTexture(nullptr)
	{
	}

	OpenGLRendererAPI::~OpenGLRendererAPI()
	{
	}

	void OpenGLRendererAPI::Init()
	{
		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_FRONT);
		//glFrontFace(GL_CCW);

		glEnable(GL_BLEND); CHECK_GL_ERRORS;
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); CHECK_GL_ERRORS;

		glEnable(GL_DEPTH_TEST); CHECK_GL_ERRORS;
		glDepthFunc(GL_LESS); CHECK_GL_ERRORS;

		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS); CHECK_GL_ERRORS;

		m_DefaultShader = new OpenGLShader("Default Shader", sDefaultVertexShaderSrc, sDefaultFragShaderSrc);
		m_WhiteTexture = new OpenGLTexture((void*)&Color::White, Vector2u{ 1, 1 });
	}

	void OpenGLRendererAPI::Shutdown()
	{
		delete m_WhiteTexture;
		m_WhiteTexture = nullptr;

		delete m_DefaultShader;
		m_DefaultShader = nullptr;
	}

	void OpenGLRendererAPI::SetViewport(Vector2i position, Vector2i size)
	{
		glViewport(position.X, position.Y, size.X, size.Y);  CHECK_GL_ERRORS;
	}

	Vector2i OpenGLRendererAPI::GetViewportSize()
	{
		GLint rect[4]{0};
		glGetIntegerv(GL_VIEWPORT, rect);
		return { (int32_t)rect[2], (int32_t)rect[3] };
	}

	void OpenGLRendererAPI::Clear(Color color)
	{
		glClearColor(color.R / 255.0f, color.G / 255.0f, color.B / 255.0f, color.A / 255.0f);  CHECK_GL_ERRORS;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);  CHECK_GL_ERRORS;
	}

	void OpenGLRendererAPI::Present()
	{
		// TODO: this will invoke all draw calls to write in the current render target
		// (i.e, backbuffer, texture, framebuffer, etc)
		
	}

	void OpenGLRendererAPI::DrawPoint(Vector2i point, Color color)
	{

	}

	void OpenGLRendererAPI::DrawLine(Vector2i point1, Vector2i point2, float lineWidth, Color color)
	{
		const float vertex[]
		{
			(float)point1.X, (float)point1.Y, 0.0f, // pos
			color.R / 255.0f, color.G / 255.0f, color.B / 255.0f, color.A / 255.0f, // color
			(float)point2.X, (float)point2.Y, 0.0f, // pos
			color.R / 255.0f, color.G / 255.0f, color.B / 255.0f, color.A / 255.0f, // color
		};
		constexpr uint32_t indexes[]{ 0, 1 };

		const BufferLayout bufferLayout
		{
			{ DataType::FLOAT3, "aPos", false },
			{ DataType::FLOAT4, "aColor", false },
		};
		std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<OpenGLVertexBuffer>((void*)vertex, sizeof(vertex), bufferLayout);
		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<OpenGLIndexBuffer>((uint32_t*)indexes, (uint32_t)(sizeof(indexes) / sizeof(uint32_t)));
		OpenGLVertexArray vertexArray;
		vertexArray.AddVertexBuffer(vertexBuffer);
		vertexArray.SetIndexBuffer(indexBuffer);

		m_WhiteTexture->Bind();
		m_DefaultShader->Bind();
		m_DefaultShader->SetMat4("MVP", glm::mat4(1.0f));
		m_DefaultShader->SetInt("mTexture", 0);

		vertexArray.Bind();
		glLineWidth(lineWidth);  CHECK_GL_ERRORS;
		glDrawArrays(GL_LINES, 0, 2);  CHECK_GL_ERRORS;
		vertexArray.Unbind();

		m_DefaultShader->Unbind();
	}

	void OpenGLRendererAPI::DrawLine(const Transform& transform, Vector2f point1, Vector2f point2, float lineWidth, Color color)
	{
		const float normColor[4]{ color.R / 255.0f, color.G / 255.0f, color.B / 255.0f, color.A / 255.0f };
		const float vertex[]
		{
			point1.X, point1.Y, 0.0f, // pos
			normColor[0], normColor[1], normColor[2], normColor[3], // color
			point2.X, point2.Y, 0.0f, // pos
			normColor[0], normColor[1], normColor[2], normColor[3], // color
		};
		constexpr uint32_t indexes[]{ 0, 1 };

		const BufferLayout bufferLayout
		{
			{ DataType::FLOAT3, "aPos", false },
			{ DataType::FLOAT4, "aColor", false },
		};
		std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<OpenGLVertexBuffer>((void*)vertex, sizeof(vertex), bufferLayout);
		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<OpenGLIndexBuffer>((uint32_t*)indexes, (uint32_t)(sizeof(indexes) / sizeof(uint32_t)));
		OpenGLVertexArray vertexArray;
		vertexArray.AddVertexBuffer(vertexBuffer);
		vertexArray.SetIndexBuffer(indexBuffer);

		m_WhiteTexture->Bind();
		m_DefaultShader->Bind();
		m_DefaultShader->SetMat4("MVP", GetMVPMatrix(transform));
		m_DefaultShader->SetInt("mTexture", 0);

		vertexArray.Bind();
		glLineWidth(lineWidth);  CHECK_GL_ERRORS;
		glDrawArrays(GL_LINES, 0, 2);  CHECK_GL_ERRORS;
		vertexArray.Unbind();

		m_DefaultShader->Unbind();
	}

	void OpenGLRendererAPI::DrawQuad(DrawMode drawMode, Vector2i center, Vector2i size, Color color)
	{
		const float normColor[4]{ color.R / 255.0f, color.G / 255.0f, color.B / 255.0f, color.A / 255.0f };
		const float vertex[]
		{
			-size.X * 0.5f, -size.Y * 0.5f,  0.0f,					// pos
			normColor[0], normColor[1], normColor[2], normColor[3], // color

			-size.X * 0.5f,  size.Y * 0.5f,  0.0f,					// pos
			normColor[0], normColor[1], normColor[2], normColor[3], // color

			 size.X * 0.5f,  size.Y * 0.5f,  0.0f,					// pos
			normColor[0], normColor[1], normColor[2], normColor[3], // color

			 size.X * 0.5f, -size.Y * 0.5f,  0.0f,					// pos
			normColor[0], normColor[1], normColor[2], normColor[3], // color
		};
		constexpr uint32_t indexes[]{ 0, 1, 2, 0, 2 ,3 };

		const BufferLayout bufferLayout
		{
			{ DataType::FLOAT3, "aPos", false },
			{ DataType::FLOAT4, "aColor", false },
		};

		std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<OpenGLVertexBuffer>((void*)vertex, sizeof(vertex), bufferLayout);
		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<OpenGLIndexBuffer>((uint32_t*)indexes, (uint32_t)(sizeof(indexes) / sizeof(uint32_t)));
		OpenGLVertexArray vertexArray;
		vertexArray.AddVertexBuffer(vertexBuffer);
		vertexArray.SetIndexBuffer(indexBuffer);

		m_WhiteTexture->Bind();
		m_DefaultShader->Bind();
		m_DefaultShader->SetMat4("MVP", glm::mat4(1.0f));
		m_DefaultShader->SetInt("mTexture", 0);

		vertexArray.Bind();
		glDrawElements(drawMode == FILLED ? GL_TRIANGLES : GL_LINE_LOOP, (GLsizei)indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);  CHECK_GL_ERRORS;
		vertexArray.Unbind();

		m_DefaultShader->Unbind();
	}

	void OpenGLRendererAPI::DrawQuad(DrawMode drawMode, const Transform& transform, Vector2f size, Vector2f pivot, Color color)
	{
		const float normColor[4]{ color.R / 255.0f, color.G / 255.0f, color.B / 255.0f, color.A / 255.0f };
		const float vertex[]
		{
			-size.X * 0.5f, -size.Y * 0.5f,  0.0f,					// pos
			normColor[0], normColor[1], normColor[2], normColor[3], // color

			-size.X * 0.5f,  size.Y * 0.5f,  0.0f,					// pos
			normColor[0], normColor[1], normColor[2], normColor[3], // color

			 size.X * 0.5f,  size.Y * 0.5f,  0.0f,					// pos
			normColor[0], normColor[1], normColor[2], normColor[3], // color

			 size.X * 0.5f, -size.Y * 0.5f,  0.0f,					// pos
			normColor[0], normColor[1], normColor[2], normColor[3], // color
		};
		constexpr uint32_t indexes[]{ 0, 1, 2, 0, 2 ,3 };

		const BufferLayout bufferLayout
		{
			{ DataType::FLOAT3, "aPos", false },
			{ DataType::FLOAT4, "aColor", false },
		};

		std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<OpenGLVertexBuffer>((void*)vertex, sizeof(vertex), bufferLayout);
		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<OpenGLIndexBuffer>((uint32_t*)indexes, (uint32_t)(sizeof(indexes) / sizeof(uint32_t)));
		OpenGLVertexArray vertexArray;
		vertexArray.AddVertexBuffer(vertexBuffer);
		vertexArray.SetIndexBuffer(indexBuffer);

		m_WhiteTexture->Bind();
		m_DefaultShader->Bind();
		m_DefaultShader->SetMat4("MVP", GetMVPMatrix(transform));
		m_DefaultShader->SetInt("mTexture", 0);

		vertexArray.Bind();
		glDrawElements(drawMode == FILLED ? GL_TRIANGLES : GL_LINE_LOOP, (GLsizei)indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);  CHECK_GL_ERRORS;
		vertexArray.Unbind();

		m_DefaultShader->Unbind();
	}

	void OpenGLRendererAPI::DrawTriangle(DrawMode drawMode, Vector2i point1, Vector2i point2, Vector2i point3, Color color)
	{
		const float normColor[4]{ color.R / 255.0f, color.G / 255.0f, color.B / 255.0f, color.A / 255.0f };
		const float vertex[]
		{
			(float)point1.X, (float)point1.Y, 0.0f,					// pos
			normColor[0], normColor[1], normColor[2], normColor[3], // color

			(float)point2.X, (float)point2.Y, 0.0f,					// pos
			normColor[0], normColor[1], normColor[2], normColor[3], // color

			(float)point3.X, (float)point3.Y, 0.0f,					// pos
			normColor[0], normColor[1], normColor[2], normColor[3], // color
		};
		constexpr uint32_t indexes[]{ 0, 1, 2 };

		const BufferLayout bufferLayout
		{
			{ DataType::FLOAT3, "aPos", false },
			{ DataType::FLOAT4, "aColor", false },
		};

		std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<OpenGLVertexBuffer>((void*)vertex, sizeof(vertex), bufferLayout);
		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<OpenGLIndexBuffer>((uint32_t*)indexes, (uint32_t)(sizeof(indexes) / sizeof(uint32_t)));
		OpenGLVertexArray vertexArray;
		vertexArray.AddVertexBuffer(vertexBuffer);
		vertexArray.SetIndexBuffer(indexBuffer);

		m_WhiteTexture->Bind();
		m_DefaultShader->Bind();
		m_DefaultShader->SetMat4("MVP", glm::mat4(1.0f));
		m_DefaultShader->SetInt("mTexture", 0);

		vertexArray.Bind();
		glDrawElements(drawMode == FILLED ? GL_TRIANGLES : GL_LINE_LOOP, (GLsizei)indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);  CHECK_GL_ERRORS;
		vertexArray.Unbind();

		m_DefaultShader->Unbind();
	}

	void OpenGLRendererAPI::DrawTriangle(DrawMode drawMode, const Transform& transform, Vector2f point1, Vector2f point2, Vector2f point3, Vector2f pivot, Color color)
	{
		const float normColor[4]{ color.R / 255.0f, color.G / 255.0f, color.B / 255.0f, color.A / 255.0f };
		const float vertex[]
		{
			point1.X, point1.Y, 0.0f,								// pos
			normColor[0], normColor[1], normColor[2], normColor[3], // color

			point2.X, point2.Y, 0.0f,								// pos
			normColor[0], normColor[1], normColor[2], normColor[3], // color

			point3.X, point3.Y, 0.0f,								// pos
			normColor[0], normColor[1], normColor[2], normColor[3], // color
		};
		constexpr uint32_t indexes[]{ 0, 1, 2 };

		const BufferLayout bufferLayout
		{
			{ DataType::FLOAT3, "aPos", false },
			{ DataType::FLOAT4, "aColor", false },
		};

		std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<OpenGLVertexBuffer>((void*)vertex, sizeof(vertex), bufferLayout);
		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<OpenGLIndexBuffer>((uint32_t*)indexes, (uint32_t)(sizeof(indexes) / sizeof(uint32_t)));
		OpenGLVertexArray vertexArray;
		vertexArray.AddVertexBuffer(vertexBuffer);
		vertexArray.SetIndexBuffer(indexBuffer);

		m_WhiteTexture->Bind();
		m_DefaultShader->Bind();
		m_DefaultShader->SetMat4("MVP", GetMVPMatrix(transform));
		m_DefaultShader->SetInt("mTexture", 0);

		vertexArray.Bind();
		glDrawElements(drawMode == FILLED ? GL_TRIANGLES : GL_LINE_LOOP, (GLsizei)indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);  CHECK_GL_ERRORS;
		vertexArray.Unbind();

		m_DefaultShader->Unbind();
	}

	void OpenGLRendererAPI::DrawCircle(DrawMode drawMode, Vector2i center, float radius, uint32_t numSegments, Color color)
	{
		const float normColor[4]{ color.R / 255.0f, color.G / 255.0f, color.B / 255.0f, color.A / 255.0f };
		std::vector<float> vertex;
		std::vector<uint32_t> indexes;
		vertex.reserve((numSegments + 2) * 7); // 7 = 3(pos) + 4(color)
		indexes.reserve(numSegments * 3);

		// add center
		vertex.emplace_back((float)center.X);
		vertex.emplace_back((float)center.Y);
		vertex.emplace_back(0.0f);
		vertex.emplace_back(normColor[0]);
		vertex.emplace_back(normColor[1]);
		vertex.emplace_back(normColor[2]);
		vertex.emplace_back(normColor[3]);

		const float circumference = radius * (float)Math::TAU;
		const float alpha = (float)Math::TAU / numSegments; // in radians
		for (uint32_t i = 0; i < numSegments; ++i)
		{
			// push segments' first point
			const float x = radius * std::cos(alpha * i) + center.X;
			const float y = radius * std::sin(alpha * i) + center.Y;

			// pos
			vertex.emplace_back(x);
			vertex.emplace_back(y);
			vertex.emplace_back(0.0f);

			// color
			vertex.emplace_back(normColor[0]);
			vertex.emplace_back(normColor[1]);
			vertex.emplace_back(normColor[2]);
			vertex.emplace_back(normColor[3]);

			// push segment indices
			indexes.emplace_back(0);
			indexes.emplace_back(i + 1);
			indexes.emplace_back(((i + 1) % numSegments) + 1);
		}

		const BufferLayout bufferLayout
		{
			{ DataType::FLOAT3, "aPos", false },
			{ DataType::FLOAT4, "aColor", false },
		};

		std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<OpenGLVertexBuffer>((void*)vertex.data(), sizeof(float) * vertex.size(), bufferLayout);
		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<OpenGLIndexBuffer>(indexes.data(), (uint32_t)indexes.size());
		OpenGLVertexArray vertexArray;
		vertexArray.AddVertexBuffer(vertexBuffer);
		vertexArray.SetIndexBuffer(indexBuffer);

		m_WhiteTexture->Bind();
		m_DefaultShader->Bind();
		m_DefaultShader->SetMat4("MVP", glm::mat4(1.0f));
		m_DefaultShader->SetInt("mTexture", 0);

		vertexArray.Bind();
		glDrawElements(drawMode == FILLED ? GL_TRIANGLES : GL_LINE_LOOP, (GLsizei)indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);  CHECK_GL_ERRORS;
		vertexArray.Unbind();

		m_DefaultShader->Unbind();
	}

	void OpenGLRendererAPI::DrawCircle(DrawMode drawMode, const Transform& transform, float radius, uint32_t numSegments, Vector2f pivot, Color color)
	{
		const float xDiff = pivot.X * radius * 2.0f - (1.0f - pivot.X) * radius * 2.0f;
		const float yDiff = pivot.Y * radius * 2.0f - (1.0f - pivot.Y) * radius * 2.0f;
		const Vector2f center{ -xDiff, -yDiff };
		const float normColor[4]{ color.R / 255.0f, color.G / 255.0f, color.B / 255.0f, color.A / 255.0f };
		std::vector<float> vertex;
		std::vector<uint32_t> indexes;
		vertex.reserve((numSegments + 2) * 7); // 7 = 3(pos) + 4(color)
		indexes.reserve(numSegments * 3);

		// add center
		vertex.emplace_back(center.X);
		vertex.emplace_back(center.Y);
		vertex.emplace_back(0.0f);
		vertex.emplace_back(normColor[0]);
		vertex.emplace_back(normColor[1]);
		vertex.emplace_back(normColor[2]);
		vertex.emplace_back(normColor[3]);

		const float circumference = radius * (float)Math::TAU;
		const float alpha = (float)Math::TAU / numSegments; // in radians
		for (uint32_t i = 0; i < numSegments; ++i)
		{
			// push segments' first point
			const float x = radius * std::cos(alpha * i) + center.X;
			const float y = radius * std::sin(alpha * i) + center.Y;

			// pos
			vertex.emplace_back(x);
			vertex.emplace_back(y);
			vertex.emplace_back(0.0f);

			// color
			vertex.emplace_back(normColor[0]);
			vertex.emplace_back(normColor[1]);
			vertex.emplace_back(normColor[2]);
			vertex.emplace_back(normColor[3]);

			// push segment indices
			indexes.emplace_back(0);
			indexes.emplace_back(i + 1);
			indexes.emplace_back(((i + 1) % numSegments) + 1);
		}

		const BufferLayout bufferLayout
		{
			{ DataType::FLOAT3, "aPos", false },
			{ DataType::FLOAT4, "aColor", false },
		};

		std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<OpenGLVertexBuffer>((void*)vertex.data(), sizeof(float) * vertex.size(), bufferLayout);
		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<OpenGLIndexBuffer>(indexes.data(), (uint32_t)indexes.size());
		OpenGLVertexArray vertexArray;
		vertexArray.AddVertexBuffer(vertexBuffer);
		vertexArray.SetIndexBuffer(indexBuffer);

		m_WhiteTexture->Bind();
		m_DefaultShader->Bind();
		m_DefaultShader->SetMat4("MVP", GetMVPMatrix(transform));
		m_DefaultShader->SetInt("mTexture", 0);

		vertexArray.Bind();
		glDrawElements(drawMode == FILLED ? GL_TRIANGLES : GL_LINE_LOOP, (GLsizei)indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);  CHECK_GL_ERRORS;
		vertexArray.Unbind();

		m_DefaultShader->Unbind();
	}

	void OpenGLRendererAPI::DrawTexture(const Texture& texture, Vector2i position, Vector2i size, Color color)
	{
		const float normColor[4]{ color.R / 255.0f, color.G / 255.0f, color.B / 255.0f, color.A / 255.0f };
		const float vertex[]
		{
			-size.X * 0.5f, -size.Y * 0.5f,  0.0f,					// pos
			normColor[0], normColor[1], normColor[2], normColor[3], // color
			0.0f, 0.0f,												// uv

			-size.X * 0.5f,  size.Y * 0.5f,  0.0f,					// pos
			normColor[0], normColor[1], normColor[2], normColor[3], // color
			0.0f, 1.0f,												// uv

			 size.X * 0.5f,  size.Y * 0.5f,  0.0f,					// pos
			normColor[0], normColor[1], normColor[2], normColor[3], // color
			1.0f, 1.0f,												// uv

			 size.X * 0.5f, -size.Y * 0.5f,  0.0f,					// pos
			normColor[0], normColor[1], normColor[2], normColor[3], // color
			1.0f, 0.0f,												// uv
		};
		constexpr uint32_t indexes[]{ 0, 1, 2, 0, 2 ,3 };

		const BufferLayout bufferLayout
		{
			{ DataType::FLOAT3, "aPos", false },
			{ DataType::FLOAT4, "aColor", false },
			{ DataType::FLOAT2, "aTexCoord", false },
		};

		std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<OpenGLVertexBuffer>((void*)vertex, sizeof(vertex), bufferLayout);
		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<OpenGLIndexBuffer>((uint32_t*)indexes, (uint32_t)(sizeof(indexes) / sizeof(uint32_t)));
		OpenGLVertexArray vertexArray;
		vertexArray.AddVertexBuffer(vertexBuffer);
		vertexArray.SetIndexBuffer(indexBuffer);

		texture.Bind();
		m_DefaultShader->Bind();
		m_DefaultShader->SetMat4("MVP", glm::mat4(1.0f));
		m_DefaultShader->SetInt("mTexture", 0);

		vertexArray.Bind();
		glDrawElements(GL_TRIANGLES, (GLsizei)indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);  CHECK_GL_ERRORS;
		vertexArray.Unbind();

		m_DefaultShader->Unbind();
	}

	void OpenGLRendererAPI::DrawTexture(const Texture& texture, const Transform& transform, Vector2f size, Vector2f pivot, Color color)
	{
		const float normColor[4]{ color.R / 255.0f, color.G / 255.0f, color.B / 255.0f, color.A / 255.0f };
		const float vertex[]
		{
			-size.X * 0.5f, -size.Y * 0.5f,  0.0f,					// pos
			normColor[0], normColor[1], normColor[2], normColor[3], // color
			0.0f, 0.0f,												// uv

			-size.X * 0.5f,  size.Y * 0.5f,  0.0f,					// pos
			normColor[0], normColor[1], normColor[2], normColor[3], // color
			0.0f, 1.0f,												// uv

			 size.X * 0.5f,  size.Y * 0.5f,  0.0f,					// pos
			normColor[0], normColor[1], normColor[2], normColor[3], // color
			1.0f, 1.0f,												// uv

			 size.X * 0.5f, -size.Y * 0.5f,  0.0f,					// pos
			normColor[0], normColor[1], normColor[2], normColor[3], // color
			1.0f, 0.0f,												// uv
		};
		constexpr uint32_t indexes[]{ 0, 1, 2, 0, 2 ,3 };

		const BufferLayout bufferLayout
		{
			{ DataType::FLOAT3, "aPos", false },
			{ DataType::FLOAT4, "aColor", false },
			{ DataType::FLOAT2, "aTexCoord", false },
		};

		std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<OpenGLVertexBuffer>((void*)vertex, sizeof(vertex), bufferLayout);
		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<OpenGLIndexBuffer>((uint32_t*)indexes, (uint32_t)(sizeof(indexes) / sizeof(uint32_t)));
		OpenGLVertexArray vertexArray;
		vertexArray.AddVertexBuffer(vertexBuffer);
		vertexArray.SetIndexBuffer(indexBuffer);

		texture.Bind();
		m_DefaultShader->Bind();
		m_DefaultShader->SetMat4("MVP", GetMVPMatrix(transform));
		m_DefaultShader->SetInt("mTexture", 0);

		vertexArray.Bind();
		glDrawElements(GL_TRIANGLES, (GLsizei)indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);  CHECK_GL_ERRORS;
		vertexArray.Unbind();

		m_DefaultShader->Unbind();
	}

	void OpenGLRendererAPI::DrawSprite(const Sprite& sprite, Vector2i position, Vector2i size, Color color)
	{
		// TODO
		SPX_CORE_ASSERT(false, "Not supported yet");
	}

	void OpenGLRendererAPI::DrawSprite(const Sprite& sprite, const Transform& transform, Color color)
	{
		const float minU = (float)sprite.GetPosition().X / sprite.GetTexture()->GetWidth();
		const float maxU = (float)(sprite.GetPosition().X + sprite.GetSize().X) / sprite.GetTexture()->GetWidth();
		const float minV = 1.0f - (float)(sprite.GetPosition().Y - sprite.GetSize().Y) / sprite.GetTexture()->GetHeight();
		const float maxV = 1.0f - (float)(sprite.GetPosition().Y) / sprite.GetTexture()->GetHeight();

		const Vector2f pivot = sprite.GetPivot();
		const Vector2f size = { (float)sprite.GetSize().X / sprite.GetPixelsPerUnit(), (float)sprite.GetSize().Y / sprite.GetPixelsPerUnit() };
		const float normColor[4]{ color.R / 255.0f, color.G / 255.0f, color.B / 255.0f, color.A / 255.0f };
		const float vertex[]
		{
			-size.X * pivot.X, -size.Y * pivot.Y,  0.0f,					// pos
			normColor[0], normColor[1], normColor[2], normColor[3],			// color
			minU, minV,														// uv

			-size.X * pivot.X,  size.Y * (1.0f - pivot.Y),  0.0f,			// pos
			normColor[0], normColor[1], normColor[2], normColor[3],			// color
			minU, maxV,														// uv

			 size.X * (1.0f - pivot.X),  size.Y * (1.0f - pivot.Y),  0.0f,	// pos
			normColor[0], normColor[1], normColor[2], normColor[3],			// color
			maxU, maxV,														// uv

			 size.X * (1.0f - pivot.X), -size.Y * pivot.Y,  0.0f,			// pos
			normColor[0], normColor[1], normColor[2], normColor[3],			// color
			maxU, minV,														// uv
		};
		constexpr uint32_t indexes[]{ 0, 1, 2, 0, 2 ,3 };

		const BufferLayout bufferLayout
		{
			{ DataType::FLOAT3, "aPos", false },
			{ DataType::FLOAT4, "aColor", false },
			{ DataType::FLOAT2, "aTexCoord", false },
		};

		std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<OpenGLVertexBuffer>((void*)vertex, sizeof(vertex), bufferLayout);
		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<OpenGLIndexBuffer>((uint32_t*)indexes, (uint32_t)(sizeof(indexes) / sizeof(uint32_t)));
		OpenGLVertexArray vertexArray;
		vertexArray.AddVertexBuffer(vertexBuffer);
		vertexArray.SetIndexBuffer(indexBuffer);

		sprite.GetTexture()->Bind();
		m_DefaultShader->Bind();
		m_DefaultShader->SetMat4("MVP", GetMVPMatrix(transform));
		m_DefaultShader->SetInt("mTexture", 0);

		vertexArray.Bind();
		glDrawElements(GL_TRIANGLES, (GLsizei)indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);  CHECK_GL_ERRORS;
		vertexArray.Unbind();

		m_DefaultShader->Unbind();
	}

	void OpenGLRendererAPI::DrawText(const std::string& text, const Font& font, float size, Vector2i position, Vector2i scale, Color color)
	{
		// TODO
		SPX_CORE_ASSERT(false, "Not supported yet");
	}

	//void OpenGLRendererAPI::OnWindowResized(uint32_t width, uint32_t height)
	//{
	//	// do nothing
	//}

	//void OpenGLRendererAPI::SetClearColor(float r, float g, float b, float a) const
	//{
	//	glClearColor(r, g, b, a);
	//}

	//void OpenGLRendererAPI::Clear() const
	//{
	//	// todo: temporal mask
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	//}

	//void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) const
	//{
	//	glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	//}

	//void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const
	//{
	//	glViewport(x, y, width, height);
	//}
}
