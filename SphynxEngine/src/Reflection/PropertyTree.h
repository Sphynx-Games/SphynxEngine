#pragma once

#include "Core/Core.h"
#include "StdTypes.h"
#include "Attribute.h"


namespace Sphynx
{
	namespace Reflection
	{
		struct Property;

		class SPHYNX_API IPropertyTreeVisitor
		{
		public:
			virtual ~IPropertyTreeVisitor() = default;

		public:
			#define X(_Type)	virtual void Visit(const Property* property, _Type& data) = 0;
			TYPES()
			SPECIAL_TYPES()
			#undef X

			virtual void VisitEnum(const Property* property, void* data) = 0;
			virtual bool VisitClass(const Property* property, void* data) = 0;
			virtual bool VisitClass(const Property* property, void* data, const CommonAttribute::IndexedCollection& collection) = 0;
			virtual bool VisitClass(const Property* property, void* data, const CommonAttribute::AssociativeCollection& collection) = 0;
			
			// Visit events
			#define X(_Type)	virtual void OnBeforeVisit(const Property* property, _Type& data) {}; \
								virtual void OnAfterVisit(const Property* property, _Type& data) {};
			TYPES()
			SPECIAL_TYPES()
			#undef X

			virtual void OnBeforeVisitEnum(const Property* property, void* data) {};
			virtual void OnAfterVisitEnum(const Property* property, void* data) {};
			virtual void OnBeforeVisitClass(const Property* property, void* data) {};
			virtual void OnAfterVisitClass(const Property* property, void* data) {};
			virtual void OnBeforeVisitClass(const Property* property, void* data, const CommonAttribute::IndexedCollection& collection) {};
			virtual void OnAfterVisitClass(const Property* property, void* data, const CommonAttribute::IndexedCollection& collection) {};
			virtual void OnBeforeVisitClass(const Property* property, void* data, const CommonAttribute::AssociativeCollection& collection) {};
			virtual void OnAfterVisitClass(const Property* property, void* data, const CommonAttribute::AssociativeCollection& collection) {};
		};

		class SPHYNX_API PropertyTree
		{
		public:
			struct SPHYNX_API TraversalParams
			{
				std::unordered_map<const Type*, void(*)(PropertyTree&, const Property*, void*, IPropertyTreeVisitor&)> CustomTraversal{};
			};
		public:
			PropertyTree(const Type& type, void* addr, TraversalParams&& params = {});

		public:
			static void Traverse(const Type& type, void* addr, IPropertyTreeVisitor& visitor, TraversalParams&& params = {});
			void Traverse(IPropertyTreeVisitor& visitor);
			void Traverse(IPropertyTreeVisitor& visitor, const Property* property);

		private:
			const Type& m_Type;
			void* m_Addr;
			TraversalParams m_Params;
		};
	}
}
