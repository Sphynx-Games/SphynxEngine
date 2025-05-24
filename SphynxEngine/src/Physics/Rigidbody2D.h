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
		Rigidbody2D(RigidbodyDef definition);

		inline class PhysicsWorld2D* GetPhysicsWorld() const { return m_PhysicsWorld; }
		inline struct Rigidbody2DData* GetData() const { return m_Data; }

		Vector2f GetPosition() const;
		float GetRotation() const;
		Transform GetTransform() const;

		bool IsEnabled() const;
		void SetEnabled(bool enable);

		RigidbodyType GetType() const;
		void SetType(RigidbodyType type);

		Vector2f GetLinearVelocity() const;
		void SetLinearVelocity(Vector2f velocity);

		float GetAngularVelocity() const;
		void SetAngularVelocity(float velocity);

		float GetLinearDamping() const;
		void SetLinearDamping(float damping);

		float GetAngularDamping() const;
		void SetAngularDamping(float damping);

		float GetGravityScale() const;
		void SetGravityScale(float gravityScale);

	protected:
		~Rigidbody2D();

		void AttachToPhysicsWorld(PhysicsWorld2D* physicsWorld);
		void Detach();


		Vector2f GetPositionDef() const { return Vector2f(m_Definition.Transform.Position.X, m_Definition.Transform.Position.Y); }
		float GetRotationDef() const { return m_Definition.Transform.Rotation.Z; }

		bool IsEnabledDef() const { return m_Definition.Enabled; }
		void SetEnabledDef(bool enable) { m_Definition.Enabled = enable; }

		RigidbodyType GetTypeDef() const { return m_Definition.Type; }
		void SetTypeDef(RigidbodyType type) { m_Definition.Type = type; };

		Vector2f GetLinearVelocityDef() const { return m_Definition.LinearVelocity; }
		void SetLinearVelocityDef(Vector2f velocity) { m_Definition.LinearVelocity = velocity; }

		float GetAngularVelocityDef() const { return m_Definition.AngularVelocity; }
		void SetAngularVelocityDef(float velocity) { m_Definition.AngularVelocity = velocity; }

		float GetLinearDampingDef() const { return m_Definition.LinearDamping; }
		void SetLinearDampingDef(float damping) { m_Definition.LinearDamping = damping; }

		float GetAngularDampingDef() const { return m_Definition.AngularDamping; }
		void SetAngularDampingDef(float damping) { m_Definition.AngularDamping = damping; }

		float GetGravityScaleDef() const { return m_Definition.GravityScale; }
		void SetGravityScaleDef(float gravityScale) { m_Definition.GravityScale = gravityScale; }

	protected:
		RigidbodyDef m_Definition;
		PhysicsWorld2D* m_PhysicsWorld;
		Rigidbody2DData* m_Data;

	public:
		friend class Physics2D;
		friend class PhysicsWorld2D;
		friend class Collider2D;
	};
}


#include "Reflection/Reflection.h"


SPX_REFLECT_ENUM_BEGIN(Sphynx::RigidbodyType, SPHYNX_API)

SPX_REFLECT_ENUM_VALUE(STATIC)
SPX_REFLECT_ENUM_VALUE(DYNAMIC)
SPX_REFLECT_ENUM_VALUE(KINEMATIC)

SPX_REFLECT_ENUM_END(Sphynx::RigidbodyType, SPHYNX_API)


//SPX_REFLECT_STRUCT_BEGIN(Sphynx::RigidbodyDef, SPHYNX_API)
//
//SPX_REFLECT_PROPERTY(Enabled)
//SPX_REFLECT_PROPERTY(Type)
//SPX_REFLECT_PROPERTY(LinearVelocity)
//SPX_REFLECT_PROPERTY(AngularVelocity)
//SPX_REFLECT_PROPERTY(LinearDamping)
//SPX_REFLECT_PROPERTY(AngularDamping)
//SPX_REFLECT_PROPERTY(GravityScale)
//SPX_REFLECT_PROPERTY(Transform)
//
//SPX_REFLECT_STRUCT_END(Sphynx::RigidbodyDef, SPHYNX_API)


//SPX_REFLECT_CLASS_BEGIN(Sphynx::Rigidbody2D, SPHYNX_API)
//
//SPX_REFLECT_PROPERTY(m_Definition)
//SPX_REFLECT_PROPERTY(m_PhysicsWorld)
//SPX_REFLECT_PROPERTY(m_Data)
//
//SPX_REFLECT_CLASS_END(Sphynx::Rigidbody2D, SPHYNX_API)