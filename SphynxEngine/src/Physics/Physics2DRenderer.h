#pragma once

#include "Core/Core.h"
#include "Math/Transform.h"


namespace Sphynx
{
	class SPHYNX_API Physics2DRenderer
	{
	public:
		static void DrawBoxCollider(class BoxCollider2D* collider, const Transform& transform);
		static void DrawCircleCollider(class CircleCollider2D* collider, const Transform& transform);
		static void DrawCapsuleCollider(class CapsuleCollider2D* collider, const Transform& transform);
	};
}