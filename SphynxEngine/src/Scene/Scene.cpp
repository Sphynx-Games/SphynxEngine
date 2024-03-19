#include "Scene.h"
#include "Actor.h"
#include "Component/Components.h"
#include "Renderer/Renderer2D.h"


namespace Sphynx
{
	Scene::Scene() :
		m_Registry()
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::Update(float deltaTime)
	{
		// Draw every sprite renderer component in scene
		auto spriteGroup = m_Registry.group<SpriteRendererComponent>(entt::get<TransformComponent>);
		for (entt::entity entity : spriteGroup)
		{
			auto [spriteRenderer, tranform] = spriteGroup.get<SpriteRendererComponent, TransformComponent>(entity);
			if (spriteRenderer.Sprite != nullptr)
			{
				Renderer2D::DrawSprite(*spriteRenderer.Sprite, tranform.Transform, spriteRenderer.Tint);
			}
			else
			{
				Renderer2D::DrawQuad(DrawMode::FILLED, tranform.Transform, { 1.0f, 1.0f }, { 0.5f, 0.5f }, spriteRenderer.Tint);
			}
		}

		// Draw every box renderer component in scene
		auto boxGroup = m_Registry.group<BoxRendererComponent>(entt::get<TransformComponent>);
		for (entt::entity entity : boxGroup)
		{
			auto [boxRenderer, tranform] = boxGroup.get<BoxRendererComponent, TransformComponent>(entity);
			Renderer2D::DrawQuad(boxRenderer.DrawMode, tranform.Transform, boxRenderer.Size, boxRenderer.Pivot, boxRenderer.Color);
		}

		// Draw every line renderer component in scene
		auto lineGroup = m_Registry.group<LineRendererComponent>(entt::get<TransformComponent>);
		for (entt::entity entity : lineGroup)
		{
			auto [lineRenderer, tranform] = lineGroup.get<LineRendererComponent, TransformComponent>(entity);
			Renderer2D::DrawLine(tranform.Transform, lineRenderer.Point1, lineRenderer.Point2, lineRenderer.LineWidth, lineRenderer.Color);
		}
	}

	Actor Scene::CreateActor()
	{
		return { m_Registry.create(), this };
	}

	void Scene::DestroyActor(const Actor& actor)
	{
		m_Registry.destroy(actor);
	}

}