#pragma once

#include "Type.h"
#include "Class.h"
#include "Property.h"
#include "Storage.h"


/**
 * Usage:
 * SPX_REFLECT_CLASS_BEGIN(Texture)
 *		SPX_REFLECT_PROPERTY_BEGIN(Width)
 *		SPX_REFLECT_PROPERTY_END()
 *		SPX_REFLECT_PROPERTY_BEGIN(Height)
 *		SPX_REFLECT_PROPERTY_END()
 * SPX_REGLECT_CLASS_END(Texture)
 *
 * or a more simplified version
 *
 * SPX_REFLECT_CLASS_BEGIN(Texture)
 *		SPX_REFLECT_PROPERTY(Width)
 *		SPX_REFLECT_PROPERTY(Height)
 * SPX_REGLECT_CLASS_END(Texture)
 *
 */

#define SPX_REFLECT_CLASS_BEGIN(_Class) \
	namespace { \
		inline static ::Sphynx::Reflection::details::ClassStorage<_Class> _Class##__ClassStorage([](auto* self) { \
			self->Size = sizeof(_Class); \
			auto& Properties = self->Properties; \
			using context_type = _Class;

#define SPX_REFLECT_CLASS_END(_Class) \
		}); \
	} \
	\
	template<> \
	inline const ::Sphynx::Reflection::Class& ::Sphynx::Reflection::details::GetClassImpl<_Class>() \
	{ \
		static const Class c { \
			::Sphynx::Reflection::Type{ #_Class, sizeof(_Class), alignof(_Class) }, \
			_Class##__ClassStorage.Properties.data(), \
			_Class##__ClassStorage.Properties.size() \
		}; \
		return c; \
	} \
	\
	template<> \
	inline const ::Sphynx::Reflection::Type& ::Sphynx::Reflection::details::GetTypeImpl<_Class>() \
	{ \
		return GetClassImpl<_Class>(); \
	} \


#define SPX_REFLECT_CLASS(_Class) \
	SPX_REFLECT_CLASS_BEGIN(_Class) \
	SPX_REFLECT_CLASS_END()

#define SPX_REFLECT_PROPERTY_BEGIN(_Property) \
	{ \
		Properties.push_back(::Sphynx::Reflection::Property{ \
			::Sphynx::Reflection::GetType<decltype(((context_type*)0)->_Property)>(), \
			#_Property, \
			offsetof(context_type, _Property) \
		}); \

#define SPX_REFLECT_PROPERTY_END() \
	}

#define SPX_REFLECT_PROPERTY(_Property) \
	SPX_REFLECT_PROPERTY_BEGIN(_Property) \
	SPX_REFLECT_PROPERTY_END()

namespace Sphynx
{
	namespace Reflection
	{
		namespace details
		{
			template<typename T>
			inline const Type& GetTypeImpl();

			template<typename T>
			inline const Class& GetClassImpl();
		}

		template<typename T>
		inline const Type& GetType()
		{
			return details::GetTypeImpl<T>();
		}

		template<typename T>
		inline const Class& GetClass()
		{
			return details::GetClassImpl<T>();
		}

#define X(_Type) \
	template<> inline const Type& details::GetTypeImpl<_Type>() \
	{ \
		static const Type type{ #_Type, sizeof(_Type), alignof(_Type) }; \
		return type; \
	}

#define TYPES() \
	X(bool)					\
    X(char)					\
    X(short)				\
    X(int)					\
    X(long)					\
    X(long long)			\
    X(float)				\
    X(double)				\
    X(long double)			\
    X(unsigned char)		\
    X(unsigned short)		\
    X(unsigned int)			\
    X(unsigned long)		\
    X(unsigned long long)	\


		TYPES()
		
		template<> inline const Type& details::GetTypeImpl<void>()
		{
			static const Type type{ "void", 0, 0 };
			return type;
		}

		template<> inline const Type& details::GetTypeImpl<std::string>()
		{
			static const Type type{ "std::string", sizeof(std::string), alignof(std::string) };
			return type;
		}

	}
}

// DO NOT UNCOMMENT, THIS IS FOR TESTING PURPOSES
//struct Vector
//{
//	int a;
//	int b;
//	int c;
//};
//
//SPX_REFLECT_CLASS_BEGIN(Vector)
//	
//	// Properties
//	SPX_REFLECT_PROPERTY(a)
//	SPX_REFLECT_PROPERTY(b)
//	SPX_REFLECT_PROPERTY(c)
//
//	// TODO: Functions
//	
//SPX_REFLECT_CLASS_END(Vector)

