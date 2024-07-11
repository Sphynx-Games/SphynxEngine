#include "Physics/Physics2D.h"
#include "Physics/PhysicsWorld2D.h"
#include "Physics/Rigidbody2D.h"
#include "Physics/Collider2D.h"
#include "Math/Transform.h"
#include "Core/Delegate.h"


namespace Sphynx
{
	void Physics2D::Init()
	{
		s_PhysicsWorldToRigidbodies.Add(nullptr, Set<Rigidbody2D*>());
		s_RigidbodyToColliders.Add(nullptr, Set<Collider2D*>());
	}

	void Physics2D::Shutdown()
	{
		// delete rigidbodies
		HashMap<Rigidbody2D*, Set<Collider2D*>> rigidbodies = s_RigidbodyToColliders;
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
		rigidbodies.RemoveAll();
		s_RigidbodyToColliders.RemoveAll();

		// delete worlds
		HashMap<PhysicsWorld2D*, Set<Rigidbody2D*>> worlds = s_PhysicsWorldToRigidbodies;
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
		worlds.RemoveAll();
		s_PhysicsWorldToRigidbodies.RemoveAll();
	}

	PhysicsWorld2D* Physics2D::CreatePhysicsWorld(Vector2f gravity)
	{
		PhysicsWorld2D* world = new PhysicsWorld2D(gravity);
		s_PhysicsWorldToRigidbodies.Add(world, Set<Rigidbody2D*>());
		return world;
	}

	void Physics2D::DestroyPhysicsWorld(PhysicsWorld2D* physicsWorld)
	{
		SPX_CORE_ASSERT(physicsWorld != nullptr, "PhysicsWorld2D is nullptr!!");
		SPX_CORE_ASSERT(s_PhysicsWorldToRigidbodies.ContainsKey(physicsWorld), "Rigidbody2D not found!!");

		Set<Rigidbody2D*> worldRigidbodies = s_PhysicsWorldToRigidbodies[physicsWorld];
		for (auto rigidbody : worldRigidbodies)
		{
			DestroyRigidbody(rigidbody);
		}
		worldRigidbodies.RemoveAll();
		
		s_PhysicsWorldToRigidbodies.Remove(physicsWorld);
		delete physicsWorld;
	}

	Rigidbody2D* Physics2D::CreateRigidbody(const RigidbodyDef& rigidbodyDef)
	{
		Rigidbody2D* rigidbody = new Rigidbody2D(rigidbodyDef);
		s_PhysicsWorldToRigidbodies[nullptr].Add(rigidbody);
		s_RigidbodyToColliders.Add(rigidbody, Set<Collider2D*>());
		return rigidbody;
	}

	void Physics2D::DestroyRigidbody(Rigidbody2D* rigidbody)
	{
		SPX_CORE_ASSERT(rigidbody != nullptr, "Rigidbody2D is nullptr!!");
		SPX_CORE_ASSERT(s_RigidbodyToColliders.ContainsKey(rigidbody), "Rigidbody2D not found!!");

		if (s_PhysicsWorldToRigidbodies[rigidbody->GetPhysicsWorld()].Contains(rigidbody))
		{
			s_PhysicsWorldToRigidbodies[rigidbody->GetPhysicsWorld()].Remove(rigidbody);
		}		

		for (auto collider : s_RigidbodyToColliders[rigidbody])
		{
			collider->Detach();
			delete collider;
		}
		rigidbody->m_PhysicsWorld = nullptr;

		s_RigidbodyToColliders.Remove(rigidbody);
		delete rigidbody;
	}

	void Physics2D::AddRigidbody(PhysicsWorld2D* physicsWorld, Rigidbody2D* rigidbody)
	{
		SPX_CORE_ASSERT(physicsWorld != nullptr, "PhysicsWorld2D is nullptr!!");
		SPX_CORE_ASSERT(rigidbody != nullptr, "Rigidbody2D is nullptr!!");
		SPX_CORE_ASSERT(rigidbody->GetPhysicsWorld() == nullptr, "Rigidbody2D already added to PhysicsWorld2D!!");

		rigidbody->AttachToPhysicsWorld(physicsWorld);
		rigidbody->m_PhysicsWorld = physicsWorld;

		s_PhysicsWorldToRigidbodies[nullptr].Remove(rigidbody);
		s_PhysicsWorldToRigidbodies[physicsWorld].Add(rigidbody);
	}

	void Physics2D::RemoveRigidbody(Rigidbody2D* rigidbody)
	{
		SPX_CORE_ASSERT(rigidbody != nullptr, "Rigidbody2D is nullptr!!");

		PhysicsWorld2D* physicsWorld = rigidbody->GetPhysicsWorld();
		if (physicsWorld != nullptr)
		{
			rigidbody->Detach();
			rigidbody->m_PhysicsWorld = nullptr;
		}

		s_PhysicsWorldToRigidbodies[physicsWorld].Remove(rigidbody);
		s_PhysicsWorldToRigidbodies[nullptr].Add(rigidbody);
	}

	BoxCollider2D* Physics2D::CreateBoxCollider(Vector2f size, Vector2f offset, bool isTrigger)
	{
		BoxCollider2D* collider = new BoxCollider2D(size, offset, isTrigger);
		s_RigidbodyToColliders[nullptr].Add(collider);
		return collider;
	}

	CircleCollider2D* Physics2D::CreateCircleCollider(float radius, Vector2f offset, bool isTrigger)
	{
		CircleCollider2D* collider = new CircleCollider2D(radius, offset, isTrigger);
		s_RigidbodyToColliders[nullptr].Add(collider);
		return collider;
	}

	CapsuleCollider2D* Physics2D::CreateCapsuleCollider(Vector2f size, Vector2f offset, bool isTrigger)
	{
		CapsuleCollider2D* collider = new CapsuleCollider2D(size, offset, isTrigger);
		s_RigidbodyToColliders[nullptr].Add(collider);
		return collider;
	}

	void Physics2D::DestroyCollider(Collider2D* collider)
	{
		SPX_CORE_ASSERT(collider != nullptr, "Collider2D is nullptr!!");

		collider->Detach();
		s_RigidbodyToColliders[collider->GetRigidbody()].Remove(collider);
		delete collider;
	}

	void Physics2D::AddCollider(Rigidbody2D* rigidbody, Collider2D* collider)
	{
		SPX_CORE_ASSERT(rigidbody != nullptr, "Rigidbody2D is nullptr!!");
		SPX_CORE_ASSERT(collider != nullptr, "Collider2D is nullptr!!");
		SPX_CORE_ASSERT(collider->GetRigidbody() == nullptr, "Collider2D already added to Rigidbody2D!!");

		collider->AttachToRigidbody(rigidbody);
		collider->m_Rigidbody = rigidbody;

		s_RigidbodyToColliders[nullptr].Remove(collider);
		s_RigidbodyToColliders[rigidbody].Add(collider);
	}

	void Physics2D::AddCollider(PhysicsWorld2D* physicsWorld, Collider2D* collider, const Transform& transform)
	{
		SPX_CORE_ASSERT(physicsWorld != nullptr, "PhysicsWorld2D is nullptr!!");
		SPX_CORE_ASSERT(collider != nullptr, "Collider2D is nullptr!!");

		collider->AttachToPhysicsWorld(physicsWorld, transform);
	}

	void Physics2D::RemoveCollider(Collider2D* collider)
	{
		SPX_CORE_ASSERT(collider != nullptr, "Collider2D is nullptr!!");

		Rigidbody2D* rigidbody = collider->GetRigidbody();
		if (rigidbody != nullptr)
		{
			collider->Detach();
			collider->m_PhysicsWorld = nullptr;
			collider->m_Rigidbody = nullptr;
		}
		
		s_RigidbodyToColliders[rigidbody].Remove(collider);
		s_RigidbodyToColliders[nullptr].Add(collider);
	}

	const Set<Rigidbody2D*>& Physics2D::GetRigidbodies(PhysicsWorld2D* physicsWorld)
	{
		SPX_CORE_ASSERT(physicsWorld != nullptr, "PhysicsWorld2D is nullptr!!");

		return s_PhysicsWorldToRigidbodies[physicsWorld];
	}

	const Set<Collider2D*>& Physics2D::GetColliders(Rigidbody2D* rigidbody)
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