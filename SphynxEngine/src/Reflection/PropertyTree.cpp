#include "PropertyTree.h"
#include "Type.h"
#include "Class.h"
#include "Enum.h"
#include "Property.h"


namespace Sphynx
{
	namespace Reflection
	{
		PropertyTree::PropertyTree(const Type& type, void* addr) :
			m_Type(type),
			m_Addr(addr)
		{

		}

		void PropertyTree::Traverse(const Type& type, void* addr, IPropertyTreeVisitor& visitor)
		{
			PropertyTree tree{ type, addr };
			tree.Traverse(visitor);
		}

		void PropertyTree::Traverse(IPropertyTreeVisitor& visitor)
		{
			const Property fakeProperty{ m_Type, m_Type.Name, 0, 0, AccessSpecifier::PUBLIC };
			Traverse(visitor, &fakeProperty);
		}

		void PropertyTree::Traverse(IPropertyTreeVisitor& visitor, const Property* property)
		{
			if (m_Type.Kind == TypeKind::PRIMITIVE)
			{
#define X(_Type) \
				if (&GetType<_Type>() == &m_Type) \
				{ \
					visitor.OnBeforeVisit(property, *((_Type*)m_Addr)); \
					visitor.Visit(property, *((_Type*)m_Addr)); \
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
				visitor.OnAfterVisitEnum(property, m_Addr);
			}
			else // struct or class
			{
				const Reflection::Class& rClass = static_cast<const Reflection::Class&>(m_Type);

				// Treat special cases first
				if (const IndexedCollection* indexedCollection = rClass.GetAttribute<IndexedCollection>())
				{
					visitor.OnBeforeVisitClass(property, m_Addr, *indexedCollection);
					const bool skip = !visitor.VisitClass(property, m_Addr, *indexedCollection);

					for (size_t i = 0; !skip && i < indexedCollection->GetSize(m_Addr); ++i)
					{
						const Type& rType = indexedCollection->GetValueType();
						std::string indexStr = std::to_string(i);

						const Property fakeProperty{ rType, indexStr.c_str(), 0, 0, AccessSpecifier::PUBLIC };
						PropertyTree tree{ rType, indexedCollection->Get(m_Addr, i) };
						tree.Traverse(visitor, &fakeProperty);
					}

					visitor.OnAfterVisitClass(property, m_Addr, *indexedCollection);
				}
				else if (const AssociativeCollection* associativeCollection = rClass.GetAttribute<AssociativeCollection>())
				{
					visitor.OnBeforeVisitClass(property, m_Addr, *associativeCollection);
					const bool skip = !visitor.VisitClass(property, m_Addr, *associativeCollection);

					for (size_t i = 0; !skip && i < associativeCollection->GetSize(m_Addr); ++i)
					{
						{
							const Type& rType = associativeCollection->GetKeyType();

							const Property fakeProperty{ rType, rType.Name, 0, 0, AccessSpecifier::PUBLIC };
							PropertyTree tree{ rType, (void*)associativeCollection->GetKey(m_Addr, i) };
							tree.Traverse(visitor, &fakeProperty);
						}
						{
							const Type& rType = associativeCollection->GetValueType();
							const Property fakeProperty{ rType, rType.Name, 0, 0, AccessSpecifier::PUBLIC };

							PropertyTree tree{ associativeCollection->GetValueType(), (void*)associativeCollection->GetValue(m_Addr, i) };
							tree.Traverse(visitor, &fakeProperty);
						}
					}

					visitor.OnAfterVisitClass(property, m_Addr, *associativeCollection);
				}
				else
				{
					visitor.OnBeforeVisitClass(property, m_Addr);
					const bool skip = !visitor.VisitClass(property, m_Addr);

					if (!skip)
					{
						for (const Property& cProperty : rClass)
						{
							PropertyTree tree{ cProperty.Type, (std::byte*)m_Addr + cProperty.Offset };
							tree.Traverse(visitor, &cProperty);
						}
					}

					visitor.OnAfterVisitClass(property, m_Addr);
				}

			}
		}
	}
}
