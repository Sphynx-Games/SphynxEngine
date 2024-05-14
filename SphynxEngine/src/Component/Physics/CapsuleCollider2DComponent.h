#pragma once

#include "Core/Core.h"
#include "Component/Common.h"
#include "Math/Vector.h"


namespace Sphynx
{
	struct SPHYNX_API CapsuleCollider2DComponent
	{
		COMPONENT_COMMON_BODY_NO_DEFAULT_CONSTRUCTOR(CapsuleCollider2DComponent);

	public:
		CapsuleCollider2DComponent(Vector2f size = { 1.0f, 2.0f }, Vector2f offset = { 0.0f, 0.0f }, bool isTrigger = false);

		Vector2f GetSize();
		void SetSize(Vector2f size);

		Vector2f GetOffset();
		void SetOffset(Vector2f offset);

		bool IsTrigger();
		void SetIsTrigger(bool trigger);

	private:
		Vector2f Size;
		Vector2f Offset;
		bool Trigger;

		class CapsuleCollider2D* m_Collider;

		friend class Scene;
	};
}
