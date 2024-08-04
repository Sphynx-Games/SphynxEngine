#pragma once

#include "Core/Core.h"
#include "Type.h"
#include "Property.h"


namespace Sphynx
{
	namespace Reflection
	{
		struct Property;

		struct SPHYNX_API Class : public Type
		{
			const Property* Properties;
			size_t PropertiesCount;

			inline const Property* begin() { return Properties; }
			inline const Property* begin() const { return Properties; }
			inline const Property* end() { return Properties + PropertiesCount; }
			inline const Property* end() const { return Properties + PropertiesCount; }
		};
	}
}
