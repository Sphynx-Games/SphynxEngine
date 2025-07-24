#pragma once

#include "Core/Core.h"
#include "PropertyTree.h"
#include "Core/Delegate.h"
#include "Container/Pair.h"

#include <stack>


namespace Sphynx
{
	namespace Reflection
	{
		struct Class;

		enum PropertyDiffType
		{
			SOURCE_KEY_PROPERTY_NOT_FOUND,
			SOURCE_VALUE_PROPERTY_NOT_FOUND,
			TARGET_KEY_PROPERTY_NOT_FOUND,
			TARGET_VALUE_PROPERTY_NOT_FOUND,
			DIFFERENT_VALUE
		};

		struct SPHYNX_API PropertyDiffInfo
		{
			PropertyDiffInfo(const Reflection::Property* sourceProperty, void* sourceData, const Reflection::Property* targetProperty, void* targetData, PropertyDiffType type) :
				SourceProperty(sourceProperty),
				SourceData(sourceData),
				TargetProperty(targetProperty),
				TargetData(targetData),
				Type(type)
			{}

			const Reflection::Property* SourceProperty;
			void* SourceData;
			const Reflection::Property* TargetProperty;
			void* TargetData;
			PropertyDiffType Type;
		};

		class SPHYNX_API PropertyComparator : public IPropertyTreeVisitor
		{
			struct PropertyNode
			{
				enum NodeType
				{
					IndexedCollection,
					AssociativeCollection,
					NonCollection
				};

				const Reflection::Property* Property;
				void* Address;
				NodeType Type;
				std::vector<Reflection::Property> CollectionProperties;
				std::vector<std::string> IndexedCollectionPropertyNames;

				// only used in associative collections
				bool VisitingKey;
				void* CollectionAddress;
				const Reflection::Property* ValueProperty;
			};

		public:
			PropertyComparator(void* target, const Class& reflectionClass);

			virtual void Visit(const Reflection::Property* property, bool& data) override;
			virtual void Visit(const Reflection::Property* property, char& data) override;
			virtual void Visit(const Reflection::Property* property, signed char& data) override;
			virtual void Visit(const Reflection::Property* property, wchar_t& data) override;
			virtual void Visit(const Reflection::Property* property, short& data) override;
			virtual void Visit(const Reflection::Property* property, int& data) override;
			virtual void Visit(const Reflection::Property* property, long& data) override;
			virtual void Visit(const Reflection::Property* property, long long& data) override;
			virtual void Visit(const Reflection::Property* property, float& data) override;
			virtual void Visit(const Reflection::Property* property, double& data) override;
			virtual void Visit(const Reflection::Property* property, long double& data) override;
			virtual void Visit(const Reflection::Property* property, unsigned char& data) override;
			virtual void Visit(const Reflection::Property* property, unsigned short& data) override;
			virtual void Visit(const Reflection::Property* property, unsigned int& data) override;
			virtual void Visit(const Reflection::Property* property, unsigned long& data) override;
			virtual void Visit(const Reflection::Property* property, unsigned long long& data) override;
			virtual void Visit(const Reflection::Property* property, ::std::string& data) override;
			virtual void Visit(const Reflection::Property* property, ::std::wstring& data) override;
			virtual void Visit(const Reflection::Property* property, ::std::filesystem::path& data) override;

			virtual void VisitEnum(const Reflection::Property* property, void* data) override;
			virtual bool VisitClass(const Reflection::Property* property, void* data) override;
			virtual bool VisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::IndexedCollection& collection) override;
			virtual bool VisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::AssociativeCollection& collection) override;

			virtual void OnAfterVisitClass(const Reflection::Property* property, void* data) override;
			virtual void OnAfterVisitClass(const Property* property, void* data, const CommonAttribute::IndexedCollection& collection) override;
			virtual void OnAfterVisitClass(const Property* property, void* data, const CommonAttribute::AssociativeCollection& collection) override;

		private:
			static const Reflection::Property* FindSimilarProperty(PropertyNode& propertyNode, const Reflection::Property* property);

			template<typename T>
			void VisitPrimitive(const Reflection::Property* property, T& data);

			bool IsRootNode(const Reflection::Property* property) const;
			const PropertyNode& GetTopPropertyNode() const;
			PropertyNode& GetTopPropertyNode();
			void PushPropertyNode(const Property* property, void* address);
			void PushPropertyNode(const Property* property);
			void PopPropertyNode();

			PropertyDiffType GetPropertyNotFoundType(const PropertyNode& propertyNode, bool source = false); // true = source, false = target

		public:
			MulticastDelegate<void(const PropertyDiffInfo&)> OnPropertyDiffFound;

		private:
			Reflection::Property m_RootProperty;
			std::stack<PropertyNode> m_TargetsStack;

		};

	}
}