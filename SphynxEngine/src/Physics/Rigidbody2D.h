#pragma once

#include "Core/Core.h"
#include "Math/Transform.h"


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
		virtual ~Rigidbody2D() = default;

		virtual void AddCollider(class Collider2D* collider) = 0;
		virtual void RemoveCollider(Collider2D* collider) = 0;


		virtual const RigidbodyDef& GetDefinition() const { return m_Definition; }
		virtual class PhysicsWorld2D* GetPhysicWorld() const { return m_PhysicWorld; }

		virtual void SetTransform(const Transform& transform) = 0;
		virtual Vector2f GetPosition() = 0;
		virtual float GetRotation() = 0;

		virtual bool IsEnabled() = 0;
		virtual void SetEnabled(bool enable) = 0;

		virtual RigidbodyType GetType() = 0;
		virtual void SetType(RigidbodyType type) = 0;

		virtual Vector2f GetLinearVelocity() = 0;
		virtual void SetLinearVelocity(Vector2f velocity) = 0;

		virtual float GetAngularVelocity() = 0;
		virtual void SetAngularVelocity(float velocity) = 0;

		virtual float GetLinearDamping() = 0;
		virtual void SetLinearDamping(float damping) = 0;

		virtual float GetAngularDamping() = 0;
		virtual void SetAngularDamping(float damping) = 0;

		virtual float GetGravityScale() = 0;
		virtual void SetGravityScale(float gravityScale) = 0;

	protected:
		RigidbodyDef m_Definition;
		PhysicsWorld2D* m_PhysicWorld;
	};
}