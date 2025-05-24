#include "Reflection/Registry.h"


namespace Sphynx
{
	namespace Reflection
	{
		Registry* gRegistry = nullptr;

		static void EnsureRegistryInitialized()
		{
			if (gRegistry == nullptr)
			{
				static Registry sRegistry;
				gRegistry = &sRegistry;
			}
		}

		void Registry::Init()
		{
			EnsureRegistryInitialized();
			for (const auto& func : gRegistry->m_TypeFunctions)
			{
				[[maybe_unused]] const Type& cClass = func();
				//SPX_CORE_LOG_TRACE("    Reflection initialize {}", cClass.Name);
			}
		}

		void Registry::Shutdown()
		{
			// do nothing for now
		}

		void Registry::Register(const Type& (*typeFunc)())
		{
			EnsureRegistryInitialized();
			gRegistry->m_TypeFunctions.push_back(typeFunc);
		}

		void Registry::Unregister(const Type& (*typeFunc)())
		{
			EnsureRegistryInitialized();
			auto it = std::find(std::begin(gRegistry->m_TypeFunctions), std::end(gRegistry->m_TypeFunctions), typeFunc);
			SPX_CORE_ASSERT(it != std::end(gRegistry->m_TypeFunctions), "Unregistering non registered type!");
			gRegistry->m_TypeFunctions.erase(it);
		}

	}
}