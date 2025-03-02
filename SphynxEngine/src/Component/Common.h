#pragma once
#include "Reflection/Reflection.h"
#include "Reflection/Attribute.h"

#include "ComponentRegistry.h"


#define COMPONENT_COMMON_BODY(Component) \
public:\
	Component() = default; \
	Component(const Component& other) = default; \
	Component(Component&& other) = default; \
	Component& operator=(Component&& other) = default

#define COMPONENT_COMMON_BODY_NO_DEFAULT_CONSTRUCTOR(Component) \
public:\
	Component(const Component& other) = default; \
	Component(Component&& other) = default; \
	Component& operator=(Component&& other) = default


namespace Sphynx
{
	namespace Reflection
	{
		namespace CommonAttribute
		{
			class InternalComponent : public ::Sphynx::Reflection::Attribute
			{
			public:
				template<typename TComponent>
				InternalComponent(::Sphynx::Reflection::details::Tag<TComponent>)
				{
					ComponentRegistry::Register<TComponent>();
					deleter = &ComponentRegistry::Unregister<TComponent>;
				}

				~InternalComponent()
				{
					deleter();
					deleter = nullptr;
				}

			private:
				void(*deleter)();
			};
		}
	}
}
#define InternalComponent ::Sphynx::Reflection::CommonAttribute::InternalComponent, ::Sphynx::Reflection::details::Tag<context_type>{}
