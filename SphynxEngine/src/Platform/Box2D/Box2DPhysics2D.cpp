#include "Physics/Physics2D.h"
#include "Box2DContactListener.h"
#include "Component/Physics/Physics2DComponents.h"
#include "Component/TransformComponent.h"
#include "Scene/Scene.h"
#include "Renderer/Renderer2D.h"
#include "Math/Transform.h"
#include "Core/Delegate.h"

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
		Physics2DScene() :
			m_PhysicsWorld(b2World({ 0.0f, -10.0f })),
			m_contactListener()
		{
			m_PhysicsWorld.SetContactListener(&m_contactListener);
		}

		~Physics2DScene()
		{
			OnPostStepPhysics.Unbind();
		}

	public:
		Delegate<void()> OnPostStepPhysics;

	private:
		b2World m_PhysicsWorld;
		Box2DContactListener m_contactListener;

		friend class Physics2D;
	};

	class Rigidbody2D
	{
	public:
		Rigidbody2D() : m_Body() {}
		Rigidbody2D(b2Body* body) : m_Body(body) {}

		~Rigidbody2D()
		{
		}

	private:
		b2Body* m_Body;

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

		SPX_CORE_ASSERT(false);
		return b2BodyType::b2_staticBody;
	}

	RigidbodyType Box2D_To_RigidbodyType(b2BodyType type)
	{
		switch (type)
		{
		case b2BodyType::b2_staticBody:     return RigidbodyType::STATIC;
		case b2BodyType::b2_dynamicBody:    return RigidbodyType::DYNAMIC;
		case b2BodyType::b2_kinematicBody:  return RigidbodyType::KINEMATIC;
		}

		SPX_CORE_ASSERT(false);
		return RigidbodyType::STATIC;
	}


	Physics2DScene* Physics2D::CreatePhysics2DScene()
	{
		return new Physics2DScene();
	}

	void Physics2D::DestroyPhysics2DScene(Physics2DScene* physiscsScene)
	{
		delete physiscsScene;
	}

	Rigidbody2D* Physics2D::CreateRigidbody(Physics2DScene* physicsScene, Collider2D* collider, RigidbodyDef& rigidbodyDef)
	{
		if (physicsScene == nullptr) return nullptr;

		b2BodyDef bodyDef;
		bodyDef.enabled = rigidbodyDef.Enabled;
		bodyDef.type = RigidbodyType_To_Box2D(rigidbodyDef.Type);
		bodyDef.linearVelocity = { rigidbodyDef.LinearVelocity.X, rigidbodyDef.LinearVelocity.Y };
		bodyDef.angularVelocity = rigidbodyDef.AngularVelocity;
		bodyDef.linearDamping = rigidbodyDef.LinearDamping;
		bodyDef.angularDamping = rigidbodyDef.AngularDamping;
		bodyDef.gravityScale = rigidbodyDef.GravityScale;
		bodyDef.position = { rigidbodyDef.Transform.Position.X, rigidbodyDef.Transform.Position.Y };
		bodyDef.angle = glm::radians(rigidbodyDef.Transform.Rotation.Z);

		b2Body* body = physicsScene->m_PhysicsWorld.CreateBody(&bodyDef);

		if (collider == nullptr)
		{
			Rigidbody2D* rigidbody = new Rigidbody2D(body);
			return rigidbody;
		}

		// BOX COLLIDERS
		if (BoxCollider2D* collider2D = dynamic_cast<BoxCollider2D*>(collider))
		{
			b2PolygonShape shape;
			shape.SetAsBox(
				(collider2D->Size.X * rigidbodyDef.Transform.Scale.X) / 2.0f,
				(collider2D->Size.Y * rigidbodyDef.Transform.Scale.Y) / 2.0f,
				{ collider2D->Offset.X * rigidbodyDef.Transform.Scale.X, collider2D->Offset.Y * rigidbodyDef.Transform.Scale.Y },
				0.0f
			);

			body->CreateFixture(&shape, 1.0f);
		}

		// CIRCLE COLLIDERS
		else if (CircleCollider2D* collider2D = dynamic_cast<CircleCollider2D*>(collider))
		{
			b2CircleShape shape;
			shape.m_p = { collider2D->Offset.X * rigidbodyDef.Transform.Scale.X, collider2D->Offset.Y * rigidbodyDef.Transform.Scale.Y };
			shape.m_radius = collider2D->Radius * std::max(rigidbodyDef.Transform.Scale.X, rigidbodyDef.Transform.Scale.Y);

			body->CreateFixture(&shape, 1.0f);
		}

		// CAPSULE COLLIDERS
		else if (CapsuleCollider2D* collider2D = dynamic_cast<CapsuleCollider2D*>(collider))
		{
			Vector2f capsuleSize = collider2D->Size * Vector2f{ rigidbodyDef.Transform.Scale.X, rigidbodyDef.Transform.Scale.Y };
			Vector2f scaledOffset = { collider2D->Offset.X * rigidbodyDef.Transform.Scale.X, collider2D->Offset.Y * rigidbodyDef.Transform.Scale.Y };

			if (capsuleSize.X == capsuleSize.Y)
			{
				// 1 CIRCLE
				b2CircleShape circle;
				circle.m_p = b2Vec2{ scaledOffset.X, scaledOffset.Y };
				circle.m_radius = capsuleSize.X / 2.0f;

				body->CreateFixture(&circle, 1.0f);
			}
			else
			{
				// 1 BOX + 2 CIRCLES
				Vector2f boxSize =
				{
					(capsuleSize.X > capsuleSize.Y) ? capsuleSize.X - capsuleSize.Y : capsuleSize.X,
					(capsuleSize.X > capsuleSize.Y) ? capsuleSize.Y : capsuleSize.Y - capsuleSize.X
				};

				b2PolygonShape box;
				box.SetAsBox(
					boxSize.X / 2.0f,
					boxSize.Y / 2.0f,
					{ scaledOffset.X, scaledOffset.Y },
					0.0f
				);

				b2CircleShape circleA; // right, up
				circleA.m_p = (capsuleSize.X > capsuleSize.Y)
					? b2Vec2{ (boxSize.X / 2.0f) + scaledOffset.X, scaledOffset.Y }
				: b2Vec2{ scaledOffset.X, (boxSize.Y / 2.0f) + scaledOffset.Y };
				circleA.m_radius = (capsuleSize.X > capsuleSize.Y) ? boxSize.Y / 2.0f : boxSize.X / 2.0f;
				b2CircleShape circleB; // left, down
				circleB.m_p = (capsuleSize.X > capsuleSize.Y)
					? b2Vec2{ (-boxSize.X / 2.0f) + scaledOffset.X, scaledOffset.Y }
				: b2Vec2{ scaledOffset.X, (-boxSize.Y / 2.0f) + scaledOffset.Y };
				circleB.m_radius = (capsuleSize.X > capsuleSize.Y) ? boxSize.Y / 2.0f : boxSize.X / 2.0f;

				body->CreateFixture(&box, 1.0f);
				body->CreateFixture(&circleA, 1.0f);
				body->CreateFixture(&circleB, 1.0f);
			}
		}

		Rigidbody2D* rigidbody = new Rigidbody2D(body);
		return rigidbody;
	}

	void Physics2D::DestroyRigidbody(Rigidbody2D* rigidbody)
	{
		SPX_CORE_ASSERT(IsRigidbodyValid(rigidbody), "Rigidbody is not valid!!");

		b2Body* b2body = rigidbody->m_Body;
		b2body->GetWorld()->DestroyBody(b2body);
		rigidbody->m_Body = nullptr;
		delete rigidbody;
	}

	void Physics2D::Step(Physics2DScene* physicsScene, float timeStep)
	{
		SPX_CORE_ASSERT(physicsScene != nullptr, "Physics2DScene is nullptr!!");

		physicsScene->m_PhysicsWorld.Step(timeStep, 8, 3);
		physicsScene->OnPostStepPhysics.Execute();
	}

	Delegate<void()>& Physics2D::GetOnPostStepPhysiscsDelegate(Physics2DScene* physicsScene)
	{
		return physicsScene->OnPostStepPhysics;
	}

	void Physics2D::DebugCollider(const Collider2D* collider, const Transform& transform)
	{
		if (collider == nullptr)
		{
			SPX_CORE_LOG_WARNING("There are no Rigidbodies for the current PhysicsScene!!");
			return;
		}

		auto GetTransformWithOffset = [](const Transform& local, const Transform& transform)
			{
				glm::mat4 localMatrix = GetModelMatrixFromTransform(local);
				glm::mat4 worldMatrix = GetModelMatrixFromTransform(transform);
				glm::mat4 entityWorldMatrix = worldMatrix * localMatrix;

				glm::vec3 scale;
				glm::quat orientation;
				glm::vec3 pos;
				glm::vec3 skew;
				glm::vec4 perspective;
				glm::decompose(entityWorldMatrix, scale, orientation, pos, skew, perspective);

				glm::vec3 rotation = glm::eulerAngles(orientation);

				Transform entityTransform;
				entityTransform.Position = { pos.x, pos.y, pos.z };
				entityTransform.Rotation = { glm::degrees(rotation.x), glm::degrees(rotation.y), glm::degrees(rotation.z) };
				entityTransform.Scale = { scale.x, scale.y, scale.z };
				return entityTransform;
			};

		// BOX COLLIDERS
		if (const BoxCollider2D* collider2D = dynamic_cast<const BoxCollider2D*>(collider))
		{
			Transform local = {
					{ collider2D->Offset.X, collider2D->Offset.Y, 0.0f},
					{ collider2D->Size.X, collider2D->Size.Y, 1.0f},
					{ 0.0f, 0.0f, 0.0f }
			};
			Transform entityTransform = GetTransformWithOffset(local, transform);

			Renderer2D::DrawQuad(entityTransform);
		}

		// CIRCLE COLLIDERS
		else if (const CircleCollider2D* collider2D = dynamic_cast<const CircleCollider2D*>(collider))
		{
			Transform local = {
					{ collider2D->Offset.X, collider2D->Offset.Y, 0.0f },
					{ collider2D->Radius * 2.0f, collider2D->Radius * 2.0f, 1.0f },
					{ 0.0f, 0.0f, 0.0f }
			};
			Transform entityTransform = GetTransformWithOffset(local, transform);
			float maxScale = std::max(entityTransform.Scale.X, entityTransform.Scale.Y);
			entityTransform.Scale.X = maxScale;
			entityTransform.Scale.Y = maxScale;

			Renderer2D::DrawCircle(entityTransform);
		}

		// CAPSULE COLLIDERS
		else if (const CapsuleCollider2D* collider2D = dynamic_cast<const CapsuleCollider2D*>(collider))
		{
			// draw each fixture
			// -- BOX
			Vector2f capsuleSize = collider2D->Size * Vector2f{ transform.Scale.X, transform.Scale.Y };
			Vector2f boxSize =
			{
				(capsuleSize.X > capsuleSize.Y) ? capsuleSize.X - capsuleSize.Y : capsuleSize.X,
				(capsuleSize.X > capsuleSize.Y) ? capsuleSize.Y : capsuleSize.Y - capsuleSize.X
			};
			boxSize /= Vector2f{ transform.Scale.X, transform.Scale.Y };

			Transform local = {
					{ collider2D->Offset.X, collider2D->Offset.Y, 0.0f },
					{ 1.0f, 1.0f, 1.0f },
					{ 0.0f, 0.0f, 0.0f }
			};
			Transform entityTransform = GetTransformWithOffset(local, transform);

			Renderer2D::DrawQuad(entityTransform, boxSize);
			
			// bounding box
			//Renderer2D::DrawQuad(entityTransform, collider2D->Size, { 0.5, 0.5f }, Color::Green);

			// -- CIRCLES
			// right, up
			Transform circleTransformA;
			circleTransformA.Position = (capsuleSize.X > capsuleSize.Y)
				? Vector3f{ (boxSize.X / 2.0f) + collider2D->Offset.X, collider2D->Offset.Y , 0.0f }
			: Vector3f{ collider2D->Offset.X, (boxSize.Y / 2.0f) + collider2D->Offset.Y, 0.0f };
			circleTransformA.Scale /= transform.Scale;
			circleTransformA = GetTransformWithOffset(circleTransformA, transform);
			float circleRadiusA = std::min(capsuleSize.X, capsuleSize.Y) / 2.0f;

			// left, down
			Transform circleTransformB;
			circleTransformB.Position = (capsuleSize.X > capsuleSize.Y)
				? Vector3f{ (-boxSize.X / 2.0f) + collider2D->Offset.X, collider2D->Offset.Y, 0.0f }
			: Vector3f{ collider2D->Offset.X, (-boxSize.Y / 2.0f) + collider2D->Offset.Y, 0.0f };
			circleTransformB.Scale /= transform.Scale;
			circleTransformB = GetTransformWithOffset(circleTransformB, transform);
			float circleRadiusB = std::min(capsuleSize.X, capsuleSize.Y) / 2.0f;

			Renderer2D::DrawCircle(circleTransformA, circleRadiusA);
			Renderer2D::DrawCircle(circleTransformB, circleRadiusB);
		}
	}


	bool Physics2D::IsRigidbodyValid(Rigidbody2D* rigidbody)
	{
		return rigidbody != nullptr && rigidbody->m_Body != nullptr;
	}

	Vector2f Physics2D::GetRigidbodyPosition(Rigidbody2D* rigidbody)
	{
		return Vector2f(rigidbody->m_Body->GetPosition().x, rigidbody->m_Body->GetPosition().y);
	}

	float Physics2D::GetRigidbodyRotation(Rigidbody2D* rigidbody)
	{
		return glm::degrees(rigidbody->m_Body->GetAngle());
	}

	bool Physics2D::IsRigidbodyEnabled(Rigidbody2D* rigidbody)
	{
		SPX_CORE_ASSERT(IsRigidbodyValid(rigidbody), "Rigidbody is not valid!!");

		return rigidbody->m_Body->IsEnabled();
	}

	RigidbodyType Physics2D::GetRigidbodyType(Rigidbody2D* rigidbody)
	{
		SPX_CORE_ASSERT(IsRigidbodyValid(rigidbody), "Rigidbody is not valid!!");

		return Box2D_To_RigidbodyType(rigidbody->m_Body->GetType());
	}

	Vector2f Physics2D::GetRigidbodyLinearVelocity(Rigidbody2D* rigidbody)
	{
		SPX_CORE_ASSERT(IsRigidbodyValid(rigidbody), "Rigidbody is not valid!!");

		b2Vec2 velocity = rigidbody->m_Body->GetLinearVelocity();
		return Vector2f(velocity.x, velocity.y);
	}

	float Physics2D::GetRigidbodyAngularVelocity(Rigidbody2D* rigidbody)
	{
		SPX_CORE_ASSERT(IsRigidbodyValid(rigidbody), "Rigidbody is not valid!!");

		return rigidbody->m_Body->GetAngularVelocity();
	}

	float Physics2D::GetRigidbodyLinearDamping(Rigidbody2D* rigidbody)
	{
		SPX_CORE_ASSERT(IsRigidbodyValid(rigidbody), "Rigidbody is not valid!!");

		return rigidbody->m_Body->GetLinearDamping();
	}

	float Physics2D::GetRigidbodyAngularDamping(Rigidbody2D* rigidbody)
	{
		SPX_CORE_ASSERT(IsRigidbodyValid(rigidbody), "Rigidbody is not valid!!");

		return rigidbody->m_Body->GetAngularDamping();
	}

	float Physics2D::GetRigidbodyGravityScale(Rigidbody2D* rigidbody)
	{
		SPX_CORE_ASSERT(IsRigidbodyValid(rigidbody), "Rigidbody is not valid!!");

		return rigidbody->m_Body->GetGravityScale();
	}

	void Physics2D::SetRigidbodyEnabled(Rigidbody2D* rigidbody, bool enable)
	{
		SPX_CORE_ASSERT(IsRigidbodyValid(rigidbody), "Rigidbody is not valid!!");

		rigidbody->m_Body->SetEnabled(enable);
	}

	void Physics2D::SetRigidbodyType(Rigidbody2D* rigidbody, RigidbodyType type)
	{
		SPX_CORE_ASSERT(IsRigidbodyValid(rigidbody), "Rigidbody is not valid!!");

		rigidbody->m_Body->SetType(RigidbodyType_To_Box2D(type));
	}

	void Physics2D::SetRigidbodyLinearVelocity(Rigidbody2D* rigidbody, Vector2f velocity)
	{
		SPX_CORE_ASSERT(IsRigidbodyValid(rigidbody), "Rigidbody is not valid!!");

		rigidbody->m_Body->SetLinearVelocity({ velocity.X, velocity.Y });
	}

	void Physics2D::SetRigidbodyAngularVelocity(Rigidbody2D* rigidbody, float velocity)
	{
		SPX_CORE_ASSERT(IsRigidbodyValid(rigidbody), "Rigidbody is not valid!!");

		rigidbody->m_Body->SetAngularVelocity(velocity);
	}

	void Physics2D::SetRigidbodyLinearDamping(Rigidbody2D* rigidbody, float damping)
	{
		SPX_CORE_ASSERT(IsRigidbodyValid(rigidbody), "Rigidbody is not valid!!");

		rigidbody->m_Body->SetLinearDamping(damping);
	}

	void Physics2D::SetRigidbodyAngularDamping(Rigidbody2D* rigidbody, float damping)
	{
		SPX_CORE_ASSERT(IsRigidbodyValid(rigidbody), "Rigidbody is not valid!!");

		rigidbody->m_Body->SetAngularDamping(damping);
	}

	void Physics2D::SetRigidbodyGravityScale(Rigidbody2D* rigidbody, float gravityScale)
	{
		SPX_CORE_ASSERT(IsRigidbodyValid(rigidbody), "Rigidbody is not valid!!");

		rigidbody->m_Body->SetGravityScale(gravityScale);
	}

	bool Physics2D::IsColliderTrigger(Collider2D* collider)
	{
		return collider->Trigger;
	}

	void Physics2D::SetColliderIsTrigger(Collider2D* collider, bool trigger)
	{
		collider->Trigger = trigger;
	}

	Vector2f Physics2D::GetColliderOffset(Collider2D* collider)
	{
		return collider->Offset;
	}

	void Physics2D::SetColliderOffset(Collider2D* collider, Vector2f offset)
	{
		collider->Offset = offset;
	}

	Vector2f Physics2D::GetBoxColliderSize(BoxCollider2D* collider)
	{
		return collider->Size;
	}

	void Physics2D::SetBoxColliderSize(BoxCollider2D* collider, Vector2f size)
	{
		collider->Size = size;
	}

	float Physics2D::GetCircleColliderRadius(CircleCollider2D* collider)
	{
		return collider->Radius;
	}

	void Physics2D::SetCircleColliderRadius(CircleCollider2D* collider, float radius)
	{
		collider->Radius = radius;
	}

	Vector2f Physics2D::GetCapsuleColliderSize(CapsuleCollider2D* collider)
	{
		return collider->Size;
	}

	void Physics2D::SetCapsuleColliderSize(CapsuleCollider2D* collider, Vector2f size)
	{
		collider->Size = size;
	}
}