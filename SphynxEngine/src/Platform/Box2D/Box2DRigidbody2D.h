#pragma once

#include "Physics/Rigidbody2D.h"

#include <box2d/b2_body.h>


namespace Sphynx
{
	class Box2DRigidbody2D : public Rigidbody2D
	{
	public:
		Box2DRigidbody2D(RigidbodyDef definition);
		~Box2DRigidbody2D();

		virtual void AddCollider(class Collider2D* collider) override;
		virtual void RemoveCollider(Collider2D* collider) override;

		virtual Vector2f GetPosition() const override;
		virtual float GetRotation() const override;

		virtual bool IsEnabled() const override;
		virtual void SetEnabled(bool enable) override;

		virtual RigidbodyType GetType() const override;
		virtual void SetType(RigidbodyType type) override;

		virtual Vector2f GetLinearVelocity() const override;
		virtual void SetLinearVelocity(Vector2f velocity) override;

		virtual float GetAngularVelocity() const override;
		virtual void SetAngularVelocity(float velocity) override;

		virtual float GetLinearDamping() const override;
		virtual void SetLinearDamping(float damping) override;

		virtual float GetAngularDamping() const override;
		virtual void SetAngularDamping(float damping) override;

		virtual float GetGravityScale() const override;
		virtual void SetGravityScale(float gravityScale) override;

		static b2BodyType RigidbodyType_To_Box2D(RigidbodyType type);
		static RigidbodyType Box2D_To_RigidbodyType(b2BodyType type);

	private:
		class b2Body* m_Body;

		friend class Box2DPhysicsWorld2D;
	};
}
