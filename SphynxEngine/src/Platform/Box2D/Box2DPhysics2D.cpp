#include "Physics/Physics2D.h"
#include "Component/Physics2DComponents.h"
#include "Component/TransformComponent.h"
#include "Scene/Scene.h"
#include "Renderer/Renderer2D.h"
#include "Math/Transform.h"

#include "entt/entt.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/matrix_decompose.hpp"
#include "glm/gtc/quaternion.hpp"

#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_circle_shape.h"

namespace Sphynx
{
	class Physics2DScene
	{
	public: 
		Physics2DScene() : m_PhysicsWorld(new b2World({ 0.0f, -10.0f })) {}

	private:
		b2World* m_PhysicsWorld;
		std::unordered_map<RigidbodyComponent*, b2Body*> m_Rigidbodies;

		friend class Physics2D;
	};

	extern const glm::mat4 GetModelMatrixFromTransform(const Transform& transform);

	Vector2f RotatePointAboutOtherPoint(Vector2f point, Vector2f otherPoint, float angleInRadians)
	{
		// R.x = (P.x - C.x) * Math.cos(a) - (P.y - C.y) * Math.sin(a) + C.x
		// R.y = (P.x - C.x) * Math.sin(a) + (P.y - C.y) * Math.cos(a) + C.y

		// translate point back to origin: (P.x - C.x)
		// rotate point:                   * Math.cos(a) - (P.y - C.y) * Math.sin(a)
		// translate point back:           + C.x

		float sinValue = sin(angleInRadians);
		float cosValue = cos(angleInRadians);

		Vector2f substraction = { otherPoint.X - point.X, otherPoint.Y - point.Y };

		return {
			substraction.X * cosValue - substraction.Y * sinValue + point.X,
			substraction.X * sinValue + substraction.Y * cosValue + point.Y
		};
	}

	b2BodyType RigidbodyType_To_Box2D(RigidbodyType type)
	{
		switch (type)
		{
		case RigidbodyType::STATIC:     return b2BodyType::b2_staticBody;
		case RigidbodyType::DYNAMIC:    return b2BodyType::b2_dynamicBody;
		case RigidbodyType::KINEMATIC:  return b2BodyType::b2_kinematicBody;
		}
	}

	b2BodyDef CreateBodyDef(const RigidbodyComponent& rigidbody, const TransformComponent& transform)
	{
		// body definition
		b2BodyDef bodyDef;
		bodyDef.type = RigidbodyType_To_Box2D(rigidbody.RigidbodyType);
		bodyDef.position = { transform.Transform.Position.X, transform.Transform.Position.Y };
		bodyDef.angle = glm::radians(transform.Transform.Rotation.Z);
		return bodyDef;
	}


	Physics2DScene* Physics2D::CreatePhysics2DScene()
	{
		return new Physics2DScene();
	}

	Physics2DScene* Physics2D::CreatePhysics2DScene(Scene& scene)
	{
		Physics2DScene* physiscsScene = new Physics2DScene();

		// BOX COLLIDERS
		auto boxGroup = scene.m_Registry.group<BoxCollider2DComponent>(entt::get<RigidbodyComponent, TransformComponent>);
		for (entt::entity entity : boxGroup)
		{
			auto [collider, rigidbody, transform] = boxGroup.get<BoxCollider2DComponent, RigidbodyComponent, TransformComponent>(entity);

			// body definition
			b2BodyDef bodyDef = CreateBodyDef(rigidbody, transform);

			// body, shape of a box and fixture
			b2PolygonShape shape;
			shape.SetAsBox(
				(transform.Transform.Scale.X * collider.Size.X) / 2.0f,
				(transform.Transform.Scale.Y * collider.Size.Y) / 2.0f//,
				//{ 0.0f, 0.0f }, // { collider.Pivot.X, collider.Pivot.Y }, // TODO: take pivot into account
				//0.0f
			);

			b2Body* body = physiscsScene->m_PhysicsWorld->CreateBody(&bodyDef);
			body->CreateFixture(&shape, 1.0f);

			// add rigidbody and body to the map
			physiscsScene->m_Rigidbodies.emplace(&rigidbody, body);
		}

		// CIRCLE COLLIDERS
		auto circleGroup = scene.m_Registry.group<CircleCollider2DComponent>(entt::get<RigidbodyComponent, TransformComponent>);
		for (entt::entity entity : circleGroup)
		{
			auto [collider, rigidbody, transform] = circleGroup.get<CircleCollider2DComponent, RigidbodyComponent, TransformComponent>(entity);

			// body definition
			b2BodyDef bodyDef = CreateBodyDef(rigidbody, transform);

			// body, shape of a circle and fixture
			b2CircleShape shape;
			shape.m_p = { 0.0f, 0.0f }; // TODO: take pivot into account
			shape.m_radius = collider.Radius * std::max(transform.Transform.Scale.X, transform.Transform.Scale.Y);

			b2Body* body = physiscsScene->m_PhysicsWorld->CreateBody(&bodyDef);
			body->CreateFixture(&shape, 1.0f);

			// add rigidbody and body to the map
			physiscsScene->m_Rigidbodies.emplace(&rigidbody, body);
		}

		// CAPSULE COLLIDERS
		auto capsuleGroup = scene.m_Registry.group<CapsuleCollider2DComponent>(entt::get<RigidbodyComponent, TransformComponent>);
		for (entt::entity entity : capsuleGroup)
		{
			auto [collider, rigidbody, transform] = capsuleGroup.get<CapsuleCollider2DComponent, RigidbodyComponent, TransformComponent>(entity);

			// body definition
			b2BodyDef bodyDef = CreateBodyDef(rigidbody, transform);

			// body, shape and fixtures
			Vector2f capsuleSize = collider.Size * Vector2f{ transform.Transform.Scale.X, transform.Transform.Scale.Y };
			Vector2f boxSize =
			{
				(capsuleSize.X > capsuleSize.Y) ? capsuleSize.X - capsuleSize.Y : capsuleSize.X,
				(capsuleSize.X > capsuleSize.Y) ? capsuleSize.Y : capsuleSize.Y - capsuleSize.X
			};

			b2CircleShape circleA; // right, up
			circleA.m_p = (capsuleSize.X > capsuleSize.Y) ? b2Vec2{ boxSize.X / 2.0f, 0.0f } : b2Vec2{ 0.0f, boxSize.Y / 2.0f };
			circleA.m_radius = (capsuleSize.X > capsuleSize.Y) ? boxSize.Y / 2.0f : boxSize.X / 2.0f;
			b2CircleShape circleB; // left, down
			circleB.m_p = (capsuleSize.X > capsuleSize.Y) ? b2Vec2{ -boxSize.X / 2.0f, 0.0f } : b2Vec2{ 0.0f, -boxSize.Y / 2.0f };
			circleB.m_radius = (capsuleSize.X > capsuleSize.Y) ? boxSize.Y / 2.0f : boxSize.X / 2.0f;

			b2PolygonShape box;
			box.SetAsBox(
				boxSize.X / 2.0f,
				boxSize.Y / 2.0f//,
				//{ 0.0f, 0.0f }, // { collider.Pivot.X, collider.Pivot.Y },
				//0.0f
			);

			b2Body* body = physiscsScene->m_PhysicsWorld->CreateBody(&bodyDef);
			body->CreateFixture(&box, 1.0f);
			body->CreateFixture(&circleA, 1.0f);
			body->CreateFixture(&circleB, 1.0f);

			// add rigidbody and body to the map
			physiscsScene->m_Rigidbodies.emplace(&rigidbody, body);
		}

		return physiscsScene;
	}

	void Physics2D::DestroyPhysics2DScene(Physics2DScene* physiscsScene)
	{
		delete physiscsScene->m_PhysicsWorld;
		physiscsScene->m_PhysicsWorld = nullptr;
		delete physiscsScene;
	}

	void Physics2D::Step(Physics2DScene* physicsScene, float timeStep)
	{
		physicsScene->m_PhysicsWorld->Step(timeStep, 8, 3);
	}

	void Physics2D::Step(Scene& scene, float timeStep)
	{
		Step(scene.m_PhysicsScene, timeStep);
		PostStep(scene, timeStep);
	}

	void Physics2D::PostStep(Scene& scene, float timeStep)
	{
		auto SyncBodyTransform = [](Scene& scene, const auto& group)
			{
				for (entt::entity entity : group)
				{
					auto [rigidbody, transform] = group.get<RigidbodyComponent, TransformComponent>(entity);

					b2Body* body = scene.m_PhysicsScene->m_Rigidbodies[&rigidbody];
					transform.Transform.Position = { body->GetPosition().x, body->GetPosition().y, transform.Transform.Position.Z };
					transform.Transform.Rotation.Z = glm::degrees(body->GetAngle());
				}
			};
		
		auto boxGroup = scene.m_Registry.group<BoxCollider2DComponent>(entt::get<RigidbodyComponent, TransformComponent>);
		SyncBodyTransform(scene, boxGroup);

		auto circleGroup = scene.m_Registry.group<CircleCollider2DComponent>(entt::get<RigidbodyComponent, TransformComponent>);
		SyncBodyTransform(scene, circleGroup);

		auto capsuleGroup = scene.m_Registry.group<CapsuleCollider2DComponent>(entt::get<RigidbodyComponent, TransformComponent>);
		SyncBodyTransform(scene, capsuleGroup);


		// "physics debug"
		for (entt::entity entity : boxGroup)
		{
			auto [collider, transform] = boxGroup.get<BoxCollider2DComponent, TransformComponent>(entity);
			Renderer2D::DrawQuad(transform.Transform, collider.Size, collider.Pivot);
		}

		for (entt::entity entity : circleGroup)
		{
			auto [collider, transform] = circleGroup.get<CircleCollider2DComponent, TransformComponent>(entity);
			Renderer2D::DrawCircle(transform.Transform, collider.Radius, 32u, collider.Pivot);
		}

		for (entt::entity entity : capsuleGroup)
		{
			auto [collider, rigidbody, transform] = capsuleGroup.get<CapsuleCollider2DComponent, RigidbodyComponent, TransformComponent>(entity);

			b2PolygonShape* polygonShape;
			std::vector<b2CircleShape*> circleShapes;
			circleShapes.reserve(2);

			// identify each fixture
			for (b2Fixture* fixture = scene.m_PhysicsScene->m_Rigidbodies[&rigidbody]->GetFixtureList(); fixture; fixture = fixture->GetNext())
			{
				b2Shape::Type shapeType = fixture->GetType();
				if (shapeType == b2Shape::e_polygon)
				{
					polygonShape = (b2PolygonShape*)fixture->GetShape();
				}
				else if (shapeType == b2Shape::e_circle)
				{
					circleShapes.push_back((b2CircleShape*)fixture->GetShape());
				}
			}

			// draw each fixture
			Vector2f capsuleSize = collider.Size * Vector2f{ transform.Transform.Scale.X, transform.Transform.Scale.Y };
			Vector2f boxSize =
			{
				(capsuleSize.X > capsuleSize.Y) ? capsuleSize.X - capsuleSize.Y : capsuleSize.X,
				(capsuleSize.X > capsuleSize.Y) ? capsuleSize.Y : capsuleSize.Y - capsuleSize.X
			};
			boxSize /= Vector2f{ transform.Transform.Scale.X, transform.Transform.Scale.Y };
			Renderer2D::DrawQuad(transform.Transform, boxSize); // TODO: take into account Pivot or Offset

			for (b2CircleShape* circleShape : circleShapes)
			{
				Transform local = {
					Vector3f{ circleShape->m_p.x, circleShape->m_p.y, 0.0f } / transform.Transform.Scale,
					Vector3f(1.0f) / transform.Transform.Scale,
					{ 0.0f, 0.0f, 0.0f }
				};
				glm::mat4 localMatrix = GetModelMatrixFromTransform(local);
				glm::mat4 worldMatrix = GetModelMatrixFromTransform(transform.Transform);
				glm::mat4 circleWorldMatrix = worldMatrix * localMatrix;

				glm::vec3 scale;
				glm::quat orientation;
				glm::vec3 trans;
				glm::vec3 skew;
				glm::vec4 perspective;
				glm::decompose(circleWorldMatrix, scale, orientation, trans, skew, perspective);

				glm::vec3 rotation = glm::eulerAngles(orientation);

				Transform circleTransform;
				circleTransform.Position = { trans.x, trans.y, trans.z };
				circleTransform.Rotation = { glm::degrees(rotation.x), glm::degrees(rotation.y), glm::degrees(rotation.z) };
				circleTransform.Scale = { scale.x, scale.y, scale.z };

				Renderer2D::DrawCircle(circleTransform, circleShape->m_radius * std::min(collider.Size.X, collider.Size.Y));
			}

			//Renderer2D::DrawQuad(transform.Transform, collider.Size, collider.Pivot, Color::Green);

		}
	}
}