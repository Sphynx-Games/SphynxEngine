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

	PhysicsWorld2D* Physics2D::CreatePhysicsWorld()
	{
#ifdef SPX_PHYSICS_2D_BOX2D
		Box2DPhysicsWorld2D* world = new Box2DPhysicsWorld2D();
		s_PhysicWorlds.push_back(world);
		return world;
#else

#error No Physics 2D Engine selected

#endif
	}

	void Physics2D::DestroyPhysicsWorld(PhysicsWorld2D* physicWorld)
	{
		s_PhysicWorlds.erase(std::remove(s_PhysicWorlds.begin(), s_PhysicWorlds.end(), physicWorld), s_PhysicWorlds.end());
		delete physicWorld;
	}

	Rigidbody2D* Physics2D::CreateRigidbody(const RigidbodyDef& rigidbodyDef)
	{
#ifdef SPX_PHYSICS_2D_BOX2D
		Box2DRigidbody2D* rigidbody = new Box2DRigidbody2D(rigidbodyDef);
		s_Rigidbodies.push_back(rigidbody);
		return rigidbody;
#else

#error No Physics 2D Engine selected

#endif
	}

	void Physics2D::DestroyRigidbody(Rigidbody2D* rigidbody)
	{
		s_Rigidbodies.erase(std::remove(s_Rigidbodies.begin(), s_Rigidbodies.end(), rigidbody), s_Rigidbodies.end());
		delete rigidbody;
	}

	void Physics2D::AddRigidbody(PhysicsWorld2D* physicWorld, Rigidbody2D* rigidbody)
	{
		SPX_CORE_ASSERT(physicWorld != nullptr, "PhysicsWolrd2D is nullptr!!");
		physicWorld->AddRigidbody(rigidbody);
	}

	void Physics2D::RemoveRigidbody(Rigidbody2D* rigidbody)
	{
		SPX_CORE_ASSERT(rigidbody != nullptr, "Rigidbody2D is nullptr!!");
		SPX_CORE_ASSERT(rigidbody->GetPhysicWorld() != nullptr, "PhysicsWolrd2D is nullptr!!");
		rigidbody->GetPhysicWorld()->RemoveRigidbody(rigidbody);
	}

	BoxCollider2D* Physics2D::CreateBoxCollider(Vector2f size, Vector2f offset, bool isTrigger, bool debug)
	{
#ifdef SPX_PHYSICS_2D_BOX2D
		Box2DBoxCollider2D* collider = new Box2DBoxCollider2D(size, offset, isTrigger, debug);
		s_Colliders.push_back(collider);
		return collider;
#else

#error No Physics 2D Engine selected

#endif
	}

	CircleCollider2D* Physics2D::CreateCircleCollider(float radius, Vector2f offset, bool isTrigger, bool debug)
	{
#ifdef SPX_PHYSICS_2D_BOX2D
		Box2DCircleCollider2D* collider = new Box2DCircleCollider2D(radius, offset, isTrigger, debug);
		s_Colliders.push_back(collider);
		return collider;
#else

#error No Physics 2D Engine selected

#endif
	}

	CapsuleCollider2D* Physics2D::CreateCapsuleCollider(Vector2f size, Vector2f offset, bool isTrigger, bool debug)
	{
#ifdef SPX_PHYSICS_2D_BOX2D
		Box2DCapsuleCollider2D* collider = new Box2DCapsuleCollider2D(size, offset, isTrigger, debug);
		s_Colliders.push_back(collider);
		return collider;
#else

#error No Physics 2D Engine selected

#endif
	}

	void Physics2D::DestroyCollider(Collider2D* collider)
	{
		s_Colliders.erase(std::remove(s_Colliders.begin(), s_Colliders.end(), collider), s_Colliders.end());
		delete collider;
	}

	void Physics2D::AddCollider(Rigidbody2D* rigidbody, Collider2D* collider)
	{
		SPX_CORE_ASSERT(rigidbody != nullptr, "Rigidbody2D is nullptr!!");
		rigidbody->AddCollider(collider);
	}

	void Physics2D::RemoveCollider(Rigidbody2D* rigidbody, Collider2D* collider)
	{
		SPX_CORE_ASSERT(rigidbody != nullptr, "Rigidbody2D is nullptr!!");
		rigidbody->RemoveCollider(collider);
	}

	void Physics2D::Step(PhysicsWorld2D* physicWorld, float timeStep)
	{
		SPX_CORE_ASSERT(physicWorld != nullptr, "PhysicsWorld2D is nullptr!!");
		physicWorld->Step(timeStep);
	}
}