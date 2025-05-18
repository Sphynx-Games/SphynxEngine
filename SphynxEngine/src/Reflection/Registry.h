#pragma once

#include "Reflection/Class.h"
#include <vector>

#ifdef FORCE_IMPORT
#define REFLECTION_API __declspec(dllimport)
#else
#define REFLECTION_API __declspec(dllexport)
#endif // FORCE_IMPORT


namespace Sphynx
{
	namespace Reflection
	{
		class Registry
		{
		public:
			static void Init();
			static void Shutdown();

		private:
			template<typename T>
			static void InitializeDeferred();

		private:
			REFLECTION_API static std::vector<const Type&(*)()>& GetTypeFunctions();

		private:
			template<typename T>
			friend struct TypeRegister;
		};

		template<typename T>
		void Registry::InitializeDeferred()
		{
			GetTypeFunctions().push_back(&GetType<T>);
		}

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
