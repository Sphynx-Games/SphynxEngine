#pragma once


namespace Sphynx
{
	namespace Reflection
	{
		class SPHYNX_API Attribute
		{
		public:
			virtual ~Attribute() = default;

		};
	}
}

// Common attributes
namespace Sphynx
{
	namespace Reflection
	{
		namespace details
		{
			// Pre-declaration
			template<typename T>
			struct Tag;
		}

		namespace CommonAttribute
		{
			template<typename T>
			class SPHYNX_API Collection;

			template<>
			class SPHYNX_API Collection<struct Indexed> : public ::Sphynx::Reflection::Attribute
			{
				using TAccessFunction = void* (*)(void*, uint64_t);
				using TConstAccessFunction = const void* (*)(const void*, uint64_t);
				using TSizeFunction = size_t(*)(const void*);
				using TAddFunction = void* (*)(void*);

			public:
				template<typename TCollection>
				Collection(::Sphynx::Reflection::details::Tag<TCollection>) :
					m_ValueType(GetType<TCollection::value_type>()),
					m_AccessFunction([](void* obj, uint64_t index) -> void* { return &(*static_cast<TCollection*>(obj))[index]; }),
					m_ConstAccessFunction([](const void* obj, uint64_t index) -> const void* { return &(*static_cast<const TCollection*>(obj))[index]; }),
					m_SizeFunction([](const void* obj) -> size_t { return static_cast<const TCollection*>(obj)->Size(); }),
					m_AddFunction([](void* obj) -> void* { return &(static_cast<TCollection*>(obj)->Emplace()); })
				{}

			public:
				inline const Type& GetValueType() const { return m_ValueType; }

				inline void* Get(void* obj, uint64_t index) const { return m_AccessFunction(obj, index); }
				inline const void* Get(const void* obj, uint64_t index) const { return m_ConstAccessFunction(obj, index); }
				inline const size_t GetSize(const void* obj) const { return m_SizeFunction(obj); }

				inline void* Add(void* obj) const { return m_AddFunction(obj); };

			private:
				const Type& m_ValueType;
				TAccessFunction m_AccessFunction;
				TConstAccessFunction m_ConstAccessFunction;
				TSizeFunction m_SizeFunction;
				TAddFunction m_AddFunction;
			};

			template<>
			class SPHYNX_API Collection<struct Associative> : public ::Sphynx::Reflection::Attribute
			{
			};

			using IndexedCollection = Collection<Indexed>;
			using AssociativeCollection = Collection<Associative>;

#define		IndexedCollection		::Sphynx::Reflection::CommonAttribute::IndexedCollection , ::Sphynx::Reflection::details::Tag<context_type>{}
#define		AssociativeCollection	::Sphynx::Reflection::CommonAttribute::AssociativeCollection , ::Sphynx::Reflection::details::Tag<context_type>{}

			// Plain Old Data Attribute
			class SPHYNX_API POD : public ::Sphynx::Reflection::Attribute
			{
				using TCopyToFunction = void(*)(const void*, void*);

			public:
				template<typename T>
				POD(::Sphynx::Reflection::details::Tag<T>) :
					m_CopyToFunction([](const void* source, void* dest) -> void { *((T*)dest) = *((T*)source); })
				{}

			public:
				void CopyTo(const void* source, void* dest) const { m_CopyToFunction(source, dest); }

			private:
				TCopyToFunction m_CopyToFunction;
			};

#define		POD		::Sphynx::Reflection::CommonAttribute::POD , ::Sphynx::Reflection::details::Tag<context_type>{}

			class SPHYNX_API Description : public ::Sphynx::Reflection::Attribute
			{
			public:
				Description(const char* description) :
					m_Description(description) {};

			private:
				const char* m_Description;

			};

			class SPHYNX_API Range : public ::Sphynx::Reflection::Attribute
			{
			public:
				Range(float from, float to) :
					m_From(from),
					m_To(to)
				{};

			private:
				float m_From;
				float m_To;
			};
		}
	}
}
