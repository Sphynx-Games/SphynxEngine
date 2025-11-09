#include "spxpch.h"
#include "SceneDeserializer.h"
#include "Serialization/Reader.h"
#include "Serialization/Actor/ActorDeserializer.h"
#include "Scene/Scene.h"
#include "Scene/Actor.h"

#include "Reflection/PropertyTree.h"
#include "Component/ComponentRegistry.h"
#include "Component/UUIDComponent.h"
#include "Component/NameComponent.h"


namespace Sphynx
{
	SceneDeserializer::SceneDeserializer(Scene& scene, Reader&& reader) :
		ReflectionDeserializer(&scene, Reflection::GetType<Scene>(), reader),
		m_Scene(scene)
	{

	}

	SceneDeserializer::~SceneDeserializer()
	{

	}

	void SceneDeserializer::Deserialize()
	{
		using namespace Reflection;
		PropertyTree::TraversalParams params;
		params.CustomTraversal[&GetClass<Actor>()] = [](PropertyTree& tree, const Property* prop, void* addr, IPropertyTreeVisitor& visitor)
			{
				SceneDeserializer& self = static_cast<SceneDeserializer&>(visitor);
				Reader reader{ self.m_Reader };
				tree.Traverse(ActorDeserializer{ *static_cast<Actor*>(addr), std::move(reader) }, prop);
			};
		PropertyTree::Traverse(GetClass<Scene>(), &m_Scene, *this, std::move(params));
	}

	bool SceneDeserializer::VisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::IndexedCollection& collection)
	{
		if (&collection.GetValueType() != &Reflection::GetClass<Actor>())
			return ReflectionDeserializer::VisitClass(property, data, collection);

		const size_t size = GetCollectionCount();
		for (size_t i = 0; i < size; ++i)
		{
			// this is a special case
			if (&collection.GetValueType() == &Reflection::GetClass<Actor>())
			{
				Actor& actor = m_Scene.CreateActor(); // this will add an element to the array
				actor.AddComponent<NameComponent>();
			}
			else
			{
				SPX_CORE_ASSERT(false, "This should not execute!");
			}
		}

		return true;
	}
}
