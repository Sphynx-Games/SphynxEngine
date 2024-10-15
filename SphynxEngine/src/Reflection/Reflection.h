#pragma once

#include "Type.h"
#include "Class.h"
#include "Function.h"
#include "Enum.h"
#include "Property.h"
#include "Storage.h"
#include "Attribute.h"
#include "Traits/Traits.h"
#include "Core/Invoke.h"


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
 * SPX_REFLECT_CLASS_END(Texture)
 *
 */
#define EXPAND_MACRO(x) x

 // ---------- Class -----------
#define SPX_REFLECT_CLASS_BEGIN(_Class) \
	namespace { \
		inline static ::Sphynx::Reflection::details::ClassStorage<_Class> EXPAND_MACRO(s_ClassStorage)EXPAND_MACRO(__LINE__)([](auto* self) { \
			self->Size = sizeof(_Class); \
			[[maybe_unused]] auto& Properties = self->Properties; \
			[[maybe_unused]] auto& Functions = self->Functions; \
			[[maybe_unused]] auto& Attributes = self->Attributes; \
			using context_type = _Class;

#define SPX_REFLECT_CLASS_END(_Class) \
		}); \
	} \
	\
	template<> \
	inline const ::Sphynx::Reflection::Class& ::Sphynx::Reflection::details::GetClassImpl<_Class>() \
	{ \
		static const auto* Storage = ::Sphynx::Reflection::details::ClassStorage<_Class>::Instance; \
		static const Class c { \
			::Sphynx::Reflection::Type{ #_Class, sizeof(_Class), alignof(_Class), false }, \
			Storage->Properties.data(), \
			Storage->Properties.size(), \
			Storage->Functions.data(), \
			Storage->Functions.size(), \
			Storage->Attributes.data(), \
			Storage->Attributes.size() \
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
	SPX_REFLECT_CLASS_END(_Class)

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
		}); \
		[[maybe_unused]] auto& Attributes = (--Properties.end())->Attributes;

#define SPX_REFLECT_PROPERTY_END() \
	}

#define SPX_REFLECT_PROPERTY(_Property) \
	SPX_REFLECT_PROPERTY_BEGIN(_Property) \
	SPX_REFLECT_PROPERTY_END()

 // ---------- Member Function -----------
#define SPX_REFLECT_FUNCTION_BEGIN(_Function) \
	{ \
		auto FuncPtr = [](void* obj, void* params) \
		{ \
			::Sphynx::Invoke( \
				*reinterpret_cast<typename ::Sphynx::Traits::function_scope_type<decltype(&context_type::_Function)>::type*>(obj), \
				&context_type::_Function, \
				params); \
		}; \
		using param_pack = typename ::Sphynx::Traits::function_args_type<decltype(&context_type::_Function)>::type; \
		using return_type = typename ::Sphynx::Traits::function_return_type<decltype(&context_type::_Function)>::type; \
		static std::array<::Sphynx::Reflection::Function::Parameter, ::Sphynx::Traits::args_pack_size<param_pack>::value> Parameters = \
			::Sphynx::Reflection::details::GetParameterPackArray<param_pack>( \
				std::make_index_sequence<::Sphynx::Traits::args_pack_size<param_pack>::value>{}); \
	 \
		Functions.push_back(::Sphynx::Reflection::Function{ #_Function, ::Sphynx::Reflection::GetType<return_type>(), Parameters.data(), ::Sphynx::Traits::args_pack_size<param_pack>::value, std::move(FuncPtr) }); \
		[[maybe_unused]] auto& Attributes = (--Functions.end())->Attributes; \
		[[maybe_unused]] ::Sphynx::Reflection::Function::Parameter* ParamPtr = Parameters.data(); \

#define SPX_REFLECT_FUNCTION_END() \
	}

#define SPX_REFLECT_FUNCTION(_Function) \
	SPX_REFLECT_FUNCTION_BEGIN(_Function) \
	SPX_REFLECT_FUNCTION_END()

#define SPX_REFLECT_FUNCTION_PARAM_BEGIN(_Param) \
	{ \
		[[maybe_unused]] auto& Attributes = ParamPtr->Attributes; \
		ParamPtr->Name = #_Param; \

#define SPX_REFLECT_FUNCTION_PARAM_END() \
		++ParamPtr; \
	}

#define SPX_REFLECT_FUNCTION_PARAM(_Param) \
	SPX_REFLECT_FUNCTION_PARAM_BEGIN(_Param) \
	SPX_REFLECT_FUNCTION_PARAM_END()

 // ---------- Enum -----------
#define SPX_REFLECT_ENUM_BEGIN(_Enum) \
	namespace { \
		inline static ::Sphynx::Reflection::details::EnumStorage<_Enum> EXPAND_MACRO(s_EnumStorage)EXPAND_MACRO(__LINE__)([](auto* self) { \
			using context_enum = _Enum; \
			[[maybe_unused]] auto& Entries = self->Entries; \
			[[maybe_unused]] auto& Values = self->Values; \
			[[maybe_unused]] auto& Attributes = self->Attributes; \


#define SPX_REFLECT_ENUM_END(_Enum) \
		}); \
	} \
	\
	template<> \
	inline const ::Sphynx::Reflection::Enum& ::Sphynx::Reflection::details::GetEnumImpl<_Enum>() \
	{ \
		static const auto* Storage = ::Sphynx::Reflection::details::EnumStorage<_Enum>::Instance; \
		static const Enum e { \
			::Sphynx::Reflection::Type{ #_Enum, sizeof(_Enum), alignof(_Enum), false }, \
			Storage->Entries.data(), \
			Storage->Entries.size(), \
			Storage->Attributes.data(), \
			Storage->Attributes.size() \
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
		inline Iterator begin() { return ::Sphynx::Reflection::details::EnumStorage<_Enum>::Instance->Values.data(); } \
		inline Iterator begin() const { return ::Sphynx::Reflection::details::EnumStorage<_Enum>::Instance->Values.data(); } \
		inline Iterator end() { return ::Sphynx::Reflection::details::EnumStorage<_Enum>::Instance->Values.data() + ::Sphynx::Reflection::details::EnumStorage<_Enum>::Instance->Values.size(); } \
		inline Iterator end() const { return ::Sphynx::Reflection::details::EnumStorage<_Enum>::Instance->Values.data() + ::Sphynx::Reflection::details::EnumStorage<_Enum>::Instance->Values.size(); } \
	};

#define SPX_REFLECT_ENUM_VALUE_BEGIN(_Value) \
	{ \
		Entries.push_back(::Sphynx::Reflection::Enum::Entry{ std::string(#_Value) ,(int64_t)context_enum::_Value }); \
		Values.push_back((int64_t)context_enum::_Value); \
		[[maybe_unused]] auto& Entry = *(Entries.end() - 1); \
		[[maybe_unused]] auto& Attributes = Entry.Attributes;

#define SPX_REFLECT_ENUM_VALUE_END() \
	}

#define SPX_REFLECT_ENUM_VALUE(_Value) \
	SPX_REFLECT_ENUM_VALUE_BEGIN(_Value) \
	SPX_REFLECT_ENUM_VALUE_END()

#define SPX_REFLECT_ENUM(_Enum) \
	SPX_REFLECT_ENUM_BEGIN(_Enum) \
	SPX_REFLECT_ENUM_END(_Enum)

 // ---------- Attribute -----------
#define SPX_REFLECT_ATTRIBUTE(Attr, ...) \
	{ \
		using namespace ::Sphynx::Reflection::CommonAttribute; \
		static Attr s_Attribute{ __VA_ARGS__ }; \
		Attributes.push_back(&s_Attribute); \
	}


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
		static const Type type{ #_Type, sizeof(_Type), alignof(_Type), true }; \
		return type; \
	}

#define TYPES() \
	X(bool)					\
	X(char)					\
	X(wchar_t)					\
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
			static const Type type{ "void", 0, 0, true };
			return type;
		}

		template<> inline const Type& details::GetTypeImpl<std::string>()
		{
			static const Type type{ "std::string", sizeof(std::string), alignof(std::string), true };
			return type;
		}

		template<> inline const Type& details::GetTypeImpl<std::wstring>()
		{
			static const Type type{ "std::wstring", sizeof(std::wstring), alignof(std::wstring), true };
			return type;
		}

		template<> inline const Type& details::GetTypeImpl<std::filesystem::path>()
		{
			static const Type type{ "std::filesystem::path", sizeof(std::filesystem::path), alignof(std::filesystem::path), true };
			return type;
		}
	}
}

// DO NOT UNCOMMENT, THIS IS FOR TESTING PURPOSES
//namespace S
//{
//	struct Vector
//	{
//		int a;
//		int b;
//		int c;
//
//		int Hola(double a, int b) const
//		{
//			return 0;
//		}
//	};
//
//	struct TestA {};
//
//	enum class Test
//	{
//		A,
//		B,
//		C
//	};
//}
//
//SPX_REFLECT_CLASS_BEGIN(S::Vector)
//
//	// Attributes
//	SPX_REFLECT_ATTRIBUTE(Description, "Test class for testing purposes")
//
//	// Properties
//	SPX_REFLECT_PROPERTY_BEGIN(a)
//	SPX_REFLECT_ATTRIBUTE(Description, "This is a variable called 'a'")
//		SPX_REFLECT_ATTRIBUTE(Range, 0, 10)
//	SPX_REFLECT_PROPERTY_END(a)
//
//	SPX_REFLECT_PROPERTY(b)
//	SPX_REFLECT_PROPERTY(c)
//
//	// Functions
//	SPX_REFLECT_FUNCTION_BEGIN(Hola)
//	SPX_REFLECT_ATTRIBUTE(Description, "Function called Hola")
//		
//		SPX_REFLECT_FUNCTION_PARAM_BEGIN(a)
//			SPX_REFLECT_ATTRIBUTE(Description, "Param called a")
//		SPX_REFLECT_FUNCTION_PARAM_END(a)
//	SPX_REFLECT_FUNCTION_END(Hola)
//
//SPX_REFLECT_CLASS_END(S::Vector)
//
//SPX_REFLECT_CLASS(S::TestA)
//
//SPX_REFLECT_ENUM_BEGIN(S::Test)
//
//SPX_REFLECT_ATTRIBUTE(Description, "Test enum for testing purposes")
//
//SPX_REFLECT_ENUM_VALUE_BEGIN(A)
//	SPX_REFLECT_ATTRIBUTE(Description, "Enum value A for testing purposes")
//SPX_REFLECT_ENUM_VALUE_END(A)
//
//SPX_REFLECT_ENUM_VALUE(B)
//SPX_REFLECT_ENUM_VALUE(C)
//
//SPX_REFLECT_ENUM_END(S::Test)

