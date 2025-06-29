#include "spxpch.h"
#include "ModuleManager.h"

#ifdef SPX_PLATFORM_WINDOWS

#include <Windows.h>
#include <tchar.h>

namespace Sphynx
{
	ModuleHandle ModuleManager::LoadModule(const std::filesystem::path& path)
	{
		HMODULE module = NULL;
		module = LoadLibrary(_T(path.string().c_str()));
		if (module == NULL || module == INVALID_HANDLE_VALUE)
		{
			SPX_CORE_LOG_ERROR("Module '{}' not found!!", path.string());
			return ModuleHandle::Invalid;
		}

		ModuleHandle handle = ModuleHandle::Generate();
		s_LoadedModules.Add(handle, module);

		return handle;
	}

	void ModuleManager::UnloadModule(const ModuleHandle& handle)
	{
		if (handle == ModuleHandle::Invalid)
		{
			SPX_CORE_LOG_WARNING("The ModuleHandle is invalid!!");
			return;
		}

		if (!s_LoadedModules.ContainsKey(handle))
		{
			SPX_CORE_LOG_WARNING("The provided handle is not associated to any loaded module!!");
			return;
		}

		HMODULE module = static_cast<HMODULE>(s_LoadedModules.GetValue(handle));
		if (module == NULL)
		{
			SPX_CORE_LOG_WARNING("The HMODULE is NULL!!");
			return;
		}

		FreeLibrary(module);
		s_LoadedModules.Remove(handle);
	}

	void ModuleManager::UnloadAllModules()
	{
		for (auto [handle, module] : s_LoadedModules)
		{
			if (module != nullptr)
			{
				FreeLibrary(static_cast<HMODULE>(module));
			}
		}
		s_LoadedModules.RemoveAll();
	}

	void* ModuleManager::GetModule(const ModuleHandle& handle)
	{
		SPX_CORE_ASSERT(s_LoadedModules.ContainsKey(handle), "The provided handle is not associated to any loaded module!!");
		return s_LoadedModules.GetValue(handle);
	}

	const HashMap<ModuleHandle, void*>& ModuleManager::GetAllModules()
	{
		return s_LoadedModules;
	}
}

#endif