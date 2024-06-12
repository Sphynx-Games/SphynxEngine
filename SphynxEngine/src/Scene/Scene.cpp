#include "Scene.h"
#include "Actor.h"
#include "Component/Components.h"
#include "Renderer/Renderer2D.h"
#include "Physics/Physics2D.h"
#include "Physics/PhysicsWorld2D.h"
#include "Physics/Collider2D.h"
#include "Physics/Physics2DRenderer.h"
#include "Core/Delegate.h"


namespace Sphynx
{
	Scene::Scene() :
		m_HasBegunPlay(false),
		m_Registry(),
		m_Actors(),
		m_PhysicsWorld(nullptr)
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::BeginPlay()
	{
		if (m_HasBegunPlay) return;

		InitPhysics();
		
		m_HasBegunPlay = true;
	}

	void Scene::EndPlay()
	{
		Physics2D::DestroyPhysicsWorld(m_PhysicsWorld);
		m_PhysicsWorld = nullptr;
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

		// Simulate PHYSICS in scene
		if (m_PhysicsWorld != nullptr)
		{
			Physics2D::Step(m_PhysicsWorld, deltaTime);
		}
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

	void Scene::InitPhysics()
	{
		m_PhysicsWorld = Physics2D::CreatePhysicsWorld();

		auto CreateBody = [&](Rigidbody2DComponent& rigidbody, Collider2D* collider, const TransformComponent& transform)
			{
				RigidbodyDef def = RigidbodyDef();
				def.Enabled = rigidbody.IsEnabled();
				def.Type = rigidbody.GetRigidbodyType();
				def.LinearVelocity = rigidbody.GetLinearVelocity();
				def.AngularVelocity = rigidbody.GetAngularVelocity();
				def.LinearDamping = rigidbody.GetLinearDamping();
				def.AngularDamping = rigidbody.GetAngularDamping();
				def.GravityScale = rigidbody.GetGravityScale();
				def.Transform = transform.Transform;
				Rigidbody2D* rb = Physics2D::CreateRigidbody(def);
				Physics2D::AddRigidbody(m_PhysicsWorld, rb);
				rigidbody.m_Rigidbody = rb;
				Physics2D::AddCollider(rb, collider);
			};

		// BOX
		auto boxGroup = m_Registry.group<BoxCollider2DComponent>(entt::get<Rigidbody2DComponent, TransformComponent>);
		for (entt::entity entity : boxGroup)
		{
			auto [collider, rigidbody, transform] = boxGroup.get<BoxCollider2DComponent, Rigidbody2DComponent, TransformComponent>(entity);

			BoxCollider2D* collider2D = Physics2D::CreateBoxCollider(collider.GetSize(), collider.GetOffset(), collider.IsTrigger());
			collider.m_Collider = collider2D;
			CreateBody(rigidbody, collider2D, transform);
		}

		// CIRCLE
		auto circleGroup = m_Registry.group<CircleCollider2DComponent>(entt::get<Rigidbody2DComponent, TransformComponent>);
		for (entt::entity entity : circleGroup)
		{
			auto [collider, rigidbody, transform] = circleGroup.get<CircleCollider2DComponent, Rigidbody2DComponent, TransformComponent>(entity);

			CircleCollider2D* collider2D = Physics2D::CreateCircleCollider(collider.GetRadius(), collider.GetOffset(), collider.IsTrigger());
			collider.m_Collider = collider2D;
			CreateBody(rigidbody, collider2D, transform);
		}

		// CAPSULE
		auto capsuleGroup = m_Registry.group<CapsuleCollider2DComponent>(entt::get<Rigidbody2DComponent, TransformComponent>);
		for (entt::entity entity : capsuleGroup)
		{
			auto [collider, rigidbody, transform] = capsuleGroup.get<CapsuleCollider2DComponent, Rigidbody2DComponent, TransformComponent>(entity);

			CapsuleCollider2D* collider2D = Physics2D::CreateCapsuleCollider(collider.GetSize(), collider.GetOffset(), collider.IsTrigger());
			collider.m_Collider = collider2D;
			CreateBody(rigidbody, collider2D, transform);
		}

		// post step => update transform values
		m_PhysicsWorld->OnPostStepPhysics.Bind([&]() {
			auto UpdateTransform = [](const auto& group)
				{
					for (entt::entity entity : group)
					{
						auto [rigidbody, transform] = group.get<Rigidbody2DComponent, TransformComponent>(entity);
						
						if (rigidbody.m_Rigidbody != nullptr)
						{
							Vector2f currentPos = rigidbody.m_Rigidbody->GetPosition();
							float currentRot = rigidbody.m_Rigidbody->GetRotation();
							transform.Transform.Position.X = currentPos.X;
							transform.Transform.Position.Y = currentPos.Y;
							transform.Transform.Rotation.Z = currentRot;
						}
					}
				};

			auto boxGroup = m_Registry.group<BoxCollider2DComponent>(entt::get<Rigidbody2DComponent, TransformComponent>);
			UpdateTransform(boxGroup);

			auto circleGroup = m_Registry.group<CircleCollider2DComponent>(entt::get<Rigidbody2DComponent, TransformComponent>);
			UpdateTransform(circleGroup);

			auto capsuleGroup = m_Registry.group<CapsuleCollider2DComponent>(entt::get<Rigidbody2DComponent, TransformComponent>);
			UpdateTransform(capsuleGroup);

			DebugPhysics();
		});
	}

	void Scene::DebugPhysics()
	{
		auto boxGroup = m_Registry.group<BoxCollider2DComponent>(entt::get<Rigidbody2DComponent, TransformComponent>);
		for (entt::entity entity : boxGroup)
		{
			auto [collider, transform] = boxGroup.get<BoxCollider2DComponent, TransformComponent>(entity);

			if (collider.NeedsDebug())
			{
				Physics2DRenderer::DrawBoxCollider(collider.m_Collider, transform.Transform);
			}
		}

		auto circleGroup = m_Registry.group<CircleCollider2DComponent>(entt::get<Rigidbody2DComponent, TransformComponent>);
		for (entt::entity entity : circleGroup)
		{
			auto [collider, transform] = circleGroup.get<CircleCollider2DComponent, TransformComponent>(entity);
			if (collider.NeedsDebug())
			{
				Physics2DRenderer::DrawCircleCollider(collider.m_Collider, transform.Transform);
			}
		}

		auto capsuleGroup = m_Registry.group<CapsuleCollider2DComponent>(entt::get<Rigidbody2DComponent, TransformComponent>);
		for (entt::entity entity : capsuleGroup)
		{
			auto [collider, transform] = capsuleGroup.get<CapsuleCollider2DComponent, TransformComponent>(entity);

			if (collider.NeedsDebug())
			{
				Physics2DRenderer::DrawCapsuleCollider(collider.m_Collider, transform.Transform);
			}
		}
	}

}
