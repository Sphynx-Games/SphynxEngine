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
	}
}
