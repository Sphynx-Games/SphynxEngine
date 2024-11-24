#pragma once

#include "Core/Core.h"
#include "Reflection/ReflectionSerializer.h"
#include "Scene/Actor.h"


namespace Sphynx
{
	class SPHYNX_API SceneSerializer
	{
	public:
		SceneSerializer(const class Scene& scene, class Writer&& writer);

		void Serialize();

	private:
		template<typename T>
		void SerializeComponent(const Actor& actor)
		{
			if (actor.HasComponent<T>())
			{
				m_Writer.PushMap();
				m_Writer.PushKey();
				m_Writer.Write(Reflection::GetType<T>().Name);

				m_Writer.PushKey();
				const T& component = actor.GetComponent<T>();
				ReflectionSerializer serializer(component, m_Writer);
				serializer.Serialize();
				m_Writer.PopMap();
			}
		}

	private:
		const Scene& m_Scene;
		Writer& m_Writer;
	};
}