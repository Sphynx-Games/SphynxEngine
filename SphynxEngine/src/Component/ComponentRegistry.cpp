#include "ComponentRegistry.h"


namespace Sphynx
{
	Array<const Reflection::Class& (*)()> ComponentRegistry::s_DeferredRegistry{};
	Array<const Reflection::Class*> ComponentRegistry::s_ComponentClasses{};
	Array<Set<size_t>> ComponentRegistry::s_ComponentSubclasses{};
	Array<ComponentFunctions> ComponentRegistry::s_ComponentFunctions{};

	const Array<const Reflection::Class*>& ComponentRegistry::GetComponents()
	{
		FlushDeferredRegistry();
		return s_ComponentClasses;
	}

	ComponentFunctions& ComponentRegistry::GetComponentFunctions(const Reflection::Class& componentClass)
	{
		FlushDeferredRegistry();

		size_t index = GetComponentIndex(componentClass);
		return s_ComponentFunctions.Get(index);
	}

	void* ComponentRegistry::InvokeAddComponent(const Reflection::Class& componentClass, Actor& actor)
	{
		FlushDeferredRegistry();

		ComponentFunctions& compFunc = GetComponentFunctions(componentClass);
		return compFunc.AddComponent(actor);
	}

	void ComponentRegistry::InvokeRemoveComponent(const Reflection::Class& componentClass, Actor& actor)
	{
		FlushDeferredRegistry();

		ComponentFunctions& compFunc = GetComponentFunctions(componentClass);
		compFunc.RemoveComponent(actor);
	}

	void* ComponentRegistry::InvokeGetComponent(const Reflection::Class& componentClass, const Actor& actor, bool includeSubclasses)
	{
		FlushDeferredRegistry();

		size_t compIndex = GetComponentIndex(componentClass);
		const ComponentFunctions& compFunc = s_ComponentFunctions.Get(compIndex);
		void* result = compFunc.GetComponent(actor);
		if (result != nullptr || !includeSubclasses) return result;

		for (size_t subcompIndex : s_ComponentSubclasses[compIndex])
		{
			const ComponentFunctions& compFunc = s_ComponentFunctions.Get(subcompIndex);
			void* result = compFunc.GetComponent(actor);
			if (result != nullptr) return result;
		}

		return result;
	}

	bool ComponentRegistry::InvokeHasComponent(const Reflection::Class& componentClass, const Actor& actor, bool includeSubclasses)
	{
		FlushDeferredRegistry();
		size_t compIndex = GetComponentIndex(componentClass);
		const ComponentFunctions& compFunc = s_ComponentFunctions.Get(compIndex);
		const bool result = compFunc.HasComponent(actor);
		if (result || !includeSubclasses) return result;

		for (size_t subcompIndex : s_ComponentSubclasses[compIndex])
		{
			const ComponentFunctions& compFunc = s_ComponentFunctions.Get(subcompIndex);
			const bool result = compFunc.HasComponent(actor);
			if (result) return result;
		}

		return result;
	}

	void ComponentRegistry::InvokeCloneComponent(const Reflection::Class& componentClass, const Actor& source, Actor& target)
	{
		FlushDeferredRegistry();
		ComponentFunctions& compFunc = GetComponentFunctions(componentClass);
		compFunc.CloneComponent(source, target);
	}

	Array<Actor> ComponentRegistry::InvokeGetActorsByComponent(const Reflection::Class& componentClass, const Scene& scene, bool includeSubclasses)
	{
		FlushDeferredRegistry();
		size_t compIndex = GetComponentIndex(componentClass);
		const ComponentFunctions& compFunc = s_ComponentFunctions.Get(compIndex);
		Array<Actor> result = compFunc.GetActorsByComponent(scene);
		if (!includeSubclasses) return result;

		for (size_t subcompIndex : s_ComponentSubclasses[compIndex])
		{
			const ComponentFunctions& compFunc = s_ComponentFunctions.Get(subcompIndex);
			const Array<Actor> temp = compFunc.GetActorsByComponent(scene);
			result.Append(temp);
		}

		return result;
	}

	Array<void*> ComponentRegistry::InvokeGetSceneComponents(const Reflection::Class& componentClass, const Scene& scene, bool includeSubclasses)
	{
		FlushDeferredRegistry();
		size_t compIndex = GetComponentIndex(componentClass);
		const ComponentFunctions& compFunc = s_ComponentFunctions.Get(compIndex);
		Array<void*> result = compFunc.GetSceneComponents(scene);
		if (!includeSubclasses) return result;

		for (size_t subcompIndex : s_ComponentSubclasses[compIndex])
		{
			const ComponentFunctions& compFunc = s_ComponentFunctions.Get(subcompIndex);
			const Array<void*> temp = compFunc.GetSceneComponents(scene);
			result.Append(temp);
		}

		return result;
	}

	void ComponentRegistry::RegisterDeferred(const Reflection::Class& (*cClass)())
	{
		SPX_CORE_ASSERT(!s_DeferredRegistry.Contains(cClass), "Component already registered!");
		//SPX_CORE_LOG_TRACE("Registering component");
		s_DeferredRegistry.Add(cClass);
	}

	void ComponentRegistry::UnregisterDeferred(const Reflection::Class& (*cClass)())
	{
		// This won't really deferred anything, it is immediate xD
		const Reflection::Class& result = cClass();
		// FIXME: component classes might be uninitialized before component unregistration
		SPX_CORE_ASSERT(s_ComponentClasses.Contains(&result), "Component not registered!");
		//SPX_CORE_LOG_TRACE("Unregistering component");
		
		size_t index = GetComponentIndex(result);
		std::for_each(std::begin(s_ComponentSubclasses), std::end(s_ComponentSubclasses), [index](Set<size_t>& set)
			{
				// at this point, classes can be releasing if unloaded or shutdown
				// that is why we go thought all sets and remove "index" if possible
				set.Remove(index);
			});

		s_ComponentClasses.RemoveAt(index);
		s_ComponentFunctions.RemoveAt(index);
		s_ComponentSubclasses.RemoveAt(index);
	}

	void ComponentRegistry::FlushDeferredRegistry()
	{
		for (const auto& func : s_DeferredRegistry)
		{
			const Reflection::Class& cClass = func();
			s_ComponentClasses.Add(&cClass);
			s_ComponentSubclasses.Emplace();
			
			size_t index = s_ComponentClasses.Size() - 1;
			std::for_each_n(cClass.ParentClasses, cClass.ParentClassesCount, [index](const Reflection::Class* parentClass)
				{
					size_t parentIndex = GetComponentIndex(*parentClass);
					s_ComponentSubclasses[parentIndex].Add(index);
				});
		}
		s_DeferredRegistry.RemoveAll();
	}

	size_t ComponentRegistry::GetComponentIndex(const Reflection::Class& componentClass)
	{
		auto lambda = [&](auto reflectClass) { return reflectClass == &componentClass; };
		auto it = std::find_if(s_ComponentClasses.begin(), s_ComponentClasses.end(), lambda);
		auto index = std::distance(s_ComponentClasses.begin(), it);
		return static_cast<size_t>(index);
	}
}
