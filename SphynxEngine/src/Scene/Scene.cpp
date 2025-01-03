#include "spxpch.h"
#include "Scene.h"
#include "SceneRenderer.h"
#include "Actor.h"
#include "Component/Components.h"
#include "Renderer/Renderer2D.h"
#include "Physics/Physics2D.h"
#include "Physics/PhysicsWorld2D.h"
#include "Physics/Collider2D.h"
#include "Physics/Physics2DRenderer.h"
#include "Serialization/SceneSerialiZer.h"
#include "Serialization/SceneDeserializer.h"
#include "Asset/AssetManager.h"
#include "Renderer/Sprite.h"
#include "Core/Application.h"
#include "Renderer/Window.h"


namespace Sphynx
{
	void Scene::CloneRegistry(const Scene& other)
	{
		auto& sourceRegistry = other.m_Registry;
		auto& targetRegistry = m_Registry;
		std::unordered_map<UUID, entt::entity> enttMap;

		// create entities
		auto idView = sourceRegistry.view<UUIDComponent>();
		for (auto& e : idView)
		{
			UUID uuid = sourceRegistry.get<UUIDComponent>(e).UUID;

			Actor& actor = CreateActor();
			actor.GetComponent<UUIDComponent>().UUID = uuid;
			enttMap[uuid] = (entt::entity)actor;
		}

		// copy components
		CopyComponent<NameComponent>(sourceRegistry, targetRegistry, enttMap);
		CopyComponent<TransformComponent>(sourceRegistry, targetRegistry, enttMap);
		CopyComponent<CameraComponent>(sourceRegistry, targetRegistry, enttMap);
		CopyComponent<LineRendererComponent>(sourceRegistry, targetRegistry, enttMap);
		CopyComponent<SpriteRendererComponent>(sourceRegistry, targetRegistry, enttMap);
		CopyComponent<BoxRendererComponent>(sourceRegistry, targetRegistry, enttMap);
		CopyComponent<Rigidbody2DComponent>(sourceRegistry, targetRegistry, enttMap);
		CopyComponent<BoxCollider2DComponent>(sourceRegistry, targetRegistry, enttMap);
		CopyComponent<CircleCollider2DComponent>(sourceRegistry, targetRegistry, enttMap);
		CopyComponent<CapsuleCollider2DComponent>(sourceRegistry, targetRegistry, enttMap);
	}

	Scene::Scene() :
		m_UUID(UUID::Generate()),
		m_Name("Default Scene"),
		m_HasBegunPlay(false),
		m_Registry(),
		m_Actors(),
		m_PhysicsWorld(nullptr)
	{
	}

	Scene::Scene(std::string name) :
		m_UUID(UUID::Generate()),
		m_Name(name),
		m_HasBegunPlay(false),
		m_Registry(),
		m_Actors(),
		m_PhysicsWorld(nullptr)
	{
	}

	Scene::Scene(const Scene& other) :
		m_UUID(other.m_UUID),
		m_Name(other.m_Name),
		m_HasBegunPlay(false),
		m_Registry(),
		m_Actors(),
		m_PhysicsWorld(nullptr)
	{
		CloneRegistry(other);
	}

	Scene::~Scene()
	{
	}

	Scene& Scene::operator=(const Scene& other)
	{
		SPX_CORE_ASSERT(!m_HasBegunPlay, "Scene must not be playing!!");
		m_UUID = other.m_UUID;
		m_Name = other.m_Name;
		m_HasBegunPlay = false;
		m_PhysicsWorld = nullptr;
		m_Registry.clear();
		m_Actors.clear();
		CloneRegistry(other);
		return *this;
	}

	void Scene::BeginPlay()
	{
		if (m_HasBegunPlay) return;

		InitPhysics();
		
		m_HasBegunPlay = true;
	}

	void Scene::EndPlay()
	{
		if (!m_HasBegunPlay) return;

		m_HasBegunPlay = false;

		if (m_PhysicsWorld != nullptr)
		{
			Physics2D::DestroyPhysicsWorld(m_PhysicsWorld);
			m_PhysicsWorld = nullptr;
		}
	}

	void Scene::Update(float deltaTime)
	{
		// Simulate PHYSICS in scene
		if (m_PhysicsWorld != nullptr)
		{
			Physics2D::Step(m_PhysicsWorld, deltaTime);
		}
	}

	Actor& Scene::CreateActor()
	{
		Actor& actor = m_Actors.emplace_back(m_Registry.create(), this);
		actor.AddComponent<UUIDComponent>(UUID::Generate());
		return actor;
	}

	void Scene::DestroyActor(const Actor& actor)
	{
		m_Registry.destroy(actor);

		auto it = std::find(m_Actors.begin(), m_Actors.end(), actor);
		if (it != m_Actors.end())
		{
			m_Actors.erase(it);
		}
	}

	const std::vector<Actor>& Scene::GetActors() const
	{
		return m_Actors;
	}

	void Scene::InitPhysics()
	{
		// Helper lambda functions
		auto CreateBody = [&](Rigidbody2DComponent* rigidbody, Collider2D* collider, const TransformComponent& transform)
			{
				RigidbodyDef def = RigidbodyDef();
				def.Transform = transform.Transform;
				if (rigidbody != nullptr)
				{
					def.Enabled = rigidbody->IsEnabled();
					def.Type = rigidbody->GetRigidbodyType();
					def.LinearVelocity = rigidbody->GetLinearVelocity();
					def.AngularVelocity = rigidbody->GetAngularVelocity();
					def.LinearDamping = rigidbody->GetLinearDamping();
					def.AngularDamping = rigidbody->GetAngularDamping();
					def.GravityScale = rigidbody->GetGravityScale();
					
					Rigidbody2D* rb = Physics2D::CreateRigidbody(def);
					Physics2D::AddRigidbody(m_PhysicsWorld, rb);
					rigidbody->m_Rigidbody = rb;
					Physics2D::AddCollider(rb, collider);
				}
				else
				{
					Physics2D::AddCollider(m_PhysicsWorld, collider, def.Transform);
				}
			};
		
		auto CreateBox = [&](Rigidbody2DComponent* rigidbody, BoxCollider2DComponent& collider, const TransformComponent& transform)
			{
				BoxCollider2D* collider2D = Physics2D::CreateBoxCollider(collider.GetSize(), collider.GetOffset(), collider.IsTrigger());
				collider.m_Collider = collider2D;
				CreateBody(rigidbody, collider2D, transform);
			};

		auto CreateCircle = [&](Rigidbody2DComponent* rigidbody, CircleCollider2DComponent& collider, const TransformComponent& transform)
			{
				CircleCollider2D* collider2D = Physics2D::CreateCircleCollider(collider.GetRadius(), collider.GetOffset(), collider.IsTrigger());
				collider.m_Collider = collider2D;
				CreateBody(rigidbody, collider2D, transform);
			};

		auto CreateCapsule = [&](Rigidbody2DComponent* rigidbody, CapsuleCollider2DComponent& collider, const TransformComponent& transform)
			{
				CapsuleCollider2D* collider2D = Physics2D::CreateCapsuleCollider(collider.GetSize(), collider.GetOffset(), collider.IsTrigger());
				collider.m_Collider = collider2D;
				CreateBody(rigidbody, collider2D, transform);
			};

		
		// 1. Create PhysicsWorld
		m_PhysicsWorld = Physics2D::CreatePhysicsWorld(Physics2D::WorldGravity);


		// 2. Create rigidbodies and colliders
		// -- BOX
		auto boxGroup = m_Registry.group<BoxCollider2DComponent>(entt::get<Rigidbody2DComponent, TransformComponent>);
		for (entt::entity entity : boxGroup)
		{
			auto [collider, rigidbody, transform] = boxGroup.get<BoxCollider2DComponent, Rigidbody2DComponent, TransformComponent>(entity);
			CreateBox(&rigidbody, collider, transform);
		}

		auto boxView = m_Registry.view<BoxCollider2DComponent, TransformComponent>();
		for (auto entity : boxView)
		{
			if (!m_Registry.all_of<Rigidbody2DComponent>(entity))
			{
				auto [collider, transform] = boxView.get<BoxCollider2DComponent, TransformComponent>(entity);
				CreateBox(nullptr, collider, transform);
			}
		}

		// -- CIRCLE
		auto circleGroup = m_Registry.group<CircleCollider2DComponent>(entt::get<Rigidbody2DComponent, TransformComponent>);
		for (entt::entity entity : circleGroup)
		{
			auto [collider, rigidbody, transform] = circleGroup.get<CircleCollider2DComponent, Rigidbody2DComponent, TransformComponent>(entity);
			CreateCircle(&rigidbody, collider, transform);
		}

		auto circleView = m_Registry.view<CircleCollider2DComponent, TransformComponent>();
		for (auto entity : circleView)
		{
			if (!m_Registry.all_of<Rigidbody2DComponent>(entity))
			{
				auto [collider, transform] = circleView.get<CircleCollider2DComponent, TransformComponent>(entity);
				CreateCircle(nullptr, collider, transform);
			}
		}

		// -- CAPSULE
		auto capsuleGroup = m_Registry.group<CapsuleCollider2DComponent>(entt::get<Rigidbody2DComponent, TransformComponent>);
		for (entt::entity entity : capsuleGroup)
		{
			auto [collider, rigidbody, transform] = capsuleGroup.get<CapsuleCollider2DComponent, Rigidbody2DComponent, TransformComponent>(entity);
			CreateCapsule(&rigidbody, collider, transform);
		}

		auto capsuleView = m_Registry.view<CapsuleCollider2DComponent, TransformComponent>();
		for (auto entity : capsuleView)
		{
			if (!m_Registry.all_of<Rigidbody2DComponent>(entity))
			{
				auto [collider, transform] = capsuleView.get<CapsuleCollider2DComponent, TransformComponent>(entity);
				CreateCapsule(nullptr, collider, transform);
			}
		}


		// 3. Post step => update transform values
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
		// BOX
		auto boxView = m_Registry.view<BoxCollider2DComponent, TransformComponent>();
		for (auto entity : boxView)
		{
			auto [collider, transform] = boxView.get<BoxCollider2DComponent, TransformComponent>(entity);
			if (collider.NeedsDebug())
			{
				Physics2DRenderer::DrawBoxCollider(collider.m_Collider, transform.Transform);
			}
		}

		// CIRCLE
		auto circleView = m_Registry.view<CircleCollider2DComponent, TransformComponent>();
		for (auto entity : circleView)
		{
			auto [collider, transform] = circleView.get<CircleCollider2DComponent, TransformComponent>(entity);
			if (collider.NeedsDebug())
			{
				Physics2DRenderer::DrawCircleCollider(collider.m_Collider, transform.Transform);
			}
		}

		// CAPSULE
		auto capsuleView = m_Registry.view<CapsuleCollider2DComponent, TransformComponent>();
		for (auto entity : capsuleView)
		{
			auto [collider, transform] = capsuleView.get<CapsuleCollider2DComponent, TransformComponent>(entity);
			if (collider.NeedsDebug())
			{
				Physics2DRenderer::DrawCapsuleCollider(collider.m_Collider, transform.Transform);
			}
		}
	}

}
