#pragma once

#include "Core/Core.h"
#include "Component/Common.h"
#include "Math/Vector.h"


namespace Sphynx
{
	struct SPHYNX_API BoxCollider2DComponent
	{
		SPX_REFLECT_GENERATED_BODY();
		COMPONENT_COMMON_BODY_NO_DEFAULT_CONSTRUCTOR(BoxCollider2DComponent);

	public:
		BoxCollider2DComponent(Vector2f size = { 1.0, 1.0 }, Vector2f offset = { 0.0f, 0.0f }, bool isTrigger = false);

		Vector2f GetSize() const;
		void SetSize(Vector2f size);

		Vector2f GetOffset() const;
		void SetOffset(Vector2f offset);
		
		bool IsTrigger() const;
		void SetIsTrigger(bool trigger);

		bool NeedsDebug() const;
		void SetDebug(bool debug);

	private:
		Vector2f m_Size;
		Vector2f m_Offset;
		bool m_Trigger;
		bool m_Debug;

		class BoxCollider2D* m_Collider;

		friend class Scene;
	};
}


#include "Reflection/Reflection.h"


SPX_REFLECT_STRUCT_BEGIN(Sphynx::BoxCollider2DComponent, SPHYNX_API)
SPX_REFLECT_ATTRIBUTE(InternalComponent);

SPX_REFLECT_PROPERTY(m_Size)
SPX_REFLECT_PROPERTY(m_Offset)
SPX_REFLECT_PROPERTY(m_Trigger)
SPX_REFLECT_PROPERTY(m_Debug)

SPX_REFLECT_STRUCT_END(Sphynx::BoxCollider2DComponent, SPHYNX_API)