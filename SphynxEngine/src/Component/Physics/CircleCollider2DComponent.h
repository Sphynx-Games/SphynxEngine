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

	public:
		float m_Radius;
		Vector2f m_Offset;
		bool m_Trigger;
		bool m_Debug;

		class CircleCollider2D* m_Collider;

		friend class Scene;
	};
}


#include "Reflection/Reflection.h"


SPX_REFLECT_STRUCT_BEGIN(Sphynx::CircleCollider2DComponent)

SPX_REFLECT_PROPERTY(m_Radius)
SPX_REFLECT_PROPERTY(m_Offset)
SPX_REFLECT_PROPERTY(m_Trigger)
SPX_REFLECT_PROPERTY(m_Debug)

SPX_REFLECT_STRUCT_END(Sphynx::CircleCollider2DComponent)