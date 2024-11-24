#pragma once

#include "Core/Core.h"
#include "Scene/Actor.h"
#include "Reflection/ReflectionDeserializer.h"


namespace Sphynx
{
	class SPHYNX_API SceneDeserializer
	{
	public:
		SceneDeserializer(class Scene& scene, const class Reader& reader);
		void Deserialize();

	private:
		template<typename T>
		void DeserializeComponent(Actor& actor, const std::string& componentName)
		{
			if (!strcmp(componentName.c_str(), Reflection::GetType<T>().Name))
			{
				T& component = actor.AddComponent<T>();
				ReflectionDeserializer deserializer(component, m_Reader);
				deserializer.Deserialize();
			}
		}

	private:
		Scene& m_Scene;
		const Reader& m_Reader;
	};
}