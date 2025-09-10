#pragma once

#include <Core/Core.h>
#include <Serialization/Scene/SceneSerializer.h>


namespace Sphynx
{
	class EditorScene;
	class Writer;

	class EditorSceneSerializer : public SceneSerializer
	{
	public:
		EditorSceneSerializer(const EditorScene& scene, Writer&& writer);

		virtual bool VisitClass(const Reflection::Property* property, void* data) override;
		virtual void OnBeforeVisitClass(const Reflection::Property* property, void* data) override;
		virtual void OnAfterVisitClass(const Reflection::Property* property, void* data) override;

	public:
		void Serialize();

	};
}