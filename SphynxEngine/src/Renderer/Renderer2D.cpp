#include "Renderer2D.h"
#include "RendererAPI.h"
#include "OrthographicCamera.h"
#include "Texture.h"
#include "Sprite.h"
#include "Math/Transform.h"
#include "Core/Assets.h"

namespace Sphynx
{
	RendererAPI* Renderer2D::s_RendererAPI = nullptr;
	RendererConfig Renderer2D::s_RendererConfig;

	void Renderer2D::Init()
	{
		s_RendererAPI = RendererAPI::Create();
		s_RendererAPI->Init();
	}

	void Renderer2D::Shutdown()
	{
		Assets::DeleteTextures();

		s_RendererAPI->Shutdown();
		delete s_RendererAPI;
		s_RendererAPI = nullptr;
	}

	void Renderer2D::Begin(const OrthographicCamera* camera)
	{
		s_RendererConfig.ViewProjectionMatrix = camera != nullptr ? camera->GetViewProjectionMatrix() : glm::mat4(1.0f);
		s_RendererAPI->Clear(s_RendererConfig.ClearColor);
	}

	void Renderer2D::End()
	{
		s_RendererAPI->Present();
	}

	const RendererAPI* Renderer2D::GetRendererAPI()
	{
		return s_RendererAPI;
	}

	const RendererConfig& Renderer2D::GetConfiguration()
	{
		return s_RendererConfig;
	}

	void Renderer2D::SetViewport(Vector2i position, Vector2i size)
	{
		s_RendererAPI->SetViewport(position, size);
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

	void Renderer2D::DrawLine(Vector2i point1, Vector2i point2, float lineWidth, Color color)
	{
		s_RendererAPI->DrawLine(point1, point2, lineWidth, color);
	}

	void Renderer2D::DrawQuad(Vector2i center, Vector2i size, Color color)
	{
		s_RendererAPI->DrawQuad(s_RendererConfig.DrawMode, center, size, color);
	}

	void Renderer2D::DrawTriangle(Vector2i point1, Vector2i point2, Vector2i point3, Color color)
	{
		s_RendererAPI->DrawTriangle(s_RendererConfig.DrawMode, point1, point2, point3, color);
	}

	void Renderer2D::DrawCircle(Vector2i center, float radius, uint32_t numSegments, Color color)
	{
		s_RendererAPI->DrawCircle(s_RendererConfig.DrawMode, center, radius, numSegments, color);
	}

	void Renderer2D::DrawQuad(DrawMode drawMode, Vector2i center, Vector2i size, Color color)
	{
		s_RendererAPI->DrawQuad(drawMode, center, size, color);
	}

	void Renderer2D::DrawTriangle(DrawMode drawMode, Vector2i point1, Vector2i point2, Vector2i point3, Color color)
	{
		s_RendererAPI->DrawTriangle(drawMode, point1, point2, point3, color);
	}

	void Renderer2D::DrawCircle(DrawMode drawMode, Vector2i center, float radius, uint32_t numSegments, Color color)
	{
		s_RendererAPI->DrawCircle(drawMode, center, radius, numSegments, color);
	}

	void Renderer2D::DrawTexture(const Texture& texture, Vector2i position, Vector2i size, Color color)
	{
		s_RendererAPI->DrawTexture(texture, position, size, color);
	}

	void Renderer2D::DrawSprite(const Sprite& sprite, Vector2i position, Vector2i size, Color color)
	{
		s_RendererAPI->DrawSprite(sprite, position, size, color);
	}

	void Renderer2D::DrawLine(const Transform& transform, Vector2f point1, Vector2f point2, float lineWidth, Color color)
	{
		s_RendererAPI->DrawLine(transform, point1, point2, lineWidth, color);
	}


	void Renderer2D::DrawQuad(const Transform& transform, Vector2f size, Vector2f pivot, Color color)
	{
		s_RendererAPI->DrawQuad(s_RendererConfig.DrawMode, transform, size, pivot, color);
	}

	void Renderer2D::DrawTriangle(const Transform& transform, Vector2f point1, Vector2f point2, Vector2f point3, Vector2f pivot, Color color)
	{
		s_RendererAPI->DrawTriangle(s_RendererConfig.DrawMode, transform, point1, point2, point3, pivot, color);
	}

	void Renderer2D::DrawCircle(const Transform& transform, float radius, uint32_t numSegments, Vector2f pivot, Color color)
	{
		s_RendererAPI->DrawCircle(s_RendererConfig.DrawMode, transform, radius, numSegments, pivot, color);
	}

	void Renderer2D::DrawQuad(DrawMode drawMode, const Transform& transform, Vector2f size, Vector2f pivot, Color color)
	{
		s_RendererAPI->DrawQuad(drawMode, transform, size, pivot, color);
	}

	void Renderer2D::DrawTriangle(DrawMode drawMode, const Transform& transform, Vector2f point1, Vector2f point2, Vector2f point3, Vector2f pivot, Color color)
	{
		s_RendererAPI->DrawTriangle(drawMode, transform, point1, point2, point3, pivot, color);
	}

	void Renderer2D::DrawCircle(DrawMode drawMode, const Transform& transform, float radius, uint32_t numSegments, Vector2f pivot, Color color)
	{
		s_RendererAPI->DrawCircle(drawMode, transform, radius, numSegments, pivot, color);
	}

	void Renderer2D::DrawTexture(const Texture& texture, const Transform& transform, Vector2f size, Vector2f pivot, Color color)
	{
		s_RendererAPI->DrawTexture(texture, transform, size, pivot, color);
	}

	void Renderer2D::DrawSprite(const Sprite& sprite, const Transform& transform, Color color)
	{
		s_RendererAPI->DrawSprite(sprite, transform, color);
	}
}