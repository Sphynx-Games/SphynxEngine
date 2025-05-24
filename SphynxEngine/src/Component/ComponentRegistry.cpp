#include "ComponentRegistry.h"


namespace Sphynx
{
	Array<const Reflection::Class& (*)()> ComponentRegistry::s_ComponentClasses{};
	Array<ComponentFunctions> ComponentRegistry::s_ComponentFunctions{};

	void ComponentRegistry::Register(const Reflection::Class& (*cClass)())
	{
		SPX_CORE_ASSERT(!s_ComponentClasses.Contains(cClass), "Component already registered!");
		//SPX_CORE_LOG_TRACE("Registering component");
		s_ComponentClasses.Add(cClass);
	}

	void ComponentRegistry::Unregister(const Reflection::Class& (*cClass)())
	{
		SPX_CORE_ASSERT(s_ComponentClasses.Contains(cClass), "Component not registered!");
		//SPX_CORE_LOG_TRACE("Unregistering component");
		s_ComponentClasses.Remove(cClass);
	}

	Array<const Reflection::Class*> ComponentRegistry::GetComponents()
	{
		Array<const Reflection::Class*> components;
		for (const auto& func : s_ComponentClasses)
		{
			const Reflection::Class& cClass = func();
			components.Add(&cClass);
		}

		return components;
	}

	ComponentFunctions& ComponentRegistry::GetComponentFunctions(const Reflection::Class& componentClass)
	{
		auto lambda = [&](auto reflectClass) { return &reflectClass() == &componentClass; };
		auto it = std::find_if(s_ComponentClasses.begin(), s_ComponentClasses.end(), lambda);
		auto index = std::distance(s_ComponentClasses.begin(), it);
		return s_ComponentFunctions.Get(index);
	}

	void* ComponentRegistry::InvokeAddComponent(const Reflection::Class& componentClass, Actor& actor)
	{
		ComponentFunctions& compFunc = GetComponentFunctions(componentClass);
		return compFunc.AddComponent(actor);
	}

	void ComponentRegistry::InvokeRemoveComponent(const Reflection::Class& componentClass, Actor& actor)
	{
		ComponentFunctions& compFunc = GetComponentFunctions(componentClass);
		compFunc.RemoveComponent(actor);
	}

	void* ComponentRegistry::InvokeGetComponent(const Reflection::Class& componentClass, const Actor& actor)
	{
		ComponentFunctions& compFunc = GetComponentFunctions(componentClass);
		return compFunc.GetComponent(actor);
	}

	bool ComponentRegistry::InvokeHasComponent(const Reflection::Class& componentClass, const Actor& actor)
	{
		ComponentFunctions& compFunc = GetComponentFunctions(componentClass);
		return compFunc.HasComponent(actor);
	}

	void ComponentRegistry::InvokeCloneComponent(const Reflection::Class& componentClass, const Actor& source, Actor& target)
	{
		ComponentFunctions& compFunc = GetComponentFunctions(componentClass);
		compFunc.CloneComponent(source, target);
	}

	Array<Actor> ComponentRegistry::InvokeGetActorsByComponent(const Reflection::Class& componentClass, const Scene& scene)
	{
		ComponentFunctions& compFunc = GetComponentFunctions(componentClass);
		return compFunc.GetActorsByComponent(scene);
	}
}
