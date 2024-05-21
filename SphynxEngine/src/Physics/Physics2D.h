#pragma once

#include "Core/Core.h"
#include "Core/Delegate.h"
#include "Physics/Rigidbody2D.h"
#include "Math/Transform.h"
#include <unordered_map>
#include <vector>


namespace Sphynx
{
	class SPHYNX_API Physics2D
	{
	public:
		static class PhysicsWorld2D* CreatePhysicsWorld();
		static void DestroyPhysicsWorld(PhysicsWorld2D* physicWorld);

		static Rigidbody2D* CreateRigidbody(const RigidbodyDef& rigidbodyDef);
		static void DestroyRigidbody(class Rigidbody2D* rigidbody);

		static void AddRigidbody(PhysicsWorld2D* physicWorld, Rigidbody2D* rigidbody); // to physicWorld
		static void RemoveRigidbody(Rigidbody2D* rigidbody);

		static class BoxCollider2D* CreateBoxCollider(Vector2f size = { 1.0, 1.0 }, Vector2f offset = { 0.0f, 0.0f }, bool isTrigger = false, bool debug = true);
		static class CircleCollider2D* CreateCircleCollider(float radius = 0.5f, Vector2f offset = { 0.0f, 0.0f }, bool isTrigger = false, bool debug = true);
		static class CapsuleCollider2D* CreateCapsuleCollider(Vector2f size = { 1.0, 1.0 }, Vector2f offset = { 0.0f, 0.0f }, bool isTrigger = false, bool debug = true);
		static void DestroyCollider(class Collider2D* collider);

		static void AddCollider(Rigidbody2D* rigidbody, Collider2D* collider); // to rigidbody
		static void RemoveCollider(Rigidbody2D* rigidbody, Collider2D* collider);

		static void Step(PhysicsWorld2D* physicWorld, float timeStep);

	private:
		// all created worlds, rigidbodies and colliders
		inline static std::vector<class PhysicsWorld2D*> s_PhysicWorlds;
		inline static std::vector<class Rigidbody2D*> s_Rigidbodies;
		inline static std::vector<class Collider2D*> s_Colliders;
	};
}