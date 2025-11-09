#pragma once

#include <Core/Core.h>
#include <Serialization/Scene/SceneDeserializer.h>


namespace Sphynx
{
	class EditorScene;

	class EditorSceneDeserializer : public SceneDeserializer
	{
	public:
		EditorSceneDeserializer(EditorScene& scene, Reader&& reader);
		virtual ~EditorSceneDeserializer();

	protected:
		static void EditorActorDeserializeTraversal(Reflection::PropertyTree& tree, const Reflection::Property* property, void* data, Reflection::IPropertyTreeVisitor& visitor);
		virtual bool VisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::IndexedCollection& collection) override;

	public:
		void Deserialize();

	private:
		void VisitEditorActors(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::IndexedCollection& collection);
		void VisitPrefabComponent(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::IndexedCollection& collection);

	protected:
		const Reflection::Property* m_PrefabEditorComponentsProperty;

	};
}