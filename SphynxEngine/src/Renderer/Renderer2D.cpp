#include "Renderer2D.h"
#include "RendererAPI.h"

namespace Sphynx
{
	RendererAPI* Renderer2D::m_RendererAPI = RendererAPI::Create();

	void Renderer2D::Init()
	{
		m_RendererAPI->Init();
	}

	void Renderer2D::Shutdown()
	{
	}

	void Renderer2D::Begin(glm::vec4 clearColor)
	{
		m_RendererAPI->Clear(clearColor);
	}

	void Renderer2D::End()
	{
		m_RendererAPI->Present();
	}

	void Renderer2D::SetViewport(glm::vec2 position, uint32_t width, uint32_t height)
	{
		m_RendererAPI->SetViewport(position, width, height);
	}

	void Renderer2D::DrawPoint(glm::vec2 point, glm::vec4 color)
	{
		m_RendererAPI->DrawPoint(point, color);
	}

	void Renderer2D::DrawLine(glm::vec2 point1, glm::vec2 point2, glm::vec4 color)
	{
		m_RendererAPI->DrawLine(point1, point2, color);
	}

	void Renderer2D::DrawQuad(glm::vec2 point, uint32_t width, uint32_t height, glm::vec4 color)
	{
		m_RendererAPI->DrawQuad(point, width, height, color);
	}

	void Renderer2D::DrawTriangle(glm::vec2 point1, glm::vec2 point2, glm::vec2 point3, glm::vec4 color)
	{
		m_RendererAPI->DrawTriangle(point1, point2, point3, color);
	}

	void Renderer2D::DrawCircle(glm::vec2 point, float radius, uint32_t numSegments, glm::vec4 color)
	{
		m_RendererAPI->DrawCircle(point, radius, numSegments, color);
	}
}