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

		bool IsEnabled();
		void SetEnabled(bool enabled);

		RigidbodyType GetRigidbodyType();
		void SetRigidbodyType(RigidbodyType type);

		Vector2f GetLinearVelocity();
		void SetLinearVelocity(Vector2f velocity);

		float GetAngularVelocity();
		void SetAngularVelocity(float velocity);

		float GetLinearDamping();
		void SetLinearDamping(float damping);

		float GetAngularDamping();
		void SetAngularDamping(float damping);

		float GetGravityScale();
		void SetGravityScale(float gravityScale);

	private:
		bool Enabled;
		RigidbodyType Type;
		Vector2f LinearVelocity;
		float AngularVelocity;
		float LinearDamping;
		float AngularDamping;
		float GravityScale;

		class Rigidbody2D* m_Rigidbody;

		friend class Scene;
		friend class Physics2D;
	};
}
