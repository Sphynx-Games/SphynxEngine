#pragma once

#include "Reflection/Class.h"
#include <vector>

namespace Sphynx
{
	namespace Reflection
	{
		class Registry
		{
		public:
			static void Init()
			{
				for (const auto& func : s_TypeFunctions)
				{
					[[maybe_unused]] const Type& cClass = func();
					//SPX_CORE_LOG_TRACE("    Reflection initialize {}", cClass.Name);
				}
			}

			static void Shutdown()
			{
				// do nothing for now
			}

		private:
			template<typename T>
			static void InitializeDeferred()
			{
				s_TypeFunctions.push_back(&Reflection::GetType<T>);
			}

		private:
			inline static std::vector<const Type&(*)()> s_TypeFunctions;

		private:
			template<typename T>
			friend struct TypeRegister;
		};

		template<typename T>
		struct TypeRegister
		{
			TypeRegister()
			{
				Registry::InitializeDeferred<T>();
			}
		};
	}
}
