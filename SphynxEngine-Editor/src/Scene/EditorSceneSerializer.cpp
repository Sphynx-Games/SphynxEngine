#include "spxpch.h"
#include "EditorSceneSerializer.h"
#include "EditorScene.h"
#include "Scene/PrefabActor.h"
#include <Asset/AssetManager.h>
#include <Component/UUIDComponent.h>
#include <Component/NameComponent.h>
#include <Component/ComponentRegistry.h>
#include <Reflection/PropertyComparator.h>
#include <Reflection/PropertyTree.h>
#include "Scene/MatchingVisitor.h"


namespace Sphynx
{
	namespace Utils
	{
		struct ComponentDiffFound
		{
		public:
			void operator()(const Reflection::PropertyDiffInfo& info)
			{
				switch (info.Type)
				{
				case Reflection::PropertyDiffType::DIFFERENT_VALUE:
					int stackSize = info.TargetStack.Size();
					for (int i = 0; i < stackSize - 1; ++i)
					{
						DiffTree[{info.TargetStack[i].Property, nullptr}].Add({info.TargetStack[i + 1].Property, nullptr});
					}
					void* targetDataDir = (std::byte*)info.TargetData + info.SourceProperty->Offset;
					DiffTree[{info.TargetStack[stackSize - 1].Property, nullptr}].Add({info.SourceProperty, targetDataDir});
					DiffTree[{info.SourceProperty, targetDataDir}];
				}
			}

		public:
			HashMap<Pair<const Reflection::Property*, void*>, Array<Pair<const Reflection::Property*, void*>>> DiffTree;
		};

		static void EditorActorTraversal(
			Reflection::PropertyTree& tree,
			const Reflection::Property* property,
			void* data,
			Reflection::IPropertyTreeVisitor& visitor)
		{
			using namespace Reflection;
			Actor* actor = static_cast<Actor*>(data);

			// ACTOR
			EditorScene* scene = static_cast<EditorScene*>(actor->GetScene());
			auto it = std::find_if(scene->GetPrefabActors().begin(), scene->GetPrefabActors().end(), [&](const PrefabActor& prefabActor) { return *actor == prefabActor; });
			if (it == scene->GetPrefabActors().end())
			{
				ActorTraversal(tree, property, data, visitor);
				return;
			}			

			// PREFAB ACTOR
			// Prefab reference
			{
				AssetHandle prefabHandle = AssetManager::GetAssetHandleFromAddress(it->GetPrefab());
				SPX_CORE_ASSERT(prefabHandle != AssetHandle::Invalid, "Prefab asset handle is invalid!!");

				const Property property{ GetType<AssetHandle>(), "Prefab", 0 };
				PropertyTree mTree{ property.Type, (void*)&prefabHandle };
				mTree.Traverse(visitor, &property);
			}

			// UUID and Name Components
			ActorCoreTraversal(tree, property, data, visitor);

			// Components
			{
				Array<const Reflection::Class*> actorComponents;
				for (const Reflection::Class* cClass : ComponentRegistry::GetComponents())
				{
					if (cClass == &Reflection::GetClass<UUIDComponent>() ||
						cClass == &Reflection::GetClass<NameComponent>()) continue;

					actorComponents.Add(cClass);
				}
				const Reflection::Class& cClass = GetClass<Array<uintptr_t>>();
				const Property property{ cClass, "Components", 0 };
				const CommonAttribute::IndexedCollection* collection = cClass.GetAttribute<CommonAttribute::IndexedCollection>();
				visitor.OnBeforeVisitClass(&property, &actorComponents, *collection);
				const bool skip = !visitor.VisitClass(&property, &actorComponents, *collection);
				if (!skip)
				{
					for (const Reflection::Class* cClass : actorComponents)
					{
						bool actorHasComponent = ComponentRegistry::InvokeHasComponent(*cClass, *it, false);
						bool prefabHasComponent = ComponentRegistry::InvokeHasComponent(*cClass, *it->GetPrefab(), false);
						if (actorHasComponent)
						{
							void* componentActor = ComponentRegistry::InvokeGetComponent(*cClass, *it, false);
							if (prefabHasComponent)
							{
								void* componentPrefab = ComponentRegistry::InvokeGetComponent(*cClass, *it->GetPrefab(), false);

								Utils::ComponentDiffFound diff;
								Reflection::PropertyComparator comparator{ componentActor, *cClass };
								comparator.OnPropertyDiffFound.Bind(&diff, &Utils::ComponentDiffFound::operator());
								Reflection::PropertyTree::Traverse(*cClass, componentPrefab, comparator);

								MatchingVisitor matchingVisitor{ visitor, diff.DiffTree };
								Reflection::PropertyTree::Traverse(*cClass, componentActor, matchingVisitor);
							}
							else
							{
								size_t offset = std::distance((std::byte*)componentActor, (std::byte*)data);
								const Property fakeProperty{ *cClass, cClass->Name, offset };
								PropertyTree mTree{ fakeProperty.Type, componentActor };
								mTree.Traverse(visitor, &fakeProperty);
							}
						}
						else if (prefabHasComponent)
						{
							const Property fakeProperty{ *cClass, cClass->Name, 0 };
							PropertyTree mTree{ fakeProperty.Type, nullptr };
							mTree.Traverse(visitor, &fakeProperty);
						}
					}
				}

				visitor.OnAfterVisitClass(&property, &actorComponents, *collection);
			}
		}
	}

	EditorSceneSerializer::EditorSceneSerializer(const EditorScene& scene, Writer&& writer) :
		SceneSerializer(scene, std::move(writer))
	{}

	bool EditorSceneSerializer::VisitClass(const Reflection::Property* property, void* data)
	{
		if (data == nullptr)
		{
			m_Writer.PushMap();

			m_Writer.PushKey();
			m_Writer.Write(property->Name);

			m_Writer.PushValue();
			m_Writer.Write((const void*)nullptr, 0ULL);

			m_Writer.PopMap();
			return false;
		}

		return SceneSerializer::VisitClass(property, data);
	}

	void EditorSceneSerializer::OnBeforeVisitClass(const Reflection::Property* property, void* data)
	{
		if (data == nullptr) return;
		SceneSerializer::OnBeforeVisitClass(property, data);
	}

	void EditorSceneSerializer::OnAfterVisitClass(const Reflection::Property* property, void* data)
	{
		if (data == nullptr) return;
		SceneSerializer::OnAfterVisitClass(property, data);
	}

	void EditorSceneSerializer::Serialize()
	{
		using namespace Reflection;
		PropertyTree::TraversalParams params;
		params.CustomTraversal[&GetClass<Actor>()] = &Utils::EditorActorTraversal;
		PropertyTree::Traverse(GetClass<EditorScene>(), (void*)&m_Scene, *this, std::move(params));
	}
}