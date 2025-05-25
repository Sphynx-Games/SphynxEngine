#pragma once

#include "Core/Core.h"

#include "Reflection/Class.h"
#include "Container/Array.h"
#include "Container/Set.h"

#include "Scene/Scene.h"
#include "Scene/Actor.h"


namespace Sphynx
{
	struct SPHYNX_API ComponentFunctions
	{
		// Actor functions
		void*(*AddComponent)(Actor&);
		void(*RemoveComponent)(Actor&);
		void*(*GetComponent)(const Actor&);
		bool(*HasComponent)(const Actor&);
		void(*CloneComponent)(const Actor&, Actor&); // const Actor& source, Actor& target

		// Scene functions
		Array<Actor>(*GetActorsByComponent)(const Scene&);
		Array<void*>(*GetSceneComponents)(const Scene&);
	};

	class SPHYNX_API ComponentRegistry
	{
	public:
		template<typename TComponent>
		static void Register()
		{
			RegisterDeferred(&Reflection::GetClass<TComponent>);
			ComponentFunctions& compFunc = s_ComponentFunctions.Emplace();
			compFunc.AddComponent = [](Actor& actor) -> void* { return &actor.AddComponent<TComponent>(); };
			compFunc.RemoveComponent = [](Actor& actor) -> void { actor.RemoveComponent<TComponent>(); };
			compFunc.GetComponent = [](const Actor& actor) -> void* { return actor.TryGetComponent<TComponent>(); };
			compFunc.HasComponent = [](const Actor& actor) -> bool { return actor.HasComponent<TComponent>(); };
			compFunc.CloneComponent = [](const Actor& source, Actor& target) -> void
				{
					const TComponent* comp = source.TryGetComponent<TComponent>();
					if (comp != nullptr)
					{
						target.AddComponent<TComponent>(*comp);
					}
				};

			compFunc.GetActorsByComponent = [](const Scene& scene) { return scene.GetActorsByComponent<TComponent>(); };
			compFunc.GetSceneComponents = [](const Scene& scene)
				{
					Array<void*> result;
					for (auto* c : scene.GetComponents<TComponent>())
						result.Add(c);
					return result;
				};
		}

		template<typename TComponent>
		static void Unregister() { UnregisterDeferred(&Reflection::GetClass<TComponent>); }

		static const Array<const Reflection::Class*>& GetComponents();

		static ComponentFunctions& GetComponentFunctions(const Reflection::Class& componentClass);

		static void* InvokeAddComponent(const Reflection::Class& componentClass, Actor& actor);
		static void InvokeRemoveComponent(const Reflection::Class& componentClass, Actor& actor);
		static void* InvokeGetComponent(const Reflection::Class& componentClass, const Actor& actor, bool includeSubclasses = true);
		static bool InvokeHasComponent(const Reflection::Class& componentClass, const Actor& actor, bool includeSubclasses = true);
		static void InvokeCloneComponent(const Reflection::Class& componentClass, const Actor& source, Actor& target);
		static Array<Actor> InvokeGetActorsByComponent(const Reflection::Class& componentClass, const Scene& scene, bool includeSubclasses = true);
		static Array<void*> InvokeGetSceneComponents(const Reflection::Class& componentClass, const Scene& scene, bool includeSubclasses = true);

	private:
		static void RegisterDeferred(const Reflection::Class& (*cClass)());
		static void UnregisterDeferred(const Reflection::Class& (*cClass)());
		static void FlushDeferredRegistry();
		static size_t GetComponentIndex(const Reflection::Class& componentClass);

	private:
		// Used for deferred registering
		static Array<const Reflection::Class& (*)()> s_DeferredRegistry;

		static Array<const Reflection::Class*> s_ComponentClasses;
		static Array<Set<size_t>> s_ComponentSubclasses;
		static Array<ComponentFunctions> s_ComponentFunctions;

	};
}
