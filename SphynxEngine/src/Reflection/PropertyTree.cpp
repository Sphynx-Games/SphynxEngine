#include "PropertyTree.h"
#include "Type.h"
#include "Class.h"
#include "Enum.h"
#include "Property.h"


namespace Sphynx
{
	namespace Reflection
	{
		PropertyTree::PropertyTree(const Type& type, void* addr, TraversalParams&& params) :
			m_Type(type),
			m_Addr(addr),
			m_Params(std::move(params))
		{

		}

		void PropertyTree::Traverse(const Type& type, void* addr, IPropertyTreeVisitor& visitor, TraversalParams&& params)
		{
			PropertyTree tree{ type, addr, std::move(params) };
			tree.Traverse(visitor);
		}

		void PropertyTree::Traverse(IPropertyTreeVisitor& visitor)
		{
			const Property fakeProperty{ m_Type, m_Type.Name, 0 };
			Traverse(visitor, &fakeProperty);
		}

		void PropertyTree::Traverse(IPropertyTreeVisitor& visitor, const Property* property)
		{
			auto TryTraverseIfPointer = [&](const Property* property)
				{
					if (!property->IsPointer()) return;

					Property fakeProperty{ *property };
					fakeProperty.Offset = 0;
					--fakeProperty.PointerIndirectionCount;
					void* addr = (void*)(*(uintptr_t*)((std::byte*)m_Addr + property->Offset));

					PropertyTree tree{ fakeProperty.Type, addr, TraversalParams{m_Params} };
					tree.Traverse(visitor, &fakeProperty);
				};

			if (m_Type.Kind == TypeKind::PRIMITIVE)
			{
#define X(_Type) \
				if (&GetType<_Type>() == &m_Type) \
				{ \
					visitor.OnBeforeVisit(property, *((_Type*)m_Addr)); \
					visitor.Visit(property, *((_Type*)m_Addr)); \
					TryTraverseIfPointer(property); \
					visitor.OnAfterVisit(property, *((_Type*)m_Addr)); \
					goto end; \
				} \

				TYPES()
					SPECIAL_TYPES()
#undef X

					SPX_CORE_ASSERT(false, "Primitive type not handled");
			end: {}

			}
			else if (m_Type.Kind == TypeKind::ENUM)
			{
				const Enum& rEnum = static_cast<const Enum&>(m_Type);
				visitor.OnBeforeVisitEnum(property, m_Addr);
				visitor.VisitEnum(property, m_Addr);
				TryTraverseIfPointer(property);
				visitor.OnAfterVisitEnum(property, m_Addr);
			}
			else // struct or class
			{
				const Reflection::Class& rClass = static_cast<const Reflection::Class&>(m_Type);

				// Treat special cases first
				if (const CommonAttribute::IndexedCollection* indexedCollection = rClass.GetAttribute<CommonAttribute::IndexedCollection>())
				{
					visitor.OnBeforeVisitClass(property, m_Addr, *indexedCollection);
					const bool skip = !visitor.VisitClass(property, m_Addr, *indexedCollection) || property->IsPointer();
					TryTraverseIfPointer(property);

					auto it = m_Params.CustomTraversal.find(&property->Type);
					if (it != m_Params.CustomTraversal.end())
					{
						(*it).second(*this, property, m_Addr, visitor);
					}
					else for (size_t i = 0; !skip && i < indexedCollection->GetSize(m_Addr); ++i)
					{
						const Type& rType = indexedCollection->GetValueType();
						std::string indexStr = std::to_string(i);

						const Property fakeProperty{ rType, indexStr.c_str(), 0 };
						PropertyTree tree{ rType, indexedCollection->Get(m_Addr, i), TraversalParams{m_Params} };
						tree.Traverse(visitor, &fakeProperty);
					}

					visitor.OnAfterVisitClass(property, m_Addr, *indexedCollection);
				}
				else if (const CommonAttribute::AssociativeCollection* associativeCollection = rClass.GetAttribute<CommonAttribute::AssociativeCollection>())
				{
					visitor.OnBeforeVisitClass(property, m_Addr, *associativeCollection);
					const bool skip = !visitor.VisitClass(property, m_Addr, *associativeCollection) || property->IsPointer();
					TryTraverseIfPointer(property);

					auto it = m_Params.CustomTraversal.find(&property->Type);
					if (it != m_Params.CustomTraversal.end())
					{
						(*it).second(*this, property, m_Addr, visitor);
					}
					else for (size_t i = 0; !skip && i < associativeCollection->GetSize(m_Addr); ++i)
					{
						{
							const Type& rType = associativeCollection->GetKeyType();

							const Property fakeProperty{ rType, rType.Name, 0 };
							PropertyTree tree{ rType, (void*)associativeCollection->GetKey(m_Addr, i), TraversalParams{m_Params} };
							tree.Traverse(visitor, &fakeProperty);
						}
						{
							const Type& rType = associativeCollection->GetValueType();
							const Property fakeProperty{ rType, rType.Name, 0 };

							PropertyTree tree{ associativeCollection->GetValueType(), (void*)associativeCollection->GetValue(m_Addr, i), TraversalParams{m_Params} };
							tree.Traverse(visitor, &fakeProperty);
						}
					}

					visitor.OnAfterVisitClass(property, m_Addr, *associativeCollection);
				}
				else
				{
					visitor.OnBeforeVisitClass(property, m_Addr);
					const bool skip = !visitor.VisitClass(property, m_Addr) || property->IsPointer();
					TryTraverseIfPointer(property);

					if (!skip)
					{
						auto it = m_Params.CustomTraversal.find(&property->Type);
						if (it != m_Params.CustomTraversal.end())
						{
							(*it).second(*this, property, m_Addr, visitor);
						}
						else for (const Property& cProperty : rClass)
						{
							PropertyTree tree{ cProperty.Type, (std::byte*)m_Addr + cProperty.Offset, TraversalParams{m_Params} };
							tree.Traverse(visitor, &cProperty);
						}
					}

					visitor.OnAfterVisitClass(property, m_Addr);
				}

			}
		}
	}
}
