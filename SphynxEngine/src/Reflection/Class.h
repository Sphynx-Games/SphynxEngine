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

			const Property* begin();
			const Property* begin() const;
			const Property* end();
			const Property* end() const;

			template<typename T>
			bool HasAttribute() const { return GetAttribute<T>() != nullptr; }

			template<typename T>
			const T* GetAttribute() const
			{
				for (size_t i = 0; i < AttributesCount; ++i)
				{
					// TODO: change this ugly dynamic_cast
					if (const T* attr = dynamic_cast<const T*>(Attributes[i]))
						return attr;
				}

				return nullptr;
			}
		};

		struct SPHYNX_API TemplateClass : public Class
		{
			const TemplateArgument* TemplateArgs;
			size_t TemplateArgsCount;
		};
	}
}
