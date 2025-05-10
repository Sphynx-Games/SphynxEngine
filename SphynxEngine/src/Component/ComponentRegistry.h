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
		using ComponentFunction = void (*)(Actor&);
		using ComponentFunctionBool = bool (*)(Actor&);

		ComponentFunction AddComponent;
		ComponentFunction RemoveComponent;
		ComponentFunctionBool HasComponent;
	};

	class ComponentRegistry
	{
	public:
		template<typename TComponent>
		static void Register()
		{ 
			Register(&Reflection::GetClass<TComponent>);
			ComponentFunctions& compFunc = s_ComponentFunctions.Emplace();
			compFunc.AddComponent = [](Actor& actor) -> void { actor.AddComponent<TComponent>(); };
			compFunc.RemoveComponent = [](Actor& actor) -> void { actor.RemoveComponent<TComponent>(); };
			compFunc.HasComponent = [](Actor& actor) -> bool { return actor.HasComponent<TComponent>(); };
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

		static ComponentFunctions& GetComponentFunctions(const Reflection::Class& reflectionComponentClass)
		{
			auto lambda = [&](auto reflectClass) { return &reflectClass() == &reflectionComponentClass; };
			auto it = std::find_if(s_ComponentClasses.begin(), s_ComponentClasses.end(), lambda);
			auto index = std::distance(s_ComponentClasses.begin(), it);
			return s_ComponentFunctions.Get(index);
		}

		static void InvokeAddComponentFunction(const Reflection::Class& reflectionComponentClass, Actor& actor)
		{
			ComponentFunctions& compFunc = GetComponentFunctions(reflectionComponentClass);
			compFunc.AddComponent(actor);
		}

		static void InvokeRemoveComponentFunction(const Reflection::Class& reflectionComponentClass, Actor& actor)
		{
			ComponentFunctions& compFunc = GetComponentFunctions(reflectionComponentClass);
			compFunc.RemoveComponent(actor);
		}

		static bool InvokeHasComponentFunction(const Reflection::Class& reflectionComponentClass, Actor& actor)
		{
			ComponentFunctions& compFunc = GetComponentFunctions(reflectionComponentClass);
			return compFunc.HasComponent(actor);
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
