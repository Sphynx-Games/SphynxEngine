#pragma once

#include "Core/Core.h"

#include "Reflection/Class.h"
#include "Container/Array.h"

#include "Scene/Scene.h"
#include "Scene/Actor.h"


namespace Sphynx
{
	struct ComponentFunctions
	{
		// Actor functions
		void* (*AddComponent)(Actor&);
		void (*RemoveComponent)(Actor&);
		void* (*GetComponent)(const Actor&);
		bool (*HasComponent)(const Actor&);
		void(*CloneComponent)(const Actor&, Actor&); // const Actor& source, Actor& target

		// Scene functions
		Array<Actor>(*GetActorsByComponent)(const Scene&);
	};

	class ComponentRegistry
	{
	public:
		template<typename TComponent>
		static void Register()
		{ 
			Register(&Reflection::GetClass<TComponent>);
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
		}

		template<typename TComponent>
		static void Unregister()	{ Unregister(&Reflection::GetClass<TComponent>); }

		static Array<const Reflection::Class*> GetComponents() 
		{
			Array<const Reflection::Class*> components;
			for (const auto& func : s_ComponentClasses)
			{
				const Reflection::Class& cClass = func();
				components.Add(&cClass);
			}

			return components; 
		}

		static ComponentFunctions& GetComponentFunctions(const Reflection::Class& componentClass)
		{
			auto lambda = [&](auto reflectClass) { return &reflectClass() == &componentClass; };
			auto it = std::find_if(s_ComponentClasses.begin(), s_ComponentClasses.end(), lambda);
			auto index = std::distance(s_ComponentClasses.begin(), it);
			return s_ComponentFunctions.Get(index);
		}

		static void* InvokeAddComponent(const Reflection::Class& componentClass, Actor& actor)
		{
			ComponentFunctions& compFunc = GetComponentFunctions(componentClass);
			return compFunc.AddComponent(actor);
		}

		static void InvokeRemoveComponent(const Reflection::Class& componentClass, Actor& actor)
		{
			ComponentFunctions& compFunc = GetComponentFunctions(componentClass);
			compFunc.RemoveComponent(actor);
		}

		static void* InvokeGetComponent(const Reflection::Class& componentClass, const Actor& actor)
		{
			ComponentFunctions& compFunc = GetComponentFunctions(componentClass);
			return compFunc.GetComponent(actor);
		}

		static bool InvokeHasComponent(const Reflection::Class& componentClass, const Actor& actor)
		{
			ComponentFunctions& compFunc = GetComponentFunctions(componentClass);
			return compFunc.HasComponent(actor);
		}

		static void InvokeCloneComponent(const Reflection::Class& componentClass, const Actor& source, Actor& target)
		{
			ComponentFunctions& compFunc = GetComponentFunctions(componentClass);
			compFunc.CloneComponent(source, target);
		}

		static Array<Actor> InvokeGetActorsByComponent(const Reflection::Class& componentClass, const Scene& scene)
		{
			ComponentFunctions& compFunc = GetComponentFunctions(componentClass);
			return compFunc.GetActorsByComponent(scene);
		}

	private:
		static void Register(const Reflection::Class& (*cClass)())
		{
			SPX_CORE_ASSERT(!s_ComponentClasses.Contains(cClass), "Component already registered!");
			//SPX_CORE_LOG_TRACE("Registering component");
			s_ComponentClasses.Add(cClass);
		}

		static void Unregister(const Reflection::Class& (*cClass)())
		{
			SPX_CORE_ASSERT(s_ComponentClasses.Contains(cClass), "Component not registered!");
			//SPX_CORE_LOG_TRACE("Unregistering component");
			s_ComponentClasses.Remove(cClass);
		}

	private:
		inline static Array<const Reflection::Class& (*)()> s_ComponentClasses;
		inline static Array<ComponentFunctions> s_ComponentFunctions;

	};
}
