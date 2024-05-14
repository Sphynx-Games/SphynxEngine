#pragma once

#include "Core/Core.h"
#include "Physics2DUtils.h"
#include <unordered_map>
#include <vector>


namespace Sphynx
{
	extern class Physics2DScene;
	extern class Rigidbody2D;
	extern struct RigidbodyData;
	extern struct Collider2D;
	class Scene;

	class SPHYNX_API Physics2D
	{
	public:
		static Physics2DScene* CreatePhysics2DScene();
		static Physics2DScene* CreatePhysics2DScene(Scene& scene);
		static void DestroyPhysics2DScene(Physics2DScene* physicsScene);

		static Rigidbody2D* CreateRigidbody(Physics2DScene* physicsScene, Collider2D* collider, RigidbodyDef& rigidbodyDef, void* rigidbodyData);
		static void DestroyRigidbody(Rigidbody2D* rigidbody);

		static void Step(Physics2DScene* physicsScene, float timeStep);
		static void Step(Scene& scene, float timeStep);

		// getters and setters
		// -- RIGIDBODY
		static bool IsRigidbodyValid(Rigidbody2D* rigidbody);

		static bool IsRigidbodyEnabled(Rigidbody2D* rigidbody);
		static void SetRigidbodyEnabled(Rigidbody2D* rigidbody, bool enable);

		static RigidbodyType GetRigidbodyType(Rigidbody2D* rigidbody);
		static void SetRigidbodyType(Rigidbody2D* rigidbody, RigidbodyType type);

		static Vector2f GetRigidbodyLinearVelocity(Rigidbody2D* rigidbody);
		static void SetRigidbodyLinearVelocity(Rigidbody2D* rigidbody, Vector2f velocity);

		static float GetRigidbodyAngularVelocity(Rigidbody2D* rigidbody);
		static void SetRigidbodyAngularVelocity(Rigidbody2D* rigidbody, float velocity);

		static float GetRigidbodyLinearDamping(Rigidbody2D* rigidbody);
		static void SetRigidbodyLinearDamping(Rigidbody2D* rigidbody, float damping);

		static float GetRigidbodyAngularDamping(Rigidbody2D* rigidbody);
		static void SetRigidbodyAngularDamping(Rigidbody2D* rigidbody, float damping);

		static float GetRigidbodyGravityScale(Rigidbody2D* rigidbody);
		static void SetRigidbodyGravityScale(Rigidbody2D* rigidbody, float gravityScale);

		// -- COLLIDERS
		static bool IsColliderTrigger(Collider2D* collider);
		static void SetColliderIsTrigger(Collider2D* collider, bool trigger);

		static Vector2f GetColliderOffset(Collider2D * collider);
		static void SetColliderOffset(Collider2D * collider, Vector2f offset);

		//		-- BOX
		static Vector2f GetBoxColliderSize(BoxCollider2D* collider);
		static void SetBoxColliderSize(BoxCollider2D* collider, Vector2f size);

		//		-- CIRCLE
		static float GetCircleColliderRadius(CircleCollider2D* collider);
		static void SetCircleColliderRadius(CircleCollider2D* collider, float radius);

		//		-- CAPSULE
		static Vector2f GetCapsuleColliderSize(CapsuleCollider2D* collider);
		static void SetCapsuleColliderSize(CapsuleCollider2D* collider, Vector2f size);


	private:
		static void PostStep(Scene& scene, float timeStep);

	private:
		inline static std::unordered_map<Physics2DScene*, std::vector<Rigidbody2D*>> m_PhysicScenes;
	};
}