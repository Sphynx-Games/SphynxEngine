#include "spxpch.h"
#include "EditorSceneDeserializer.h"
#include "EditorScene.h"
#include <Serialization/Reader.h>
#include <Component/Components.h>
#include <Asset/AssetManager.h>
#include <Asset/Prefab/PrefabAsset.h>
#include "Serialization/Actor/ActorDeserializer.h"
/*#include "Scene/Actor.h"
#include "Logging/Log.h"
#include "Serialization/Reflection/ReflectionDeserializer.h"*/


namespace Sphynx
{
	using PrefabComponentContainer = Array<uintptr_t>;

	struct PrefabData
	{
		Actor* actor = nullptr;
		PrefabComponentContainer components;
	};

	namespace Utils
	{
		extern void ActorCoreTraversal(
			Reflection::PropertyTree& tree,
			const Reflection::Property* property,
			void* data,
			Reflection::IPropertyTreeVisitor& visitor);
	}

	EditorSceneDeserializer::EditorSceneDeserializer(EditorScene& scene, Reader&& reader) :
		SceneDeserializer(scene, std::move(reader)),
		m_PrefabEditorComponentsProperty(nullptr)
	{
	}

	EditorSceneDeserializer::~EditorSceneDeserializer()
	{

	}

	void EditorSceneDeserializer::EditorActorDeserializeTraversal(Reflection::PropertyTree& tree, const Reflection::Property* property, void* data, Reflection::IPropertyTreeVisitor& visitor)
	{
		using namespace Reflection;
		Actor* actor = static_cast<Actor*>(data);

		EditorScene* scene = static_cast<EditorScene*>(actor->GetScene());
		auto it = std::find_if(scene->GetPrefabActors().begin(), scene->GetPrefabActors().end(), [&](const PrefabActor& prefabActor) { return *actor == prefabActor; });
		if (it == scene->GetPrefabActors().end())
		{
			EditorSceneDeserializer& self = static_cast<EditorSceneDeserializer&>(visitor);
			ActorDeserializer::ConfigurePropertyTree(tree, Reflection::GetClass<Actor>());
			ActorDeserializer deserializer{ *static_cast<Actor*>(data), Reader{ self.m_Reader } };
			ActorDeserializer::ActorDeserializeTraversal(tree, property, data, deserializer);
			return;
		}

		// Prefab
		Utils::ActorCoreTraversal(tree, property, data, visitor);

		// Components
		{
			// set deserializer component property
			EditorSceneDeserializer& deserializer = static_cast<EditorSceneDeserializer&>(visitor);

			PrefabData prefabData;
			prefabData.actor = actor;

			const Reflection::Class& cClass = GetClass<Array<uint32_t>>();
			const Property compProperty{ cClass, "Components", 0 };
			deserializer.m_PrefabEditorComponentsProperty = &compProperty;

			const CommonAttribute::IndexedCollection* collection = cClass.GetAttribute<CommonAttribute::IndexedCollection>();
			visitor.OnBeforeVisitClass(&compProperty, &prefabData, *collection);
			const bool skip = !visitor.VisitClass(&compProperty, &prefabData, *collection);

			for (size_t i = 0; !skip && i < prefabData.components.Size(); ++i)
			{
				const Reflection::Class* componentClass = (const Reflection::Class*)prefabData.components.Get(i);
				if (componentClass == nullptr) continue; // this is a component that is removed

				void* component = ComponentRegistry::InvokeGetComponent(*componentClass, *actor, false);
				size_t offset = std::distance((std::byte*)component, (std::byte*)data);

				// first lets visit the index of the array
				const std::string indexStr = std::to_string(i);
				const Property fakeProperty{ *componentClass, indexStr.c_str(), offset };
				visitor.OnBeforeVisitClass(&fakeProperty, nullptr);
				const bool skip = !visitor.VisitClass(&fakeProperty, nullptr);
				if (!skip) // once visited, now we can keep going
				{
					const Property fakeProperty{ *componentClass, componentClass->Name, offset };
					PropertyTree mTree{ fakeProperty.GetType(), component };
					mTree.Traverse(visitor, &fakeProperty);
				}
				visitor.OnAfterVisitClass(&fakeProperty, nullptr);
			}

			visitor.OnAfterVisitClass(&compProperty, &prefabData, *collection);
			deserializer.m_PrefabEditorComponentsProperty = nullptr;
		}
	}

	bool EditorSceneDeserializer::VisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::IndexedCollection& collection)
	{
		const bool isActorArray = collection.GetValueType() == Reflection::GetClass<Actor>();
		const bool isPrefabComponentArray = property == m_PrefabEditorComponentsProperty;
		if (!isActorArray && !isPrefabComponentArray)
		{
			return SceneDeserializer::VisitClass(property, data, collection);
		}

		if (isActorArray)
		{
			VisitEditorActors(property, data, collection);
		}
		else if (isPrefabComponentArray)
		{
			VisitPrefabComponent(property, data, collection);
		}

		return true;
	}

	void EditorSceneDeserializer::Deserialize()
	{
		using namespace Reflection;
		PropertyTree::TraversalParams params;
		params.CustomTraversal[&GetClass<Actor>()] = &EditorActorDeserializeTraversal;
		PropertyTree::Traverse(GetClass<EditorScene>(), &m_Scene, *this, std::move(params));
	}

	void EditorSceneDeserializer::VisitEditorActors(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::IndexedCollection& collection)
	{
		SPX_UNUSED(property);
		SPX_UNUSED(data);
		SPX_UNUSED(collection);
		EditorScene* scene = static_cast<EditorScene*>(&m_Scene);
		const size_t size = GetCollectionCount();
		for (size_t i = 0; i < size; ++i)
		{
			m_Reader.PushIndex(i);
			size_t prefabIndex{};
			const bool isPrefabActor = m_Reader.FindKey("Prefab", prefabIndex);
			AssetHandle prefabHandle = AssetHandle::Invalid;
			if (isPrefabActor)
			{
				const Reflection::Property prefabProperty{ Reflection::GetType<AssetHandle>(), "Prefab", 0 };
				Reflection::PropertyTree mTree{ prefabProperty.GetType(), &prefabHandle };
				Reader reader{ m_Reader };
				EditorSceneDeserializer editorSceneDeserializer{ *scene, std::move(reader) };
				mTree.Traverse(editorSceneDeserializer, &prefabProperty);
			}
			m_Reader.PopIndex();

			if (isPrefabActor)
			{
				if (auto assetPtr = AssetManager::GetAsset<Prefab>(prefabHandle))
				{
					scene->CreatePrefabActor(assetPtr->Asset);
				}
				else
				{
					// TODO: create an zombie prefab (invalid but "functional")
					SPX_CORE_LOG_WARNING("Unable to find a Prefab asset with handle: {}", AssetHandle::ToString(prefabHandle));
				}
			}
			else
			{
				// This is a normal actor and will be normally deserialized later
				Actor& actor = scene->CreateActor();
				actor.AddComponent<NameComponent>();
			}
		}
	}

	void EditorSceneDeserializer::VisitPrefabComponent(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::IndexedCollection& collection)
	{
		SPX_UNUSED(property);
		SPX_UNUSED(data);
		SPX_UNUSED(collection);
		const size_t size = GetCollectionCount();
		for (size_t i = 0; i < size; ++i)
		{
			// we need to check if it is a prefab or not
			m_Reader.PushIndex(i);

			m_Reader.PushMap();
			m_Reader.PushKey(0);

			std::string componentName{};
			m_Reader.Read(componentName);
			m_Reader.PopKey();

			auto components = ComponentRegistry::GetComponents();
			auto it = std::find_if(components.begin(), components.end(), [&](const Reflection::Class* cComponent)
				{
					return !strcmp(cComponent->Name, componentName.c_str());
				});

			if (it != components.end())
			{
				m_Reader.PushValue(0);
				const bool isNull = m_Reader.IsNull();
				m_Reader.PopValue();

				const Reflection::Class* cComponent = !isNull ? *it : nullptr;
				PrefabData* prefabData = static_cast<PrefabData*>(data);
				prefabData->components.Add((uintptr_t)cComponent);

				if (cComponent == nullptr)
				{
					if (ComponentRegistry::InvokeHasComponent(**it, *prefabData->actor, false))
					{
						ComponentRegistry::InvokeRemoveComponent(**it, *prefabData->actor);
					}

				}
				else if (!ComponentRegistry::InvokeHasComponent(*cComponent, *prefabData->actor, false))
				{
					ComponentRegistry::InvokeAddComponent(*cComponent, *prefabData->actor);
				}
			}
			else
			{
				SPX_CORE_ASSERT(false, "Component not found in registry!");
			}

			m_Reader.PopMap();
			m_Reader.PopIndex();
		}
	}
}
