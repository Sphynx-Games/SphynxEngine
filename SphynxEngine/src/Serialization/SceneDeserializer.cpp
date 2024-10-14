#include "spxpch.h"
#include "SceneDeserializer.h"
#include "SerializationConstants.h"
#include "Reader.h"
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
	void ReadTransform(Reader& reader, Transform& transform);
	void ReadVector3f(Reader& reader, Vector3f& vector);
	void ReadVector2f(Reader& reader, Vector2f& vector);
	void ReadVector2i(Reader& reader, Vector2i& vector);
	void ReadColor(Reader& reader, Color& color);


	SceneDeserializer::SceneDeserializer(Scene& scene, Reader& reader) :
		m_Scene(scene),
		m_Reader(reader)
	{}

	void SceneDeserializer::Deserialize()
	{
		// scene uuid and name
		//m_Reader.Read(m_Scene.m_UUID);
		m_Reader.Read(m_Scene.m_Name);
		SPX_CORE_LOG_DISPLAY(m_Scene.m_Name);

		// num actors in scene
		size_t numActors;
		m_Reader.Read(numActors);
		SPX_CORE_LOG_DISPLAY(numActors);

		for (size_t i = 0; i < numActors; ++i)
		{
			// actor id
			Actor& actor = m_Scene.CreateActor();
			//m_Reader.Read(actor.m_EntityID);

			// num of components in actor
			size_t numComponents;
			m_Reader.Read(numComponents);
			SPX_CORE_LOG_DISPLAY(numComponents);

			// UUID Component : every actor has a UUIDComponent
			/*UUIDComponent& component = actor.GetComponent<UUIDComponent>();
			UUID uuid;
			m_Reader.Read(uuid);
			component.UUID = std::move(uuid);*/

			for (size_t j = 0; j < numComponents - 1; ++j)
			{
				std::string componentName;
				m_Reader.Read(componentName);
				SPX_CORE_LOG_DISPLAY(componentName);

				// Name Component
				if (!strcmp(componentName.c_str(), NameComponent_NAME))
				{
					NameComponent& component = actor.AddComponent<NameComponent>();
					std::string name;
					m_Reader.Read(name);
					component.Name = name;
					SPX_CORE_LOG_DISPLAY(name);
				}
				// Transform Component
				else if (!strcmp(componentName.c_str(), TransformComponent_NAME))
				{
					TransformComponent& component = actor.AddComponent<TransformComponent>();
					ReadTransform(m_Reader, component.Transform);
				}
				// LineRenderer Component
				else if (!strcmp(componentName.c_str(), LineRendererComponent_NAME))
				{
					LineRendererComponent& component = actor.AddComponent<LineRendererComponent>();
					ReadVector2f(m_Reader, component.Point1);
					ReadVector2f(m_Reader, component.Point2);
					float linewidth;
					m_Reader.Read(linewidth);
					component.LineWidth = linewidth;
					SPX_CORE_LOG_DISPLAY(linewidth);
					ReadColor(m_Reader, component.Color);
				}
				// SpriteRenderer Component
				else if (!strcmp(componentName.c_str(), SpriteRendererComponent_NAME))
				{
					SpriteRendererComponent& component = actor.AddComponent<SpriteRendererComponent>();
					ReadVector2i(m_Reader, component.Sprite->GetTexture()->GetSize());
					ReadVector2i(m_Reader, component.Sprite->GetPosition());
					ReadVector2i(m_Reader, component.Sprite->GetSize());
					ReadVector2f(m_Reader, component.Sprite->GetPivot());
					uint32_t pixelsPerUnit;
					m_Reader.Read(pixelsPerUnit);
					component.Sprite->SetPixelsPerUnit(pixelsPerUnit);
					SPX_CORE_LOG_DISPLAY(pixelsPerUnit);
					ReadColor(m_Reader, component.Tint);
				}
				// BoxRenderer Component
				else if (!strcmp(componentName.c_str(), BoxRendererComponent_NAME))
				{
					BoxRendererComponent& component = actor.AddComponent<BoxRendererComponent>();
					DrawMode drawMode;
					m_Reader.Read(drawMode);
					component.DrawMode = drawMode;
					SPX_CORE_LOG_DISPLAY(drawMode);
					ReadVector2f(m_Reader, component.Size);
					ReadVector2f(m_Reader, component.Pivot);
					ReadColor(m_Reader, component.Color);
				}
				// Rigidbody Component
				else if (!strcmp(componentName.c_str(), Rigidbody2DComponent_NAME))
				{
					Rigidbody2DComponent& component = actor.AddComponent<Rigidbody2DComponent>();
					bool enabled;
					m_Reader.Read(enabled);
					component.SetEnabled(enabled);
					SPX_CORE_LOG_DISPLAY(enabled);
					RigidbodyType type;
					m_Reader.Read(type);
					component.SetRigidbodyType(type);
					SPX_CORE_LOG_DISPLAY(type);
					ReadVector2f(m_Reader, component.GetLinearVelocity());
					float value;
					m_Reader.Read(value);
					component.SetAngularVelocity(value);
					SPX_CORE_LOG_DISPLAY(value);
					m_Reader.Read(value);
					component.SetLinearDamping(value);
					SPX_CORE_LOG_DISPLAY(value);
					m_Reader.Read(value);
					component.SetAngularDamping(value);
					SPX_CORE_LOG_DISPLAY(value);
					m_Reader.Read(value);
					component.SetGravityScale(value);
					SPX_CORE_LOG_DISPLAY(value);
				}
				// BoxCollider Component
				else if (!strcmp(componentName.c_str(), BoxCollider2DComponent_NAME))
				{
					BoxCollider2DComponent& component = actor.AddComponent<BoxCollider2DComponent>();
					ReadVector2f(m_Reader, component.GetSize());
					ReadVector2f(m_Reader, component.GetOffset());
					bool boolValue;
					m_Reader.Read(boolValue);
					component.SetIsTrigger(boolValue);
					SPX_CORE_LOG_DISPLAY(boolValue);
					m_Reader.Read(boolValue);
					component.SetDebug(boolValue);
					SPX_CORE_LOG_DISPLAY(boolValue);
				}
				// CircleCollider Component
				else if (!strcmp(componentName.c_str(), CircleCollider2DComponent_NAME))
				{
					CircleCollider2DComponent& component = actor.AddComponent<CircleCollider2DComponent>();
					float value;
					m_Reader.Read(value);
					component.SetRadius(value);
					SPX_CORE_LOG_DISPLAY(value);
					ReadVector2f(m_Reader, component.GetOffset());
					bool boolValue;
					m_Reader.Read(boolValue);
					component.SetIsTrigger(boolValue);
					SPX_CORE_LOG_DISPLAY(boolValue);
					m_Reader.Read(boolValue);
					component.SetDebug(boolValue);
					SPX_CORE_LOG_DISPLAY(boolValue);
				}
				// CapsuleCollider Component
				else if (!strcmp(componentName.c_str(), CapsuleCollider2DComponent_NAME))
				{
					CapsuleCollider2DComponent& component = actor.AddComponent<CapsuleCollider2DComponent>();
					ReadVector2f(m_Reader, component.GetSize());
					ReadVector2f(m_Reader, component.GetOffset());
					bool boolValue;
					m_Reader.Read(boolValue);
					component.SetIsTrigger(boolValue);
					SPX_CORE_LOG_DISPLAY(boolValue);
					m_Reader.Read(boolValue);
					component.SetDebug(boolValue);
					SPX_CORE_LOG_DISPLAY(boolValue);
				}
			}
		}
	}

	void ReadTransform(Reader& reader, Transform& transform)
	{
		Transform trans;
		ReadVector3f(reader, trans.Position);
		ReadVector3f(reader, trans.Rotation);
		ReadVector3f(reader, trans.Scale);
		transform = trans;
	}

	void ReadVector3f(Reader& reader, Vector3f& vector)
	{
		Vector3f vec;
		reader.Read(vec.X);
		reader.Read(vec.Y);
		reader.Read(vec.Z);
		vector = vec;

		SPX_CORE_LOG_DISPLAY("[ " + std::to_string(vector.X) + ", " + std::to_string(vector.Y) + ", " + std::to_string(vector.Z) + " ]");
	}

	void ReadVector2f(Reader& reader, Vector2f& vector)
	{
		Vector2f vec;
		reader.Read(vec.X);
		reader.Read(vec.Y);
		vector = vec;

		SPX_CORE_LOG_DISPLAY("[ " + std::to_string(vector.X) + ", " + std::to_string(vector.Y) + " ]");
	}

	void ReadVector2i(Reader& reader, Vector2i& vector)
	{
		Vector2i vec;
		reader.Read(vec.X);
		reader.Read(vec.Y);
		vector = vec;

		SPX_CORE_LOG_DISPLAY("[ " + std::to_string(vector.X) + ", " + std::to_string(vector.Y) + " ]");
	}

	void ReadColor(Reader& reader, Color& color)
	{
		Color col;
		reader.Read(col.R);
		reader.Read(col.G);
		reader.Read(col.B);
		reader.Read(col.A);
		color = col;

		SPX_CORE_LOG_DISPLAY("[ " + std::to_string(color.R) + ", " + std::to_string(color.G) + ", " + std::to_string(color.B) + ", " + std::to_string(color.A) + " ]");
	}
}