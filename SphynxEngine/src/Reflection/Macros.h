#pragma once


#define EXPAND_MACRO(x) x

// ---------- Class -----------
#define SPX_REFLECT_CLASS_BEGIN(_Class, _API) \
	namespace Sphynx { namespace Reflection { \
	template struct _API TypeRegister<_Class>; \
	namespace details { \
	template<> struct _API ClassImplWrapper<::_Class> { \
		inline static TypeRegister<_Class> s_TypeRegister{}; \
		static const Class& GetClassImpl(Tag<::_Class>) \
		{ \
			static ::Sphynx::Reflection::details::ClassStorage<::_Class> Storage([](auto* self) { \
				self->Size = sizeof(::_Class); \
				[[maybe_unused]] auto& ParentClasses = self->ParentClasses; \
				[[maybe_unused]] auto& Properties = self->Properties; \
				[[maybe_unused]] auto& Functions = self->Functions; \
				[[maybe_unused]] auto& Attributes = self->Attributes; \
				using context_type = ::_Class;

#define SPX_REFLECT_CLASS_END(_Class, _API) \
			}); \
			static const Class c { \
				::Sphynx::Reflection::Type{ ::Sphynx::Reflection::details::Tag<::_Class>{}, #_Class, sizeof(::_Class), alignof(::_Class), (Storage.IsStruct ? ::Sphynx::Reflection::TypeKind::STRUCT : ::Sphynx::Reflection::TypeKind::CLASS) }, \
				Storage.ParentClasses.data(), \
				Storage.ParentClasses.size(), \
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
	inline _API const Class& GetClassImpl(Tag<::_Class>) \
	{ \
		return ClassImplWrapper<::_Class>::GetClassImpl(Tag<::_Class>{}); \
	} \
	inline _API const Type& GetTypeImpl(Tag<::_Class>) \
	{ \
		return GetClassImpl(Tag<::_Class>{}); \
	} \
}}}

#define SPX_REFLECT_CLASS(_Class, _API) \
	SPX_REFLECT_CLASS_BEGIN(_Class, _API) \
	SPX_REFLECT_CLASS_END(_Class, _API)

#define SPX_REFLECT_TEMPLATE_CLASS_BEGIN(_Class, _API) \
	namespace Sphynx { namespace Reflection { namespace details { \
	template<typename ...T> struct _API ClassImplWrapper<::_Class<T...>> { \
		inline static TypeRegister<::_Class<T...>> s_TypeRegister{}; \
		static const Class& GetClassImpl(Tag<::_Class<T...>>) \
		{ \
			static ClassStorage<::_Class<T...>> Storage([](auto* self) \
				{ \
					self->Size = sizeof(::_Class<T...>); \
					[[maybe_unused]] auto& ParentClasses = self->ParentClasses; \
					[[maybe_unused]] auto& Properties = self->Properties; \
					[[maybe_unused]] auto& Functions = self->Functions; \
					[[maybe_unused]] auto& Attributes = self->Attributes; \
					using context_type = ::_Class<T...>;

#define SPX_REFLECT_TEMPLATE_CLASS_END(_Class, _API) \
				} \
			); \
			using TemplateArgsPack = ::Sphynx::Traits::args_pack<T...>; \
			static auto TemplatedArgs = GetTemplateArgumentPackArray<TemplateArgsPack>( \
				std::make_index_sequence<::Sphynx::Traits::args_pack_size<TemplateArgsPack>::value>()); \
			 \
			static const TemplateClass c{ \
				::Sphynx::Reflection::Type{ ::Sphynx::Reflection::details::Tag<::_Class<T...>>{}, #_Class, sizeof(::_Class<T...>), alignof(::_Class<T...>), Storage.IsStruct ? ::Sphynx::Reflection::TypeKind::STRUCT : ::Sphynx::Reflection::TypeKind::CLASS }, \
				Storage.ParentClasses.data(), \
				Storage.ParentClasses.size(), \
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
	_API inline const Class& GetClassImpl(Tag<::_Class<T...>>) \
	{ \
		return ClassImplWrapper<::_Class<T...>>::GetClassImpl(Tag<::_Class<T...>>{}); \
	} \
	template<typename... T> \
	_API inline const Type& GetTypeImpl(Tag<::_Class<T...>>) \
	{ \
		return GetClassImpl(Tag<::_Class<T...>>{}); \
	} \
}}}

#define SPX_REFLECT_TEMPLATE_CLASS(_Class, _API) \
	SPX_REFLECT_TEMPLATE_CLASS_BEGIN(_Class, _API) \
	SPX_REFLECT_TEMPLATE_CLASS_END(_Class, _API)

 // ---------- Struct -----------
#define SPX_REFLECT_STRUCT_BEGIN(_Struct, _API) \
	SPX_REFLECT_CLASS_BEGIN(_Struct, _API) \
	self->IsStruct = true;

#define SPX_REFLECT_STRUCT_END(_Struct, _API) \
	SPX_REFLECT_CLASS_END(_Struct, _API)

#define SPX_REFLECT_STRUCT(_Struct, _API) \
	SPX_REFLECT_STRUCT_BEGIN(_Struct, _API) \
	SPX_REFLECT_STRUCT_END(_Struct, _API)

#define SPX_REFLECT_TEMPLATE_STRUCT_BEGIN(_Struct, _API) \
	SPX_REFLECT_TEMPLATE_CLASS_BEGIN(_Struct, _API) \
	self->IsStruct = true;

#define SPX_REFLECT_TEMPLATE_STRUCT_END(_Struct, _API) \
	SPX_REFLECT_TEMPLATE_CLASS_END(_Struct, _API)

#define SPX_REFLECT_TEMPLATE_STRUCT(_Struct, _API) \
	SPX_REFLECT_TEMPLATE_STRUCT_BEGIN(_Struct, _API) \
	SPX_REFLECT_TEMPLATE_STRUCT_END(_Struct, _API)

// ---------- Parent (inheritance) -----------
#define SPX_REFLECT_PARENT(_Class) \
	{ \
		const ::Sphynx::Reflection::Class& parent = ::Sphynx::Reflection::GetClass<::_Class>(); \
		ParentClasses.emplace_back(&parent); \
	} \

#define SPX_REFLECT_INHERITANCE(_Class) \
	SPX_REFLECT_PARENT(_Class)

// ---------- Property -----------
#define SPX_REFLECT_PROPERTY_BEGIN(_Property) \
	{ \
		Properties.emplace_back( \
			::Sphynx::Reflection::GetType<decltype(((context_type*)0)->_Property)>(), \
			#_Property, \
			offsetof(context_type, _Property) \
		); \
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
#define SPX_REFLECT_ENUM_BEGIN(_Enum, _API) \
	namespace Sphynx { namespace Reflection { namespace details { \
	_API inline const Enum& GetEnumImpl(Tag<_Enum>) \
	{ \
		static ::Sphynx::Reflection::details::EnumStorage<_Enum> Storage([](auto* self) { \
			using context_enum = _Enum; \
			[[maybe_unused]] auto& Entries = self->Entries; \
			[[maybe_unused]] auto& Values = self->Values; \
			[[maybe_unused]] auto& Attributes = self->Attributes; \


#define SPX_REFLECT_ENUM_END(_Enum, _API) \
		}); \
		using enum_underlying_type = typename std::underlying_type<::_Enum>::type; \
		static const Enum e{ \
			::Sphynx::Reflection::Type{ ::Sphynx::Reflection::details::Tag<::_Enum>{}, #_Enum, sizeof(::_Enum), alignof(::_Enum), ::Sphynx::Reflection::TypeKind::ENUM }, \
			Storage.Entries.data(), \
			Storage.Entries.size(), \
			Storage.Attributes.data(), \
			Storage.Attributes.size(), \
			::Sphynx::Reflection::details::Tag<enum_underlying_type>{} \
		}; \
		return e; \
	} \
	_API inline const Type& GetTypeImpl(Tag<::_Enum>) \
	{ \
		return GetEnumImpl(Tag<::_Enum>{}); \
	} \
	}}} \
	template<> struct _API ::Sphynx::Reflection::EnumRange<::_Enum> { \
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

#define SPX_REFLECT_ENUM(_Enum, _API) \
	SPX_REFLECT_ENUM_BEGIN(_Enum, _API) \
	SPX_REFLECT_ENUM_END(_Enum, _API)

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
private: \
	template<typename T> friend struct ::Sphynx::Reflection::details::ClassImplWrapper; \
	auto __reflection__dummy__B__() -> std::remove_reference<decltype(*this)>::type { return {}; }; \
	template<typename T> static T __reflection__dummy__A__(T(T::*)()) { return {}; }; \
	using Self = decltype(__reflection__dummy__A__(&__reflection__dummy__B__)); \
public: \
	inline static const ::Sphynx::Reflection::Class& StaticClass() { return ::Sphynx::Reflection::GetClass<Self>(); } \
	inline const ::Sphynx::Reflection::Class& GetStaticClass() const { return Self::StaticClass(); }


