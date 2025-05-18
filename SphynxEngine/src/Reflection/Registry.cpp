#include "Registry.h"


namespace Sphynx
{
	namespace Reflection
	{
		void Registry::Init()
		{
			for (const auto& func : GetTypeFunctions())
			{
				[[maybe_unused]] const Type& cClass = func();
				//SPX_CORE_LOG_TRACE("    Reflection initialize {}", cClass.Name);
			}
		}

		void Registry::Shutdown()
		{
			// do nothing for now
		}

		REFLECTION_API std::vector<const Type&(*)()>& Registry::GetTypeFunctions()
		{
			static std::vector<const Type&(*)()> s_TypeFunctions{};
			return s_TypeFunctions;
		}

	}
}