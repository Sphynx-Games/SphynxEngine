#pragma once
#include "Property.h"
#include "Function.h"
#include "Attribute.h"
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
					Functions{},
					Size{0},
					IsStruct{false},
					Attributes{}
				{
					Instance = this;
					ctor(this);
				}

				inline static ClassStorage* Instance = nullptr;

				std::vector<Property> Properties;
				std::vector<Function> Functions;
				size_t Size;
				bool IsStruct;
				std::vector<Attribute*> Attributes;
			};

			template<typename E>
			struct EnumStorage
			{
				template<typename Lambda>
				EnumStorage(Lambda&& ctor) :
					Entries{},
					Values{},
					Attributes{}
				{
					Instance = this;
					ctor(this);
				}

				inline static EnumStorage* Instance = nullptr;

				std::vector<Enum::Entry> Entries;
				std::vector<int64_t> Values;
				std::vector<Attribute*> Attributes;
			};
		}
	}
}
