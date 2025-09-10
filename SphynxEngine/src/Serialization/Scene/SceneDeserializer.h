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

		virtual void Deserialize();

	protected:
		size_t DeserializeSceneProperties();
		//void DeserializeCoreActorComponents(UUID& uuid, std::string& name);
		void DeserializeActor();
		void EndDeserialize();

	private:
		void DeserializeComponent(const Reflection::Class& componentClass, Actor& actor);

	protected:
		Scene& m_Scene;
		const Reader& m_Reader;
	};
}