#pragma once

#include "Core/Core.h"
#include "Serialization/Reflection/ReflectionDeserializer.h"


namespace Sphynx
{
	class Scene;
	class Reader;

	class SPHYNX_API SceneDeserializer : public ReflectionDeserializer
	{
	public:
		SceneDeserializer(Scene& scene, Reader&& reader);
		virtual ~SceneDeserializer();

	public:
		virtual void Deserialize() override;

	protected:
		virtual bool VisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::IndexedCollection& collection) override;
		
	protected:
		Scene& m_Scene;

	};
}
