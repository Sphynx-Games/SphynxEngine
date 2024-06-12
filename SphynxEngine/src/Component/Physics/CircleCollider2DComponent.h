#pragma once

#include "Core/Core.h"
#include "Component/Common.h"
#include "Math/Vector.h"


namespace Sphynx
{
	struct SPHYNX_API CircleCollider2DComponent
	{
		COMPONENT_COMMON_BODY_NO_DEFAULT_CONSTRUCTOR(CircleCollider2DComponent);

	public:
		CircleCollider2DComponent(float radius = 0.5f, Vector2f offset = { 0.0f, 0.0f }, bool isTrigger = false);

		float GetRadius() const;
		void SetRadius(float radius);

		Vector2f GetOffset() const;
		void SetOffset(Vector2f offset);

		bool IsTrigger() const;
		void SetIsTrigger(bool trigger);

		bool NeedsDebug() const;
		void SetDebug(bool debug);

	private:
		float m_Radius;
		Vector2f m_Offset;
		bool m_Trigger;
		bool m_Debug;

		class CircleCollider2D* m_Collider;

		friend class Scene;
	};
}
