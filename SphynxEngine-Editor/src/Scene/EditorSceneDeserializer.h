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

	public:
		EditorSceneDeserializer(EditorScene& scene, Reader&& reader);

	protected:
		virtual bool VisitClass(const Reflection::Property* property, void* data) override;
		virtual bool VisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::IndexedCollection& collection) override;

		virtual void OnBeforeVisitClass(const Reflection::Property* property, void* data) override;
		virtual void OnAfterVisitClass(const Reflection::Property* property, void* data) override;

	public:
		void Deserialize();

		void SetPrefabComponentsProperty(const Reflection::Property* property);

	protected:
		const Reflection::Property* m_PrefabEditorComponentsProperty;

	};
}