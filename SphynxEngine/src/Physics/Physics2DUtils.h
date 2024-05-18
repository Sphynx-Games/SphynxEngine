#pragma once

#include "Core/Core.h"
#include "Math/Vector.h"
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

	class SPHYNX_API Collider2D
	{
	public:
		Collider2D(Vector2f offset = { 0.0f, 0.0f }, bool isTrigger = false, bool debug = true) :
			Offset(offset),
			Trigger(isTrigger),
			Debug(debug)
		{}

		virtual ~Collider2D() = default;

	protected:
		Vector2f Offset;
		bool Trigger;
		bool Debug;

		friend class Physics2D;
	};

	class SPHYNX_API BoxCollider2D : public Collider2D
	{
	public:
		BoxCollider2D(Vector2f size = { 1.0, 1.0 }, Vector2f offset = { 0.0f, 0.0f }, bool isTrigger = false, bool debug = true) :
			Collider2D(offset, isTrigger, debug),
			Size(size)
		{}

	private:
		Vector2f Size;

		friend class Physics2D;
	};

	class SPHYNX_API CircleCollider2D : public Collider2D
	{
	public:
		CircleCollider2D(float radius = 0.5f, Vector2f offset = { 0.0f, 0.0f }, bool isTrigger = false, bool debug = true) :
			Collider2D(offset, isTrigger, debug),
			Radius(radius)
		{}

	private:
		float Radius;

		friend class Physics2D;
	};

	class SPHYNX_API CapsuleCollider2D : public Collider2D
	{
	public:
		CapsuleCollider2D(Vector2f size = { 1.0, 1.0 }, Vector2f offset = { 0.0f, 0.0f }, bool isTrigger = false, bool debug = true) :
			Collider2D(offset, isTrigger, debug),
			Size(size)
		{}

	private:
		Vector2f Size;

		friend class Physics2D;
	};
}