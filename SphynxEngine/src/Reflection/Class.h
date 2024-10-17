#pragma once

#include "Core/Core.h"
#include "Type.h"
#include "Property.h"
#include "TemplateArgument.h"


namespace Sphynx
{
	namespace Reflection
	{
		struct Property;
		struct Function;
		struct TemplateArgument;
		class Attribute;

		struct SPHYNX_API Class : public Type
		{
			const Property* Properties;
			size_t PropertiesCount;

			const Function* Functions;
			size_t FunctionsCount;

			const Attribute* const* Attributes;
			size_t AttributesCount;

			inline const Property* begin() { return Properties; }
			inline const Property* begin() const { return Properties; }
			inline const Property* end() { return Properties + PropertiesCount; }
			inline const Property* end() const { return Properties + PropertiesCount; }
		};

		struct SPHYNX_API TemplateClass : public Class
		{
			const TemplateArgument* TemplateArgs;
			size_t TemplateArgsCount;
		};
	}
}
