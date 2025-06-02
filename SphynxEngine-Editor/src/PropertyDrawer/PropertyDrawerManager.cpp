#include "PropertyDrawerManager.h"

#include "PropertyDrawer.h"
#include <algorithm>


namespace Sphynx
{
	std::vector<IPropertyDrawer*> PropertyDrawerManager::s_PropertyDrawers{};

	void PropertyDrawerManager::Init()
	{
		s_PropertyDrawers = {};
		for (IPropertyDrawer* drawer : *IPropertyDrawer::s_Registry)
		{
			RegisterDrawer(drawer);
		}
	}

	void PropertyDrawerManager::Shutdown()
	{
		for (IPropertyDrawer* drawer : *IPropertyDrawer::s_Registry)
		{
			UnregisterDrawer(drawer);
		}
	}

	void PropertyDrawerManager::RegisterDrawer(IPropertyDrawer* drawer)
	{
		SPX_CORE_ASSERT(std::find(s_PropertyDrawers.begin(), s_PropertyDrawers.end(), drawer) == s_PropertyDrawers.end(), "PropertyDrawer is already in registry");
		s_PropertyDrawers.push_back(drawer);
	}

	void PropertyDrawerManager::UnregisterDrawer(IPropertyDrawer* drawer)
	{
		auto it = std::find(s_PropertyDrawers.begin(), s_PropertyDrawers.end(), drawer);
		SPX_CORE_ASSERT(it != s_PropertyDrawers.end(), "PropertyDrawer not found in registry");
		s_PropertyDrawers.erase(it);
	}

	IPropertyDrawer* PropertyDrawerManager::GetDrawer(const Reflection::Type& type)
	{
		auto it = std::find_if(s_PropertyDrawers.begin(), s_PropertyDrawers.end(), [&](const IPropertyDrawer* drawer) { return &(drawer->GetType()) == &type; });
		if (it == s_PropertyDrawers.end())
		{
			return nullptr;
		}

		return *it;
	}

}
