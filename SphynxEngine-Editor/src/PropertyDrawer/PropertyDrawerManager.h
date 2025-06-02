#pragma once

#include <vector>
#include <Reflection/Type.h>


namespace Sphynx
{
	class IPropertyDrawer;

	class PropertyDrawerManager
	{
	public:
		static void Init();
		static void Shutdown();

		static void RegisterDrawer(IPropertyDrawer* drawer);
		static void UnregisterDrawer(IPropertyDrawer* drawer);

		static IPropertyDrawer* GetDrawer(const Reflection::Type& type);

	private:
		static std::vector<IPropertyDrawer*> s_PropertyDrawers;

	};
}
