#pragma once

#include "Core/Core.h"
#include "Scene/Actor.h"
#include "Reflection/ReflectionDeserializer.h"


namespace Sphynx
{
	class SPHYNX_API SceneDeserializer
	{
	public:
		SceneDeserializer(class Scene& scene, Reader&& reader);
		void Deserialize();

	private:
		void DeserializeComponent(const Reflection::Class& componentClass, Actor& actor);

	private:
		Scene& m_Scene;
		const Reader& m_Reader;
	};
}