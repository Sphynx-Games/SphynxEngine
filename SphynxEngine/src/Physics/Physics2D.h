#pragma once

#include "Core/Core.h"
#include "Core/Delegate.h"
#include "Math/Transform.h"
#include "Container/Map.h"
#include "Container/Set.h"


namespace Sphynx
{
	class SPHYNX_API Physics2D
	{
	public:
		static void Init();
		static void Shutdown();

		static class PhysicsWorld2D* CreatePhysicsWorld(Vector2f gravity = WorldGravity);
		static void DestroyPhysicsWorld(PhysicsWorld2D* physicsWorld);

		static class Rigidbody2D* CreateRigidbody(const struct RigidbodyDef& rigidbodyDef);
		static void DestroyRigidbody(Rigidbody2D* rigidbody);

		static class BoxCollider2D* CreateBoxCollider(Vector2f size = { 1.0, 1.0 }, Vector2f offset = { 0.0f, 0.0f }, bool isTrigger = false, bool debug = true);
		static class CircleCollider2D* CreateCircleCollider(float radius = 0.5f, Vector2f offset = { 0.0f, 0.0f }, bool isTrigger = false, bool debug = true);
		static class CapsuleCollider2D* CreateCapsuleCollider(Vector2f size = { 1.0, 1.0 }, Vector2f offset = { 0.0f, 0.0f }, bool isTrigger = false, bool debug = true);
		static void DestroyCollider(class Collider2D* collider);

		static void AddRigidbody(PhysicsWorld2D* physicsWorld, Rigidbody2D* rigidbody); // to physicWorld
		static void RemoveRigidbody(PhysicsWorld2D* physicsWorld, Rigidbody2D* rigidbody);
		static void AddCollider(Rigidbody2D* rigidbody, Collider2D* collider); // to rigidbody
		static void RemoveCollider(Rigidbody2D* rigidbody, Collider2D* collider);

		static const Set<Rigidbody2D*>& GetRigidbodies(PhysicsWorld2D* physicsWorld);
		static const Set<Collider2D*>& GetColliders(Rigidbody2D* rigidbody);

		static void Step(PhysicsWorld2D* physicsWorld, float timeStep);

	public:
		inline static Vector2f WorldGravity = Vector2f(0.0f, -10.0f);
		inline static uint32_t WorldVelocityIterations = 8;
		inline static uint32_t WorldPositionIterations = 3;

	private:
		inline static HashMap<PhysicsWorld2D*, Set<Rigidbody2D*>> s_PhysicsWorldToRigidbodies = {};
		inline static HashMap<Rigidbody2D*, Set<Collider2D*>> s_RigidbodyToColliders = {};
	};
}