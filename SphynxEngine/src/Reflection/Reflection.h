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

#include <string>
#include <filesystem>


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
	namespace Sphynx { namespace Reflection { namespace details { \
	template<> struct ClassImplWrapper<::_Class> { \
		static const Class& GetClassImpl(Tag<::_Class>) \
		{ \
			static ::Sphynx::Reflection::details::ClassStorage<::_Class> Storage([](auto* self) { \
				self->Size = sizeof(::_Class); \
				[[maybe_unused]] auto& Properties = self->Properties; \
				[[maybe_unused]] auto& Functions = self->Functions; \
				[[maybe_unused]] auto& Attributes = self->Attributes; \
				using context_type = ::_Class;

#define SPX_REFLECT_CLASS_END(_Class) \
			}); \
			static const Class c { \
				::Sphynx::Reflection::Type{ ::Sphynx::Reflection::details::Tag<::_Class>{}, #_Class, sizeof(::_Class), alignof(::_Class), (Storage.IsStruct ? ::Sphynx::Reflection::TypeKind::STRUCT : ::Sphynx::Reflection::TypeKind::CLASS) }, \
				Storage.Properties.data(), \
				Storage.Properties.size(), \
				Storage.Functions.data(), \
				Storage.Functions.size(), \
				Storage.Attributes.data(), \
				Storage.Attributes.size() \
			}; \
			return c; \
		} \
		 \
		static const Type& GetTypeImpl(Tag<::_Class>) \
		{ \
			return GetClassImpl(Tag<::_Class>{}); \
		} \
	}; \
	inline const Class& GetClassImpl(Tag<::_Class>) \
	{ \
		return ClassImplWrapper<::_Class>::GetClassImpl(Tag<::_Class>{}); \
	} \
	inline const Type& GetTypeImpl(Tag<::_Class>) \
	{ \
		return GetClassImpl(Tag<::_Class>{}); \
	} \
}}}

#define SPX_REFLECT_CLASS(_Class) \
	SPX_REFLECT_CLASS_BEGIN(_Class) \
	SPX_REFLECT_CLASS_END(_Class)

#define SPX_REFLECT_TEMPLATE_CLASS_BEGIN(_Class) \
	namespace Sphynx { namespace Reflection { namespace details { \
	template<typename ...T> struct ClassImplWrapper<::_Class<T...>> { \
		static const Class& GetClassImpl(Tag<::_Class<T...>>) \
		{ \
			static ClassStorage<::_Class<T...>> Storage([](auto* self) \
				{ \
					self->Size = sizeof(::_Class<T...>); \
					[[maybe_unused]] auto& Properties = self->Properties; \
					[[maybe_unused]] auto& Functions = self->Functions; \
					[[maybe_unused]] auto& Attributes = self->Attributes; \
					using context_type = ::_Class<T...>;

#define SPX_REFLECT_TEMPLATE_CLASS_END(_Class) \
				} \
			); \
			using TemplateArgsPack = ::Sphynx::Traits::args_pack<T...>; \
			static auto TemplatedArgs = GetTemplateArgumentPackArray<TemplateArgsPack>( \
				std::make_index_sequence<::Sphynx::Traits::args_pack_size<TemplateArgsPack>::value>()); \
			 \
			static const TemplateClass c{ \
				::Sphynx::Reflection::Type{ ::Sphynx::Reflection::details::Tag<::_Class<T...>>{}, #_Class, sizeof(::_Class<T...>), alignof(::_Class<T...>), Storage.IsStruct ? ::Sphynx::Reflection::TypeKind::STRUCT : ::Sphynx::Reflection::TypeKind::CLASS }, \
				Storage.Properties.data(), \
				Storage.Properties.size(), \
				Storage.Functions.data(), \
				Storage.Functions.size(), \
				Storage.Attributes.data(), \
				Storage.Attributes.size(), \
				TemplatedArgs.data(), \
				TemplatedArgs.size() \
			}; \
			return c; \
		} \
		 \
		static const Type& GetTypeImpl(Tag<::_Class<T...>>) \
		{ \
			return GetClassImpl(Tag<::_Class<T...>>{}); \
		} \
	}; \
	template<typename... T> \
	inline const Class& GetClassImpl(Tag<::_Class<T...>>) \
	{ \
		return ClassImplWrapper<::_Class<T...>>::GetClassImpl(Tag<::_Class<T...>>{}); \
	} \
	template<typename... T> \
	inline const Type& GetTypeImpl(Tag<::_Class<T...>>) \
	{ \
		return GetClassImpl(Tag<::_Class<T...>>{}); \
	} \
}}}

#define SPX_REFLECT_TEMPLATE_CLASS(_Class) \
	SPX_REFLECT_TEMPLATE_CLASS_BEGIN(_Class) \
	SPX_REFLECT_TEMPLATE_CLASS_END(_Class)

 // ---------- Struct -----------
#define SPX_REFLECT_STRUCT_BEGIN(_Struct) \
	SPX_REFLECT_CLASS_BEGIN(_Struct) \
	self->IsStruct = true;

#define SPX_REFLECT_STRUCT_END(_Struct) \
	SPX_REFLECT_CLASS_END(_Struct)

#define SPX_REFLECT_STRUCT(_Struct) \
	SPX_REFLECT_STRUCT_BEGIN(_Struct) \
	SPX_REFLECT_STRUCT_END(_Struct)

#define SPX_REFLECT_TEMPLATE_STRUCT_BEGIN(_Struct) \
	SPX_REFLECT_TEMPLATE_CLASS_BEGIN(_Struct) \
	self->IsStruct = true;

#define SPX_REFLECT_TEMPLATE_STRUCT_END(_Struct) \
	SPX_REFLECT_TEMPLATE_CLASS_END(_Struct)

#define SPX_REFLECT_TEMPLATE_STRUCT(_Struct) \
	SPX_REFLECT_TEMPLATE_STRUCT_BEGIN(_Struct) \
	SPX_REFLECT_TEMPLATE_STRUCT_END(_Struct)

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
	namespace Sphynx { namespace Reflection { namespace details { \
	inline const Enum& GetEnumImpl(Tag<_Enum>) \
	{ \
		static ::Sphynx::Reflection::details::EnumStorage<_Enum> Storage([](auto* self) { \
			using context_enum = _Enum; \
			[[maybe_unused]] auto& Entries = self->Entries; \
			[[maybe_unused]] auto& Values = self->Values; \
			[[maybe_unused]] auto& Attributes = self->Attributes; \


#define SPX_REFLECT_ENUM_END(_Enum) \
		}); \
		static const Enum e{ \
			::Sphynx::Reflection::Type{ ::Sphynx::Reflection::details::Tag<::_Enum>{}, #_Enum, sizeof(::_Enum), alignof(::_Enum), ::Sphynx::Reflection::TypeKind::ENUM }, \
			Storage.Entries.data(), \
			Storage.Entries.size(), \
			Storage.Attributes.data(), \
			Storage.Attributes.size(), \
			::Sphynx::Reflection::details::Tag<typename std::underlying_type<::_Enum>::type>{} \
		}; \
		return e; \
	} \
	inline const Type& GetTypeImpl(Tag<::_Enum>) \
	{ \
		return GetEnumImpl(Tag<::_Enum>{}); \
	} \
	}}} \
	template<> struct ::Sphynx::Reflection::EnumRange<::_Enum> { \
		struct Iterator \
		{ \
			using difference_type = std::ptrdiff_t; \
			using value_type = ::_Enum; \
			using pointer = ::_Enum*; \
			using reference = ::_Enum&; \
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
		inline Iterator begin() { return ::Sphynx::Reflection::details::EnumStorage<::_Enum>::Instance->Values.data(); } \
		inline Iterator begin() const { return ::Sphynx::Reflection::details::EnumStorage<::_Enum>::Instance->Values.data(); } \
		inline Iterator end() { return ::Sphynx::Reflection::details::EnumStorage<::_Enum>::Instance->Values.data() + ::Sphynx::Reflection::details::EnumStorage<_Enum>::Instance->Values.size(); } \
		inline Iterator end() const { return ::Sphynx::Reflection::details::EnumStorage<::_Enum>::Instance->Values.data() + ::Sphynx::Reflection::details::EnumStorage<_Enum>::Instance->Values.size(); } \
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
#define SPX_REFLECT_ATTRIBUTE_INTERNAL(Attr, ...) \
	{ \
		using namespace ::Sphynx::Reflection::CommonAttribute; \
		static Attr s_Attribute{ ##__VA_ARGS__ }; \
		Attributes.push_back(&s_Attribute); \
	}

#define SPX_REFLECT_ATTRIBUTE(...) EXPAND_MACRO( SPX_REFLECT_ATTRIBUTE_INTERNAL( __VA_ARGS__ ) )

// ---------- Reflection Body -----------
#define SPX_REFLECT_GENERATED_BODY() \
	template<typename T> \
	friend struct ::Sphynx::Reflection::details::ClassImplWrapper


namespace Sphynx
{
	namespace Reflection
	{
		namespace details
		{
			template<typename T>
			struct Tag {};

			template<typename T>
			struct ClassImplWrapper;

			/*
			template<typename T>
			inline const Type& GetTypeImpl(Tag<T>);

			template<typename T>
			inline const Class& GetClassImpl(Tag<T>);

			template<typename T>
			inline const Enum& GetEnumImpl(Tag<T>);
			*/
		}

		template<typename Enum>
		struct EnumRange;

		template<typename T>
		inline const Type& GetType()
		{
			return details::GetTypeImpl(details::Tag<T>{});
		}

		template<typename T>
		inline const Class& GetClass()
		{
			return details::GetClassImpl(details::Tag<T>{});
		}

		template<typename T>
		inline const Enum& GetEnum()
		{
			return details::GetEnumImpl(details::Tag<T>{});
		}

#define X(_Type) \
	inline const Type& GetTypeImpl(Tag<_Type>) \
	{ \
		static const Type type{ ::Sphynx::Reflection::details::Tag<_Type>{}, #_Type, sizeof(_Type), alignof(_Type), ::Sphynx::Reflection::TypeKind::PRIMITIVE }; \
		return type; \
	}

#define TYPES() \
	X(bool)					\
	X(char)					\
	X(wchar_t)				\
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

		namespace details
		{

			TYPES()

			inline const Type& GetTypeImpl(Tag<void>)
			{
				static const Type type{ Tag<void>{}, "void", 0, 0, TypeKind::PRIMITIVE };
				return type;
			}

			inline const Type& GetTypeImpl(Tag<::std::string>)
			{
				static const Type type{ Tag<::std::string>{}, "std::string", sizeof(::std::string), alignof(::std::string), TypeKind::PRIMITIVE };
				return type;
			}

			inline const Type& GetTypeImpl(Tag<::std::wstring>)
			{
				static const Type type{ Tag<::std::wstring>{}, "std::wstring", sizeof(::std::wstring), alignof(::std::wstring), TypeKind::PRIMITIVE };
				return type;
			}

			inline const Type& GetTypeImpl(Tag<::std::filesystem::path>)
			{
				static const Type type{ Tag<::std::filesystem::path>{}, "std::filesystem::path", sizeof(::std::filesystem::path), alignof(::std::filesystem::path), TypeKind::PRIMITIVE };
				return type;
			}
		}
	}
}
#undef X
#undef TYPES

// DO NOT UNCOMMENT, THIS IS FOR TESTING PURPOSES
//namespace S
//{
//	struct Vector
//	{
//		SPX_REFLECT_GENERATED_BODY();
//	private:
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

