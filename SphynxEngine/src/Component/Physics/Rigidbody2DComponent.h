#pragma once

#include "Core/Core.h"
#include "Component/Common.h"
#include "Physics/Rigidbody2D.h"

namespace Sphynx
{
	struct SPHYNX_API Rigidbody2DComponent
	{
		COMPONENT_COMMON_BODY_NO_DEFAULT_CONSTRUCTOR(Rigidbody2DComponent);

	public:
		Rigidbody2DComponent();

		bool IsEnabled() const;
		void SetEnabled(bool enabled);

		RigidbodyType GetRigidbodyType() const;
		void SetRigidbodyType(RigidbodyType type);

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

	private:
		bool m_Enabled;
		RigidbodyType m_Type;
		Vector2f m_LinearVelocity;
		float m_AngularVelocity;
		float m_LinearDamping;
		float m_AngularDamping;
		float m_GravityScale;

		class Rigidbody2D* m_Rigidbody;

		friend class Scene;
		friend class Physics2D;
	};
}
