#pragma once

#include "Core/Core.h"
#include "Core/UUID.h"
#include "Traits/Utility.h"
#include <filesystem>
#include "Container/Map.h"


namespace Sphynx
{
	using ModuleHandle = Traits::unique_type<UUID, struct __ModuleHandle__>;

	class SPHYNX_API ModuleManager
	{
	public:
		static ModuleHandle LoadModule(const std::filesystem::path& path);
		static void UnloadModule(const ModuleHandle& handle);
		static void* GetModule(const ModuleHandle& handle);
		static void UnloadAllModules();

	private:
		inline static HashMap<ModuleHandle, void*> s_LoadedModules = {};
	};
}