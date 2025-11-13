#include "Reflection/Class.h"

namespace Sphynx
{
	namespace Reflection
	{
		const Property* Class::begin() const
		{
			return Properties;
		}

		const Property* Class::begin()
		{
			return Properties;
		}

		const Property* Class::end() const
		{
			return Properties + PropertiesCount;
		}

		const Property* Class::end()
		{
			return Properties + PropertiesCount;
		}

		bool Class::IsSubClassOf(const Class& baseClass) const
		{
			for (size_t i = 0; i < ParentClassesCount; ++i)
			{
				const Class* parent = ParentClasses[i];
				if (baseClass == *parent || parent->IsSubClassOf(baseClass))
				{
					return true;
				}
			}
			return false;
		}
	}
}
