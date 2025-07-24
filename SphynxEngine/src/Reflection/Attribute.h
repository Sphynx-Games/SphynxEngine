#pragma once

#include "Core/Core.h"
#include <cstdint>


namespace Sphynx
{
	namespace Reflection
	{
		struct Type;

		class SPHYNX_API Attribute
		{
		public:
			Attribute();
			virtual ~Attribute();

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
			class SPHYNX_API IndexedCollection : public ::Sphynx::Reflection::Attribute
			{
				using TAccessFunction = void* (*)(void*, uint64_t);
				using TConstAccessFunction = const void* (*)(const void*, uint64_t);
				using TSizeFunction = size_t(*)(const void*);
				using TAddFunction = void* (*)(void*);

			public:
				template<typename TCollection>
				IndexedCollection(::Sphynx::Reflection::details::Tag<TCollection>) :
					m_ValueType(GetType<typename TCollection::value_type>()),
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

			class SPHYNX_API AssociativeCollection : public ::Sphynx::Reflection::Attribute
			{
				using TAccessFunction = void* (*)(void*, const void*);
				using TConstAccessFunction = const void* (*)(const void*, const void*);
				using TKeyAccessFunction = void* (*)(const void*, uint64_t);
				using TSizeFunction = size_t(*)(const void*);
				using TAddFunction = void* (*)(void*, const void*, const void*);
				using TCompareKeyFunction = bool(*)(const void*, const void*);
				using THasKeyFunction = bool(*)(const void*, const void*);
			public:
				template<typename TCollection>
				AssociativeCollection(::Sphynx::Reflection::details::Tag<TCollection>) :
					m_KeyType(GetType<typename TCollection::key_type>()),
					m_ValueType(GetType<typename TCollection::mapped_type>()),
					m_AccessFunction([](void* obj, const void* key) -> void* { return &(*static_cast<TCollection*>(obj))[*static_cast<const typename TCollection::key_type*>(key)]; }),
					m_ConstAccessFunction([](const void* obj, const void* key) -> const void* { return &(*static_cast<const TCollection*>(obj))[*static_cast<const typename TCollection::key_type*>(key)]; }),
					m_KeyAccessFunction([](const void* obj, uint64_t index) -> void* { return (void*)(&(*std::next(static_cast<const TCollection*>(obj)->cbegin(), index))); }),
					m_SizeFunction([](const void* obj) -> size_t { return static_cast<const TCollection*>(obj)->Size(); }),
					m_AddFunction([](void* obj, const void* key, const void* value) -> void*
						{
							static_cast<TCollection*>(obj)->Add(
								*static_cast<const typename TCollection::key_type*>(key),
								*static_cast<const typename TCollection::mapped_type*>(value));
							return &(*static_cast<TCollection*>(obj))[*static_cast<const typename TCollection::key_type*>(key)];
						}),
					m_CompareKeysFunction([](const void* keyA, const void* keyB) -> bool
						{ 
							if constexpr (::Sphynx::Traits::is_container_key_hashable<TCollection>::value)
							{
								return TCollection::hasher{}(*static_cast<const typename TCollection::key_type*>(keyA)) ==
									TCollection::hasher{}(*static_cast<const typename TCollection::key_type*>(keyB));
							}
							else if constexpr (::Sphynx::Traits::is_container_key_comparable<TCollection>::value)
							{
								return *static_cast<const typename TCollection::key_type*>(keyA) == 
									*static_cast<const typename TCollection::key_type*>(keyB);
							}
							else
							{
								static_assert(false, "Not implemented");
							}

							return false;
						}),
					m_HasKeyFunction([](const void* obj, const void* key) -> bool 
						{ 
							return std::find_if(static_cast<const TCollection*>(obj)->cbegin(), static_cast<const TCollection*>(obj)->cend(), [&](const auto& p)
								{
									return *static_cast<const typename TCollection::key_type*>((void*)&p) == *static_cast<const typename TCollection::key_type*>(key);
								}) != static_cast<const TCollection*>(obj)->cend();
						})
				{}

			public:
				inline const Type& GetKeyType() const { return m_KeyType; }
				inline const Type& GetValueType() const { return m_ValueType; }

				inline void* Get(void* obj, const void* key) const { return m_AccessFunction(obj, key); }
				inline const void* Get(const void* obj, const void* key) const { return m_ConstAccessFunction(obj, key); }
				inline const void* GetKey(const void* obj, uint64_t index) const { return m_KeyAccessFunction(obj, index); }
				inline void* GetKey(void* obj, uint64_t index) const { return m_KeyAccessFunction(obj, index); }
				inline const void* GetValue(const void* obj, uint64_t index) const { return Get(obj, GetKey(obj, index)); }
				inline void* GetValue(void* obj, uint64_t index) const { return Get(obj, GetKey(obj, index)); }
				inline const size_t GetSize(const void* obj) const { return m_SizeFunction(obj); }

				inline void* Add(void* obj, const void* key, const void* value) const { return m_AddFunction(obj, key, value); };

				inline bool CompareKeys(const void* keyA, const void* keyB) const { return m_CompareKeysFunction(keyA, keyB); }
				inline bool ContainsKey(const void* obj, const void* key) const { return m_HasKeyFunction(obj, key); }

			private:
				const Type& m_KeyType;
				const Type& m_ValueType;
				TAccessFunction m_AccessFunction;
				TConstAccessFunction m_ConstAccessFunction;
				TKeyAccessFunction m_KeyAccessFunction;
				TSizeFunction m_SizeFunction;
				TAddFunction m_AddFunction;
				TCompareKeyFunction m_CompareKeysFunction;
				THasKeyFunction m_HasKeyFunction;
			};

			// Plain Old Data Attribute
			class SPHYNX_API POD : public ::Sphynx::Reflection::Attribute
			{
				using TCopyToFunction = void(*)(const void*, void*);

			public:
				template<typename T>
				POD(::Sphynx::Reflection::details::Tag<T>) :
					m_CopyToFunction([](const void* source, void* dest) -> void { *((T*)dest) = *((T*)source); })
				{}
				virtual ~POD();

			public:
				void CopyTo(const void* source, void* dest) const;

			private:
				TCopyToFunction m_CopyToFunction;
			};

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

		namespace details
		{
			template<typename TAttribute, typename TContext, typename... Args>
			TAttribute CreateAttribute(Args&&... args)
			{
				if constexpr (std::is_constructible<TAttribute, Tag<TContext>, Args...>::value)
				{
					return TAttribute{ Tag<TContext>{}, std::forward<Args>(args)... };
				}
				else
				{
					return TAttribute{ std::forward<Args>(args)... };
				}
			}

		}
	}
}
