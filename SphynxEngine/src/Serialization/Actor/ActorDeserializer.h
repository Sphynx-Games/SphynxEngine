#pragma once

#include "Core/Core.h"
#include "Serialization/Reflection/ReflectionDeserializer.h"


namespace Sphynx
{
	class Actor;
	class Reader;

	namespace Reflection
	{
		struct Class;
	}

	class SPHYNX_API ActorDeserializer : public ReflectionDeserializer
	{
	public:
		ActorDeserializer(Actor& actor, Reader&& reader);
		ActorDeserializer(Actor& actor, const Reflection::Class& cClass, Reader&& reader);
		virtual ~ActorDeserializer();

	public:
		static void ActorDeserializeTraversal(Reflection::PropertyTree& tree, const Reflection::Property* property, void* data, Reflection::IPropertyTreeVisitor& visitor);
		static void ConfigurePropertyTree(Reflection::PropertyTree& tree, const Reflection::Type& type);
		virtual void Deserialize() override;

	protected:
		virtual bool VisitClass(const Reflection::Property* property, void* data) override;
		virtual bool VisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::IndexedCollection& collection) override;

		virtual void OnBeforeVisitClass(const Reflection::Property* property, void* data) override;
		virtual void OnAfterVisitClass(const Reflection::Property* property, void* data) override;

	protected:
		Actor& m_Actor;
		const Reflection::Property* m_ComponentsProperty;

	};
}