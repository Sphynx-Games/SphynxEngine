#include "Physics/Physics2D.h"
#include "Physics/PhysicsWorld2D.h"
#include "Physics/Rigidbody2D.h"
#include "Physics/Collider2D.h"
#include "Platform/Box2D/Box2DPhysicsWorld2D.h"
#include "Platform/Box2D/Box2DRigidbody2D.h"
#include "Platform/Box2D/Box2DCollider2D.h"
#include "Renderer/Renderer2D.h"
#include "Math/Transform.h"
#include "Core/Delegate.h"

#define SPX_PHYSICS_2D_BOX2D


namespace Sphynx
{
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

	void Physics2D::Shutdown()
	{
		// delete worlds
		std::unordered_map<PhysicsWorld2D*, std::unordered_set<Rigidbody2D*>> worlds = s_PhysicsWorldToRigidbodies;
		for (auto& [world, rigidbodies] : worlds)
		{
			if (world != nullptr)
			{
				DestroyPhysicsWorld(world);
			}
			else
			{
				for (auto rigidbody : rigidbodies)
				{
					DestroyRigidbody(rigidbody);
				}
			}
		}
		worlds.clear();
		s_PhysicsWorldToRigidbodies.clear();

		// delete rigidbodies
		std::unordered_map<Rigidbody2D*, std::unordered_set<Collider2D*>> rigidbodies = s_RigidbodyToColliders;
		for (auto& [rigidbody, colliders] : rigidbodies)
		{
			if (rigidbody != nullptr)
			{
				DestroyRigidbody(rigidbody);
			}
			else
			{
				for (auto collider : colliders)
				{
					DestroyCollider(collider);
				}
			}
		}
		rigidbodies.clear();
		s_RigidbodyToColliders.clear();
	}

	PhysicsWorld2D* Physics2D::CreatePhysicsWorld(Vector2f gravity)
	{
#ifdef SPX_PHYSICS_2D_BOX2D
		Box2DPhysicsWorld2D* world = new Box2DPhysicsWorld2D(gravity);
		s_PhysicsWorldToRigidbodies.emplace(world, std::unordered_set<Rigidbody2D*>());
		return world;
#else

#error No Physics 2D Engine selected

#endif
	}

	void Physics2D::DestroyPhysicsWorld(PhysicsWorld2D* physicsWorld)
	{
		SPX_CORE_ASSERT(physicsWorld != nullptr, "PhysicsWorld2D is nullptr!!");
		SPX_CORE_ASSERT(s_PhysicsWorldToRigidbodies.find(physicsWorld) != s_PhysicsWorldToRigidbodies.end(), "Rigidbody2D not found!!");

		std::unordered_set<Rigidbody2D*> worldRigidbodies = s_PhysicsWorldToRigidbodies[physicsWorld];
		for (auto rigidbody : worldRigidbodies)
		{
			DestroyRigidbody(rigidbody);
		}
		worldRigidbodies.clear();
		
		s_PhysicsWorldToRigidbodies.erase(physicsWorld);
		delete physicsWorld;
	}

	Rigidbody2D* Physics2D::CreateRigidbody(const RigidbodyDef& rigidbodyDef)
	{
#ifdef SPX_PHYSICS_2D_BOX2D
		Box2DRigidbody2D* rigidbody = new Box2DRigidbody2D(rigidbodyDef);
		s_PhysicsWorldToRigidbodies[nullptr].insert(rigidbody);
		s_RigidbodyToColliders.emplace(rigidbody, std::unordered_set<Collider2D*>());
		return rigidbody;
#else

#error No Physics 2D Engine selected

#endif
	}

	void Physics2D::DestroyRigidbody(Rigidbody2D* rigidbody)
	{
		SPX_CORE_ASSERT(rigidbody != nullptr, "Rigidbody2D is nullptr!!");
		SPX_CORE_ASSERT(s_RigidbodyToColliders.find(rigidbody) != s_RigidbodyToColliders.end(), "Rigidbody2D not found!!");

		if (s_PhysicsWorldToRigidbodies[rigidbody->GetPhysicsWorld()].count(rigidbody))
		{
			s_PhysicsWorldToRigidbodies[rigidbody->GetPhysicsWorld()].erase(rigidbody);
		}

		for (auto collider : s_RigidbodyToColliders[rigidbody])
		{
			rigidbody->RemoveCollider(collider);
			delete collider;
		}
		rigidbody->m_PhysicsWorld = nullptr;

		s_RigidbodyToColliders.erase(rigidbody);
		delete rigidbody;
	}

	void Physics2D::AddRigidbody(PhysicsWorld2D* physicsWorld, Rigidbody2D* rigidbody)
	{
		SPX_CORE_ASSERT(physicsWorld != nullptr, "PhysicsWorld2D is nullptr!!");
		SPX_CORE_ASSERT(rigidbody != nullptr, "Rigidbody2D is nullptr!!");
		SPX_CORE_ASSERT(rigidbody->GetPhysicsWorld() == nullptr, "Rigidbody2D already added to PhysicsWorld2D!!");

		physicsWorld->AddRigidbody(rigidbody);
		rigidbody->m_PhysicsWorld = physicsWorld;

		s_PhysicsWorldToRigidbodies[nullptr].erase(rigidbody);
		s_PhysicsWorldToRigidbodies[physicsWorld].insert(rigidbody);
	}

	void Physics2D::RemoveRigidbody(PhysicsWorld2D* physicsWorld, Rigidbody2D* rigidbody)
	{
		SPX_CORE_ASSERT(physicsWorld != nullptr, "PhysicsWorld2D is nullptr!!");
		SPX_CORE_ASSERT(rigidbody != nullptr, "Rigidbody2D is nullptr!!");
		SPX_CORE_ASSERT(physicsWorld == rigidbody->GetPhysicsWorld(), "physicsWorld and rigidbody->GetPhysicsWorld() are not the same!!");

		physicsWorld->RemoveRigidbody(rigidbody);
		rigidbody->m_PhysicsWorld = nullptr;

		s_PhysicsWorldToRigidbodies[physicsWorld].erase(rigidbody);
		s_PhysicsWorldToRigidbodies[nullptr].insert(rigidbody);
	}

	BoxCollider2D* Physics2D::CreateBoxCollider(Vector2f size, Vector2f offset, bool isTrigger, bool debug)
	{
#ifdef SPX_PHYSICS_2D_BOX2D
		Box2DBoxCollider2D* collider = new Box2DBoxCollider2D(size, offset, isTrigger, debug);
		s_RigidbodyToColliders[nullptr].insert(collider);
		return collider;
#else

#error No Physics 2D Engine selected

#endif
	}

	CircleCollider2D* Physics2D::CreateCircleCollider(float radius, Vector2f offset, bool isTrigger, bool debug)
	{
#ifdef SPX_PHYSICS_2D_BOX2D
		Box2DCircleCollider2D* collider = new Box2DCircleCollider2D(radius, offset, isTrigger, debug);
		s_RigidbodyToColliders[nullptr].insert(collider);
		return collider;
#else

#error No Physics 2D Engine selected

#endif
	}

	CapsuleCollider2D* Physics2D::CreateCapsuleCollider(Vector2f size, Vector2f offset, bool isTrigger, bool debug)
	{
#ifdef SPX_PHYSICS_2D_BOX2D
		Box2DCapsuleCollider2D* collider = new Box2DCapsuleCollider2D(size, offset, isTrigger, debug);
		s_RigidbodyToColliders[nullptr].insert(collider);
		return collider;
#else

#error No Physics 2D Engine selected

#endif
	}

	void Physics2D::DestroyCollider(Collider2D* collider)
	{
		SPX_CORE_ASSERT(collider != nullptr, "Collider2D is nullptr!!");
		SPX_CORE_ASSERT(s_RigidbodyToColliders[collider->GetRigidbody()].count(collider) != 0, "Collider2D not found!!");

		if (collider->GetRigidbody() != nullptr)
		{
			collider->GetRigidbody()->RemoveCollider(collider);
		}
		s_RigidbodyToColliders[collider->GetRigidbody()].erase(collider);
		delete collider;
	}

	void Physics2D::AddCollider(Rigidbody2D* rigidbody, Collider2D* collider)
	{
		SPX_CORE_ASSERT(rigidbody != nullptr, "Rigidbody2D is nullptr!!");
		SPX_CORE_ASSERT(collider != nullptr, "Collider2D is nullptr!!");
		SPX_CORE_ASSERT(collider->GetRigidbody() == nullptr, "Collider2D already added to Rigidbody2D!!");

		rigidbody->AddCollider(collider);
		collider->m_Rigidbody = rigidbody;

		s_RigidbodyToColliders[nullptr].erase(collider);
		s_RigidbodyToColliders[rigidbody].insert(collider);
	}

	void Physics2D::RemoveCollider(Rigidbody2D* rigidbody, Collider2D* collider)
	{
		SPX_CORE_ASSERT(rigidbody != nullptr, "Rigidbody2D is nullptr!!");
		SPX_CORE_ASSERT(collider != nullptr, "Collider2D is nullptr!!");
		SPX_CORE_ASSERT(rigidbody == collider->GetRigidbody(), "rigidbody and collider->GetRigidbody() are not the same!!");

		rigidbody->RemoveCollider(collider);
		collider->m_Rigidbody = nullptr;

		s_RigidbodyToColliders[rigidbody].erase(collider);
		s_RigidbodyToColliders[nullptr].insert(collider);
	}

	const std::unordered_set<Rigidbody2D*>& Physics2D::GetRigidbodies(PhysicsWorld2D* physicsWorld)
	{
		SPX_CORE_ASSERT(physicsWorld != nullptr, "PhysicsWorld2D is nullptr!!");

		return s_PhysicsWorldToRigidbodies[physicsWorld];
	}

	const std::unordered_set<Collider2D*>& Physics2D::GetColliders(Rigidbody2D* rigidbody)
	{
		SPX_CORE_ASSERT(rigidbody != nullptr, "Rigidbody2D is nullptr!!");

		return s_RigidbodyToColliders[rigidbody];
	}

	void Physics2D::Step(PhysicsWorld2D* physicsWorld, float timeStep)
	{
		SPX_CORE_ASSERT(physicsWorld != nullptr, "PhysicsWorld2D is nullptr!!");
		physicsWorld->Step(timeStep, WorldVelocityIterations, WorldPositionIterations);
	}

}