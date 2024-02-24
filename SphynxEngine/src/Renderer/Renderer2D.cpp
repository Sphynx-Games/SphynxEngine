#include "Renderer2D.h"
#include "RendererAPI.h"

namespace Sphynx
{
	RendererAPI* Renderer2D::s_RendererAPI = nullptr;
	RendererConfig Renderer2D::s_RendererConfig;

	void Renderer2D::Init()
	{
		s_RendererAPI = RendererAPI::Create();
		s_RendererAPI->Init();

		s_RendererConfig.DrawMode = DrawMode::WIREFRAME;
		s_RendererConfig.ClearColor = Color::FromHex(0x000000FF); // black
	}

	void Renderer2D::Shutdown()
	{
		delete s_RendererAPI;
		s_RendererAPI = nullptr;
	}

	void Renderer2D::Begin()
	{
		s_RendererAPI->Clear(s_RendererConfig.ClearColor);
	}

	void Renderer2D::End()
	{
		s_RendererAPI->Present();
	}

	const RendererConfig& Renderer2D::GetRendererConfig()
	{
		return s_RendererConfig;
	}

	void Renderer2D::SetViewport(Vector2i position, uint32_t width, uint32_t height)
	{
		s_RendererAPI->SetViewport(position, width, height);
	}

	void Renderer2D::SetClearColor(Color color)
	{
		s_RendererConfig.ClearColor = color;
	}

	void Renderer2D::SetDrawMode(DrawMode drawMode)
	{
		s_RendererConfig.DrawMode = drawMode;
	}

	void Renderer2D::DrawPoint(Vector2i point, Color color)
	{
		s_RendererAPI->DrawPoint(point, color);
	}

	void Renderer2D::DrawLine(Vector2i point1, Vector2i point2, Color color)
	{
		s_RendererAPI->DrawLine(point1, point2, color);
	}

	void Renderer2D::DrawQuad(Vector2i point, uint32_t width, uint32_t height, Color color)
	{
		s_RendererAPI->DrawQuad(point, width, height, color);
	}

	void Renderer2D::DrawTriangle(Vector2i point1, Vector2i point2, Vector2i point3, Color color)
	{
		s_RendererAPI->DrawTriangle(point1, point2, point3, color);
	}

	void Renderer2D::DrawCircle(Vector2i center, float radius, uint32_t numSegments, Color color)
	{
		s_RendererAPI->DrawCircle(center, radius, numSegments, color);
	}

	void Renderer2D::DrawQuad(DrawMode drawMode, Vector2i point, uint32_t width, uint32_t height, Color color)
	{
		s_RendererAPI->DrawQuad(drawMode, point, width, height, color);
	}

	void Renderer2D::DrawTriangle(DrawMode drawMode, Vector2i point1, Vector2i point2, Vector2i point3, Color color)
	{
		s_RendererAPI->DrawTriangle(drawMode, point1, point2, point3, color);
	}

	void Renderer2D::DrawCircle(DrawMode drawMode, Vector2i center, float radius, uint32_t numSegments, Color color)
	{
		s_RendererAPI->DrawCircle(drawMode, center, radius, numSegments, color);
	}
}