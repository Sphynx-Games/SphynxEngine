#pragma once
#include "Property.h"
#include <vector>
#include <string>


namespace Sphynx
{
	namespace Reflection
	{
		namespace details
		{
			template<typename T>
			struct ClassStorage
			{
				template<typename Lambda>
				ClassStorage(Lambda&& ctor) :
					Properties{},
					Size{0},
					IsStruct{false}
				{
					ctor(this);
				}

				std::vector<Property> Properties;
				size_t Size;
				bool IsStruct;
			};

			template<typename E>
			struct EnumStorage
			{
				template<typename Lambda>
				EnumStorage(Lambda&& ctor) :
					Entries{}
				{
					ctor(this);
				}

				std::vector<Enum::Entry> Entries;
				std::vector<int64_t> Values;
			};
		}
	}
}
