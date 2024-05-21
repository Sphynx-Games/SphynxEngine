#pragma once

#include "Core/Core.h"
#include "Math/Vector.h"
#include "Physics/Rigidbody2D.h"


namespace Sphynx
{
	class SPHYNX_API Collider2D
	{
	public:
		virtual ~Collider2D() = default;

		Vector2f GetOffset() { return Offset; }
		void SetOffset(Vector2f offset) { Offset = offset; }

		bool IsTrigger() { return Trigger; }
		void SetTrigger(bool trigger) { Trigger = trigger; }

		bool NeedsDebug() { return Debug; }
		void SetDebug(bool debug) { Debug = debug; }

		Rigidbody2D* GetRigidbody() { return m_Rigidbody; }
		void SetRigidbody(Rigidbody2D* rigidbody) { m_Rigidbody = rigidbody; }

	protected:
		Collider2D(Vector2f offset = { 0.0f, 0.0f }, bool isTrigger = false, bool debug = true) :
			Offset(offset),
			Trigger(isTrigger),
			Debug(debug),
			m_Rigidbody(nullptr)
		{}

	protected:
		Vector2f Offset;
		bool Trigger;
		bool Debug;

		class Rigidbody2D* m_Rigidbody;
	};

	class SPHYNX_API BoxCollider2D : public Collider2D
	{
	public:
		virtual ~BoxCollider2D() = default;

		Vector2f GetSize() { return Size; }
		void SetSize(Vector2f size) { Size = size; }

	protected:
		BoxCollider2D(Vector2f size = { 1.0, 1.0 }, Vector2f offset = { 0.0f, 0.0f }, bool isTrigger = false, bool debug = true) :
			Collider2D(offset, isTrigger, debug),
			Size(size)
		{}

	protected:
		Vector2f Size;
	};

	class SPHYNX_API CircleCollider2D : public Collider2D
	{
	public:
		virtual ~CircleCollider2D() = default;

		float GetRadius() { return Radius; }
		void SetRadius(float radius) { Radius = radius; }

	protected:
		CircleCollider2D(float radius = 0.5f, Vector2f offset = { 0.0f, 0.0f }, bool isTrigger = false, bool debug = true) :
			Collider2D(offset, isTrigger, debug),
			Radius(radius)
		{}

	protected:
		float Radius;
	};

	class SPHYNX_API CapsuleCollider2D : public Collider2D
	{
	public:
		virtual ~CapsuleCollider2D() = default;

		Vector2f GetSize() { return Size; }
		void SetSize(Vector2f size) { Size = size; }

	protected:
		CapsuleCollider2D(Vector2f size = { 1.0, 1.0 }, Vector2f offset = { 0.0f, 0.0f }, bool isTrigger = false, bool debug = true) :
			Collider2D(offset, isTrigger, debug),
			Size(size)
		{}

	protected:
		Vector2f Size;
	};
}