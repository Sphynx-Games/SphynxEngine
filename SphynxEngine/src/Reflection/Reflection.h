#pragma once

#include "Type.h"
#include "Class.h"
#include "Enum.h"
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

 // ---------- Class -----------
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
	}

#define SPX_REFLECT_CLASS(_Class) \
	SPX_REFLECT_CLASS_BEGIN(_Class) \
	SPX_REFLECT_CLASS_END()

 // ---------- Struct -----------
#define SPX_REFLECT_STRUCT_BEGIN(_Struct) \
	SPX_REFLECT_CLASS_BEGIN(_Struct) \
	self->IsStruct = true;

#define SPX_REFLECT_STRUCT_END(_Struct) \
	SPX_REFLECT_CLASS_END(_Struct)

#define SPX_REFLECT_STRUCT(_Struct) \
	SPX_REFLECT_STRUCT_BEGIN(_Struct) \
	SPX_REFLECT_STRUCT_END(_Struct)

 // ---------- Property -----------
#define SPX_REFLECT_PROPERTY_BEGIN(_Property) \
	{ \
		Properties.push_back(::Sphynx::Reflection::Property{ \
			::Sphynx::Reflection::GetType<decltype(((context_type*)0)->_Property)>(), \
			#_Property, \
			offsetof(context_type, _Property) \
		});

#define SPX_REFLECT_PROPERTY_END() \
	}

#define SPX_REFLECT_PROPERTY(_Property) \
	SPX_REFLECT_PROPERTY_BEGIN(_Property) \
	SPX_REFLECT_PROPERTY_END()

 // ---------- Enum -----------
#define SPX_REFLECT_ENUM_BEGIN(_Enum) \
	namespace { \
		inline static ::Sphynx::Reflection::details::EnumStorage<_Enum> _Enum##__EnumStorage([](auto* self) { \
			using context_enum = _Enum; \
			auto& Entries = self->Entries; \
			auto& Values = self->Values; \

#define SPX_REFLECT_ENUM_END(_Enum) \
		}); \
	} \
	\
	template<> \
	inline const ::Sphynx::Reflection::Enum& ::Sphynx::Reflection::details::GetEnumImpl<_Enum>() \
	{ \
		static const Enum e { \
			::Sphynx::Reflection::Type{ #_Enum, sizeof(_Enum), alignof(_Enum) }, \
			_Enum##__EnumStorage.Entries.data(), \
			_Enum##__EnumStorage.Entries.size() \
		}; \
		return e; \
	} \
	template<> struct ::Sphynx::Reflection::EnumRange<_Enum> { \
		struct Iterator \
		{ \
			using difference_type = std::ptrdiff_t; \
			using value_type = _Enum; \
			using pointer = _Enum*; \
			using reference = _Enum&; \
			\
			Iterator(int64_t* values) : m_Values(values) {} \
			\
			reference operator*() const { return reinterpret_cast<reference>(*m_Values); } \
			pointer operator->() { return reinterpret_cast<pointer>(m_Values); } \
			Iterator& operator++() { m_Values++; return *this; } \
			Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; } \
			friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_Values == b.m_Values; }; \
			friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_Values != b.m_Values; }; \
		\
		private: \
			int64_t* m_Values; \
		}; \
		inline Iterator begin() { return _Enum##__EnumStorage.Values.data(); } \
		inline Iterator begin() const { return _Enum##__EnumStorage.Values.data(); } \
		inline Iterator end() { return _Enum##__EnumStorage.Values.data() + _Enum##__EnumStorage.Values.size(); } \
		inline Iterator end() const { return _Enum##__EnumStorage.Values.data() + _Enum##__EnumStorage.Values.size(); } \
	};

#define SPX_REFLECT_ENUM_VALUE_BEGIN(_Value) \
	{ \
		Entries.push_back(::Sphynx::Reflection::Enum::Entry{ std::string(#_Value) ,(int64_t)context_enum::_Value }); \
		Values.push_back((int64_t)context_enum::_Value); \
		[[maybe_unused]] auto& Entry = *(Entries.end() - 1);

#define SPX_REFLECT_ENUM_VALUE_END() \
	}

#define SPX_REFLECT_ENUM_VALUE(_Value) \
	SPX_REFLECT_ENUM_VALUE_BEGIN(_Value) \
	SPX_REFLECT_ENUM_VALUE_END()

#define SPX_REFLECT_ENUM(_Enum) \
	SPX_REFLECT_ENUM_BEGIN(_Enum) \
	SPX_REFLECT_ENUM_END(_Enum)

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

			template<typename T>
			inline const Enum& GetEnumImpl();
		}

		template<typename Enum>
		struct EnumRange;

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

		template<typename T>
		inline const Enum& GetEnum()
		{
			return details::GetEnumImpl<T>();
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

enum class Test
{
	A,
	B,
	C
};

SPX_REFLECT_ENUM_BEGIN(Test)
	
	SPX_REFLECT_ENUM_VALUE(A)
	SPX_REFLECT_ENUM_VALUE(B)
	SPX_REFLECT_ENUM_VALUE(C)
	
SPX_REFLECT_ENUM_END(Test)

