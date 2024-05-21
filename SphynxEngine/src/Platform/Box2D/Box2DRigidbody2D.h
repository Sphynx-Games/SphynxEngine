#pragma once

#include "Physics/Rigidbody2D.h"

#include <box2d/b2_body.h>


namespace Sphynx
{
	class Box2DRigidbody2D : public Rigidbody2D
	{
	public:
		Box2DRigidbody2D(RigidbodyDef definition);
		virtual ~Box2DRigidbody2D();

		virtual void AddCollider(class Collider2D* collider) override;
		virtual void RemoveCollider(Collider2D* collider) override;


		virtual void SetTransform(const Transform& transform) override;
		virtual Vector2f GetPosition() override;
		virtual float GetRotation() override;

		virtual bool IsEnabled() override;
		virtual void SetEnabled(bool enable) override;

		virtual RigidbodyType GetType() override;
		virtual void SetType(RigidbodyType type) override;

		virtual Vector2f GetLinearVelocity() override;
		virtual void SetLinearVelocity(Vector2f velocity) override;

		virtual float GetAngularVelocity() override;
		virtual void SetAngularVelocity(float velocity) override;

		virtual float GetLinearDamping() override;
		virtual void SetLinearDamping(float damping) override;

		virtual float GetAngularDamping() override;
		virtual void SetAngularDamping(float damping) override;

		virtual float GetGravityScale() override;
		virtual void SetGravityScale(float gravityScale) override;

		static b2BodyType RigidbodyType_To_Box2D(RigidbodyType type);
		static RigidbodyType Box2D_To_RigidbodyType(b2BodyType type);

	private:
		class b2Body* m_Body;

		friend class Box2DPhysicsWorld2D;
	};
}
