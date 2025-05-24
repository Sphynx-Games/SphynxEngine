#pragma once

#include "Core/Core.h"
#include "Type.h"
#include <vector>


namespace Sphynx
{
	namespace Reflection
	{
		class SPHYNX_API Registry
		{
		public:
			static void Init();
			static void Shutdown();

		private:
			template<typename T>
			static void Register();
			static void Register(const Type& (*typeFunc)());
			template<typename T>
			static void Unregister();
			static void Unregister(const Type& (*typeFunc)());

		private:
			std::vector<const Type& (*)()> m_TypeFunctions;

		private:
			template<typename T>
			friend struct TypeRegister;
		};

		extern SPHYNX_API Registry* gRegistry;

		template<typename T>
		void Registry::Register()
		{
			Register(&GetType<T>);
		}

		template<typename T>
		void Registry::Unregister()
		{
			Unregister(&GetType<T>);
		}

		template<typename T>
		struct TypeRegister
		{
			TypeRegister()
			{
				Registry::Register<T>();
			}

			~TypeRegister()
			{
				Registry::Unregister<T>();
			}
		};
	}
}
