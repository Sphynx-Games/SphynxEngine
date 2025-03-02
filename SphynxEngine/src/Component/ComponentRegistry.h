#pragma once

#include "Core/Core.h"

#include "Reflection/Class.h"
#include "Container/Array.h"


namespace Sphynx
{
	class ComponentRegistry
	{
	public:
		template<typename TComponent>
		static void Register()		{ Register(&Reflection::GetClass<TComponent>); }
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

	};
}
