#pragma once

#include <Core/Core.h>
#include <Serialization/Scene/SceneDeserializer.h>


namespace Sphynx
{
	class EditorScene;
	class Prefab;

	class EditorSceneDeserializer : public SceneDeserializer
	{
	public:
		EditorSceneDeserializer(EditorScene& scene, Reader&& reader);

		virtual void Deserialize() override;

	private:
		void DeserializePrefabActor(Prefab* prefab);
		void DeserializePrefabComponent(const Reflection::Class& componentClass, Actor& actor);

	};
}