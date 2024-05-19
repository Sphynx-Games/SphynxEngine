#pragma once

#include "Core/Core.h"
#include "Component/Common.h"
#include "Physics/Physics2DUtils.h"

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
		// initial values
		bool Enabled;
		RigidbodyType Type;
		Vector2f LinearVelocity;
		float AngularVelocity;
		float LinearDamping;
		float AngularDamping;
		float GravityScale;

		// pointer to rigidbody for physics2d
		class Rigidbody2D* m_Rigidbody;

		friend class Scene;
		friend class Physics2D;
	};
}
