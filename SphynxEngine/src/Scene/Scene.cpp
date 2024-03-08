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
		auto group = m_Registry.group<TransformComponent, SpriteRendererComponent>();
		for (entt::entity entity : group)
		{
			auto [tranform, spriteRenderer] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			if (spriteRenderer.Sprite != nullptr)
			{
				Renderer2D::DrawSprite(*spriteRenderer.Sprite, tranform.Transform, { 100.0f, 100.0f }, spriteRenderer.Tint);
			}
			else
			{
				Renderer2D::DrawQuad(DrawMode::FILLED, tranform.Transform, { 1.0f, 1.0f }, { 0.5f, 0.5f }, spriteRenderer.Tint);
			}
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