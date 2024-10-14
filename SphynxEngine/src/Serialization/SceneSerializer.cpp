#include "spxpch.h"
#include "SceneSerializer.h"
#include "SerializationConstants.h"
#include "Writer.h"
#include "Scene/Scene.h"
#include "Scene/Actor.h"
#include "Component/UUIDComponent.h"
#include "Component/NameComponent.h"
#include "Component/TransformComponent.h"
#include "Component/LineRendererComponent.h"
#include "Component/BoxRendererComponent.h"
#include "Component/SpriteRendererComponent.h"
#include "Component/Physics/Rigidbody2DComponent.h"
#include "Component/Physics/BoxCollider2DComponent.h"
#include "Component/Physics/CircleCollider2DComponent.h"
#include "Component/Physics/CapsuleCollider2DComponent.h"
#include "Renderer/Sprite.h"
#include "Renderer/Texture.h"
#include "Logging/Log.h"


namespace Sphynx
{
	void WriteTransform(Writer& writer, const Transform& transform);
	void WriteVector3f(Writer& writer, const Vector3f& vector);
	void WriteVector2f(Writer& writer, const Vector2f& vector);
	void WriteVector2i(Writer& writer, const Vector2i& vector);
	void WriteColor(Writer& writer, const Color& color);


	SceneSerializer::SceneSerializer(const Scene& scene, Writer& writer) :
		m_Scene(scene),
		m_Writer(writer)
	{}

	void SceneSerializer::Serialize()
	{
		// scene uuid and name
		//m_Writer.Write(m_Scene.m_UUID);
		m_Writer.Write(m_Scene.m_Name);
		SPX_CORE_LOG_DISPLAY(m_Scene.m_Name);

		// num actors in scene
		m_Writer.Write(m_Scene.m_Actors.size());
		SPX_CORE_LOG_DISPLAY(m_Scene.m_Actors.size());

		for (const Actor& actor : m_Scene.m_Actors)
		{
			// actor id
			//m_Writer.Write(actor.m_EntityID);

			// num of components in actor
			m_Writer.Write(actor.GetNumComponents());
			SPX_CORE_LOG_DISPLAY(actor.GetNumComponents());

			// UUID Component : every actor has a UUIDComponent
			/*const UUIDComponent& component = actor.GetComponent<UUIDComponent>();
			m_Writer.Write(UUIDComponent_NAME);
			m_Writer.Write(component.UUID);*/

			// Name Component
			if (actor.HasComponent<NameComponent>())
			{
				const NameComponent& component = actor.GetComponent<NameComponent>();
				m_Writer.Write(NameComponent_NAME);
				SPX_CORE_LOG_DISPLAY(NameComponent_NAME);
				m_Writer.Write(component.Name);
				SPX_CORE_LOG_DISPLAY(component.Name);
			}

			// Transform Component
			if (actor.HasComponent<TransformComponent>())
			{
				const TransformComponent& component = actor.GetComponent<TransformComponent>();
				m_Writer.Write(TransformComponent_NAME);
				SPX_CORE_LOG_DISPLAY(TransformComponent_NAME);
				WriteTransform(m_Writer, component.Transform);
			}

			// LineRenderer Component
			if (actor.HasComponent<LineRendererComponent>())
			{
				const LineRendererComponent& component = actor.GetComponent<LineRendererComponent>();
				m_Writer.Write(LineRendererComponent_NAME);
				SPX_CORE_LOG_DISPLAY(LineRendererComponent_NAME);
				WriteVector2f(m_Writer, component.Point1);
				WriteVector2f(m_Writer, component.Point2);
				m_Writer.Write(component.LineWidth);
				SPX_CORE_LOG_DISPLAY(component.LineWidth);
				WriteColor(m_Writer, component.Color);
			}

			// SpriteRenderer Component
			if (actor.HasComponent<SpriteRendererComponent>())
			{
				const SpriteRendererComponent& component = actor.GetComponent<SpriteRendererComponent>();
				m_Writer.Write(SpriteRendererComponent_NAME);
				SPX_CORE_LOG_DISPLAY(SpriteRendererComponent_NAME);
				WriteVector2i(m_Writer, component.Sprite->GetTexture()->GetSize());
				WriteVector2i(m_Writer, component.Sprite->GetPosition());
				WriteVector2i(m_Writer, component.Sprite->GetSize());
				WriteVector2f(m_Writer, component.Sprite->GetPivot());
				m_Writer.Write(component.Sprite->GetPixelsPerUnit());
				SPX_CORE_LOG_DISPLAY(component.Sprite->GetPixelsPerUnit());
				WriteColor(m_Writer, component.Tint);
			}

			// BoxRenderer Component
			if (actor.HasComponent<BoxRendererComponent>())
			{
				const BoxRendererComponent& component = actor.GetComponent<BoxRendererComponent>();
				m_Writer.Write(BoxRendererComponent_NAME);
				SPX_CORE_LOG_DISPLAY(BoxRendererComponent_NAME);
				m_Writer.Write(component.DrawMode);
				SPX_CORE_LOG_DISPLAY(component.DrawMode);
				WriteVector2f(m_Writer, component.Size);
				WriteVector2f(m_Writer, component.Pivot);
				WriteColor(m_Writer, component.Color);
			}

			// Rigidbody Component
			if (actor.HasComponent<Rigidbody2DComponent>())
			{
				const Rigidbody2DComponent& component = actor.GetComponent<Rigidbody2DComponent>();
				m_Writer.Write(Rigidbody2DComponent_NAME);
				SPX_CORE_LOG_DISPLAY(Rigidbody2DComponent_NAME);
				m_Writer.Write(component.IsEnabled());
				SPX_CORE_LOG_DISPLAY(component.IsEnabled());
				m_Writer.Write(component.GetRigidbodyType());
				SPX_CORE_LOG_DISPLAY(component.GetRigidbodyType());
				WriteVector2f(m_Writer, component.GetLinearVelocity());
				m_Writer.Write(component.GetAngularVelocity());
				SPX_CORE_LOG_DISPLAY(component.GetAngularVelocity());
				m_Writer.Write(component.GetLinearDamping());
				SPX_CORE_LOG_DISPLAY(component.GetLinearDamping());
				m_Writer.Write(component.GetAngularDamping());
				SPX_CORE_LOG_DISPLAY(component.GetAngularDamping());
				m_Writer.Write(component.GetGravityScale());
				SPX_CORE_LOG_DISPLAY(component.GetGravityScale());
			}

			// BoxCollider Component
			if (actor.HasComponent<BoxCollider2DComponent>())
			{
				const BoxCollider2DComponent& component = actor.GetComponent<BoxCollider2DComponent>();
				m_Writer.Write(BoxCollider2DComponent_NAME);
				SPX_CORE_LOG_DISPLAY(BoxCollider2DComponent_NAME);
				WriteVector2f(m_Writer, component.GetSize());
				WriteVector2f(m_Writer, component.GetOffset());
				m_Writer.Write(component.IsTrigger());
				SPX_CORE_LOG_DISPLAY(component.IsTrigger());
				m_Writer.Write(component.NeedsDebug());
				SPX_CORE_LOG_DISPLAY(component.NeedsDebug());
			}

			// CircleCollider Component
			if (actor.HasComponent<CircleCollider2DComponent>())
			{
				const CircleCollider2DComponent& component = actor.GetComponent<CircleCollider2DComponent>();
				m_Writer.Write(CircleCollider2DComponent_NAME);
				SPX_CORE_LOG_DISPLAY(CircleCollider2DComponent_NAME);
				m_Writer.Write(component.GetRadius());
				SPX_CORE_LOG_DISPLAY(component.GetRadius());
				WriteVector2f(m_Writer, component.GetOffset());
				m_Writer.Write(component.IsTrigger());
				SPX_CORE_LOG_DISPLAY(component.IsTrigger());
				m_Writer.Write(component.NeedsDebug());
				SPX_CORE_LOG_DISPLAY(component.NeedsDebug());
			}

			// CapsuleCollider Component
			if (actor.HasComponent<CapsuleCollider2DComponent>())
			{
				const CapsuleCollider2DComponent& component = actor.GetComponent<CapsuleCollider2DComponent>();
				m_Writer.Write(CapsuleCollider2DComponent_NAME);
				SPX_CORE_LOG_DISPLAY(CapsuleCollider2DComponent_NAME);
				WriteVector2f(m_Writer, component.GetSize());
				WriteVector2f(m_Writer, component.GetOffset());
				m_Writer.Write(component.IsTrigger());
				SPX_CORE_LOG_DISPLAY(component.IsTrigger());
				m_Writer.Write(component.NeedsDebug());
				SPX_CORE_LOG_DISPLAY(component.NeedsDebug());
			}
		}
	}

	void WriteTransform(Writer& writer, const Transform& transform)
	{
		WriteVector3f(writer, transform.Position);
		WriteVector3f(writer, transform.Rotation);
		WriteVector3f(writer, transform.Scale);
	}

	void WriteVector3f(Writer& writer, const Vector3f& vector)
	{
		writer.Write(vector.X);
		writer.Write(vector.Y);
		writer.Write(vector.Z);

		SPX_CORE_LOG_DISPLAY("[ " + std::to_string(vector.X) + ", " + std::to_string(vector.Y) + ", " + std::to_string(vector.Z) + " ]");
	}

	void WriteVector2f(Writer& writer, const Vector2f& vector)
	{
		writer.Write(vector.X);
		writer.Write(vector.Y);

		SPX_CORE_LOG_DISPLAY("[ " + std::to_string(vector.X) + ", " + std::to_string(vector.Y) + " ]");
	}

	void WriteVector2i(Writer& writer, const Vector2i& vector)
	{
		writer.Write(vector.X);
		writer.Write(vector.Y);

		SPX_CORE_LOG_DISPLAY("[ " + std::to_string(vector.X) + ", " + std::to_string(vector.Y) + " ]");
	}

	void WriteColor(Writer& writer, const Color& color)
	{
		writer.Write(color.R);
		writer.Write(color.G);
		writer.Write(color.B);
		writer.Write(color.A);

		SPX_CORE_LOG_DISPLAY("[ " + std::to_string(color.R) + ", " + std::to_string(color.G) + ", " + std::to_string(color.B) + ", " + std::to_string(color.A) + " ]");
	}
}