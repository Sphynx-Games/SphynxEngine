#include "Scene.h"
#include "Actor.h"
#include "Component/Components.h"
#include "Renderer/Renderer2D.h"
#include "Physics/Physics2D.h"

namespace Sphynx
{
	Scene::Scene() :
		m_HasBegunPlay(false),
		m_Registry(),
		m_Actors(),
		m_PhysicsScene(nullptr)
	{
	}

	Scene::~Scene()
	{
	}

	extern const glm::mat4 GetModelMatrixFromTransform(const Transform& transform);

	void Scene::BeginPlay()
	{
		if (m_HasBegunPlay) return;

		m_PhysicsScene = Physics2D::CreatePhysics2DScene(*this);
		
		m_HasBegunPlay = true;
	}

	void Scene::EndPlay()
	{
		Physics2D::DestroyPhysics2DScene(m_PhysicsScene);
		m_PhysicsScene = nullptr;
	}

	void Scene::Update(float deltaTime)
	{
		// Draw every SPRITE RENDERER component in scene
		auto spriteGroup = m_Registry.group<SpriteRendererComponent>(entt::get<TransformComponent>);
		for (entt::entity entity : spriteGroup)
		{
			auto [spriteRenderer, transform] = spriteGroup.get<SpriteRendererComponent, TransformComponent>(entity);
			if (spriteRenderer.Sprite != nullptr)
			{
				Renderer2D::DrawSprite(*spriteRenderer.Sprite, transform.Transform, spriteRenderer.Tint);
			}
			else
			{
				Renderer2D::DrawQuad(DrawMode::FILLED, transform.Transform, { 1.0f, 1.0f }, { 0.5f, 0.5f }, spriteRenderer.Tint);
			}
		}

		// Draw every BOX RENDERER component in scene
		auto boxGroup = m_Registry.group<BoxRendererComponent>(entt::get<TransformComponent>);
		for (entt::entity entity : boxGroup)
		{
			auto [boxRenderer, transform] = boxGroup.get<BoxRendererComponent, TransformComponent>(entity);
			Renderer2D::DrawQuad(boxRenderer.DrawMode, transform.Transform, boxRenderer.Size, boxRenderer.Pivot, boxRenderer.Color);
		}

		// Draw every LINE RENDERER component in scene
		auto lineGroup = m_Registry.group<LineRendererComponent>(entt::get<TransformComponent>);
		for (entt::entity entity : lineGroup)
		{
			auto [lineRenderer, transform] = lineGroup.get<LineRendererComponent, TransformComponent>(entity);
			Renderer2D::DrawLine(transform.Transform, lineRenderer.Point1, lineRenderer.Point2, lineRenderer.LineWidth, lineRenderer.Color);
		}

		// Simulate PHYSISCS in scene
		Physics2D::Step(*this, deltaTime);
	}

	Actor Scene::CreateActor()
	{
		Actor& actor = m_Actors.emplace_back(m_Registry.create(), this);
		actor.AddComponent<UUIDComponent>(UUID::Generate());
		return actor;
	}

	void Scene::DestroyActor(const Actor& actor)
	{
		m_Registry.destroy(actor);
	}

	const std::vector<Actor>& Scene::GetActors() const
	{
		return m_Actors;
	}

}
