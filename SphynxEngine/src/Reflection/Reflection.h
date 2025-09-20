#pragma once

#include "Core/Core.h"
#include "Macros.h"
#include "Tag.h"
#include "StdTypes.h"

#include "Type.h"
#include "Class.h"
#include "Function.h"
#include "Enum.h"
#include "Property.h"
#include "Storage.h"
#include "Attribute.h"
#include "Registry.h"
#include "Traits/Traits.h"
#include "Core/Invoke.h"


/**
 * Usage:
 * SPX_REFLECT_CLASS_BEGIN(Texture, SPHYNX_API)
 *		SPX_REFLECT_PROPERTY_BEGIN(Width)
 *		SPX_REFLECT_PROPERTY_END()
 *		SPX_REFLECT_PROPERTY_BEGIN(Height)
 *		SPX_REFLECT_PROPERTY_END()
 * SPX_REFLECT_CLASS_END(Texture, SPHYNX_API)
 *
 * or a more simplified version
 *
 * SPX_REFLECT_CLASS_BEGIN(Texture, SPHYNX_API)
 *		SPX_REFLECT_PROPERTY(Width)
 *		SPX_REFLECT_PROPERTY(Height)
 * SPX_REFLECT_CLASS_END(Texture, SPHYNX_API)
 *
 */
 
namespace Sphynx
{
	namespace Reflection
	{
		namespace details
		{
			template<typename T>
			struct ClassImplWrapper;
		}

		template<typename Enum>
		struct EnumRange;

		template<typename T>
		inline const Type& GetType()
		{
			static_assert(!std::is_pointer<T>::value, "Cannot invoke GetType() with a pointer type");
			return details::GetTypeImpl(details::Tag<T>{});
		}

		template<typename T>
		inline const Class& GetClass()
		{
			static_assert(!std::is_pointer<T>::value, "Cannot invoke GetClass() with a pointer type");
			return details::GetClassImpl(details::Tag<T>{});
		}

		template<typename T>
		inline const Enum& GetEnum()
		{
			static_assert(!std::is_pointer<T>::value, "Cannot invoke GetEnum() with a pointer type");
			return details::GetEnumImpl(details::Tag<T>{});
		}

	}
}
