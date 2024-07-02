#pragma once

#include "Core/Core.h"
#include "Math/Transform.h"
#include "Physics/Physics2D.h"


namespace Sphynx
{
	enum SPHYNX_API RigidbodyType
	{
		STATIC, DYNAMIC, KINEMATIC
	};

	struct SPHYNX_API RigidbodyDef
	{
		bool Enabled = true;
		RigidbodyType Type = RigidbodyType::STATIC;
		Vector2f LinearVelocity = Vector2f(0.0f, 0.0f);
		float AngularVelocity = 0.0f;
		float LinearDamping = 0.0f;
		float AngularDamping = 0.0f;
		float GravityScale = 1.0f;
		Transform Transform;
	};

	class SPHYNX_API Rigidbody2D
	{
	public:
		Rigidbody2D(RigidbodyDef definition) :
			m_Definition(definition),
			m_PhysicsWorld(nullptr)
		{}

		inline virtual class PhysicsWorld2D* GetPhysicsWorld() const { return m_PhysicsWorld; }

		inline virtual Vector2f GetPosition() const { return Vector2f(m_Definition.Transform.Position.X, m_Definition.Transform.Position.Y); }
		inline virtual float GetRotation() const { return m_Definition.Transform.Rotation.Z; }

		inline virtual bool IsEnabled() const { return m_Definition.Enabled; }
		inline virtual void SetEnabled(bool enable) { m_Definition.Enabled = enable; }

		inline virtual RigidbodyType GetType() const { return m_Definition.Type; }
		inline virtual void SetType(RigidbodyType type) { m_Definition.Type = type; };

		inline virtual Vector2f GetLinearVelocity() const { return m_Definition.LinearVelocity; }
		inline virtual void SetLinearVelocity(Vector2f velocity) { m_Definition.LinearVelocity = velocity; }

		inline virtual float GetAngularVelocity() const { return m_Definition.AngularVelocity; }
		inline virtual void SetAngularVelocity(float velocity) { m_Definition.AngularVelocity = velocity; }

		inline virtual float GetLinearDamping() const { return m_Definition.LinearDamping; }
		inline virtual void SetLinearDamping(float damping) { m_Definition.LinearDamping = damping; }

		inline virtual float GetAngularDamping() const { return m_Definition.AngularDamping; }
		inline virtual void SetAngularDamping(float damping) { m_Definition.AngularDamping = damping; }

		inline virtual float GetGravityScale() const { return m_Definition.GravityScale; }
		inline virtual void SetGravityScale(float gravityScale) { m_Definition.GravityScale = gravityScale; }

	protected:
		virtual ~Rigidbody2D() 
		{
			m_PhysicsWorld = nullptr;
		};

		virtual void AddCollider(class Collider2D* collider) = 0;
		virtual void RemoveCollider(Collider2D* collider) = 0;

	protected:
		RigidbodyDef m_Definition;
		PhysicsWorld2D* m_PhysicsWorld;

	public:
		friend class Physics2D;
		friend class PhysicsWorld2D;
	};
}