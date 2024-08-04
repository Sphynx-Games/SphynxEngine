#pragma once
#include "Property.h"
#include <vector>

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
					Size{}
				{
					ctor(this);
				}

				std::vector<Property> Properties;
				size_t Size;
			};
		}
	}
}
