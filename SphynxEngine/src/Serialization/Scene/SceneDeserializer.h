#pragma once

#include "Core/Core.h"
#include "Reflection/PropertyTree.h"


namespace Sphynx
{
	class Scene;
	class Actor;
	class Reader;

	namespace Reflection
	{
		struct Class;
	}

	class SPHYNX_API SceneDeserializer
	{
	public:
		SceneDeserializer(Scene& scene, Reader&& reader);

		void Deserialize();

	private:
		void DeserializeComponent(const Reflection::Class& componentClass, Actor& actor);

	private:
		Scene& m_Scene;
		const Reader& m_Reader;
	};
}