#include "Scene.h"
#include "Actor.h"
#include "Component/Components.h"
#include "Component/BoxCollider2DComponent.h"
#include "Renderer/Renderer2D.h"

#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"

namespace Sphynx
{
	b2BodyType RigidbodyType_To_Box2D(RigidbodyType type)
	{
		switch (type)
		{
		case RigidbodyType::STATIC:     return b2BodyType::b2_staticBody;
		case RigidbodyType::DYNAMIC:    return b2BodyType::b2_dynamicBody;
		case RigidbodyType::KINEMATIC:  return b2BodyType::b2_kinematicBody;
		}
	}

	Scene::Scene() :
		m_HasBegunPlay(false),
		m_Registry(),
		m_PhysicWorld(new b2World({ 0.0f, -10.0f })),
		m_Actors()
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::BeginPlay()
	{
		if (m_HasBegunPlay) return;

		auto rigidbodyGroup = m_Registry.group<RigidbodyComponent>(entt::get<BoxCollider2DComponent, TransformComponent>);
		for (entt::entity entity : rigidbodyGroup)
		{
			auto [rigidbody, collider, transform] = rigidbodyGroup.get<RigidbodyComponent, BoxCollider2DComponent, TransformComponent>(entity);

			// body definition
			b2BodyDef bodyDef;
			bodyDef.type = RigidbodyType_To_Box2D(rigidbody.RigidbodyType);
			bodyDef.position = { transform.Transform.Position.X, transform.Transform.Position.Y };
			bodyDef.angle = glm::radians(transform.Transform.Rotation.Z);

			// body, shape of a box and fixture
			b2PolygonShape shape;
			shape.SetAsBox(
				(transform.Transform.Scale.X * collider.Size.X) / 2.0f,
				(transform.Transform.Scale.Y * collider.Size.Y) / 2.0f//,
				//{ 0.0f, 0.0f }, // { collider.Pivot.X, collider.Pivot.Y }, // TODO: take pivot into account
				//0.0f
			);

			b2Body* body = m_PhysicWorld->CreateBody(&bodyDef);
			body->CreateFixture(&shape, 1.0f);   // fixture = sub collider, box2d allows multiple fixtures, but we will only allow 1 for simplicity
			
			// add rigidbody and body to the map
			m_Rigidbodies.emplace(&rigidbody, body);
		}

		m_HasBegunPlay = true;
	}

	void Scene::Update(float deltaTime)
	{
		// Draw every sprite renderer component in scene
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

		// Draw every box renderer component in scene
		auto boxGroup = m_Registry.group<BoxRendererComponent>(entt::get<TransformComponent>);
		for (entt::entity entity : boxGroup)
		{
			auto [boxRenderer, transform] = boxGroup.get<BoxRendererComponent, TransformComponent>(entity);
			Renderer2D::DrawQuad(boxRenderer.DrawMode, transform.Transform, boxRenderer.Size, boxRenderer.Pivot, boxRenderer.Color);
		}

		// Draw every line renderer component in scene
		auto lineGroup = m_Registry.group<LineRendererComponent>(entt::get<TransformComponent>);
		for (entt::entity entity : lineGroup)
		{
			auto [lineRenderer, transform] = lineGroup.get<LineRendererComponent, TransformComponent>(entity);
			Renderer2D::DrawLine(transform.Transform, lineRenderer.Point1, lineRenderer.Point2, lineRenderer.LineWidth, lineRenderer.Color);
		}

		// Simulate physics in scene
		m_PhysicWorld->Step(deltaTime, 8, 3);
		PostPhysicsUpdate(deltaTime);
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

	void Scene::PostPhysicsUpdate(float timeStep)
	{
		auto rigidbodyGroup = m_Registry.group<RigidbodyComponent>(entt::get<BoxCollider2DComponent, TransformComponent>);
		for (entt::entity entity : rigidbodyGroup)
		{
			auto [rigidbody, collider, transform] = rigidbodyGroup.get<RigidbodyComponent, BoxCollider2DComponent, TransformComponent>(entity);

			b2Body* body = m_Rigidbodies[&rigidbody];
			transform.Transform.Position = { body->GetPosition().x, body->GetPosition().y, transform.Transform.Position.Z };
			transform.Transform.Rotation.Z = glm::degrees(body->GetAngle());
		}

		// "physics debug"
		for (entt::entity entity : rigidbodyGroup)
		{
			auto [rigidbody, collider, transform] = rigidbodyGroup.get<RigidbodyComponent, BoxCollider2DComponent, TransformComponent>(entity);
			Renderer2D::DrawQuad(transform.Transform, collider.Size, collider.Pivot);
		}
	}

	const std::vector<Actor>& Scene::GetActors() const
	{
		return m_Actors;
	}

}
