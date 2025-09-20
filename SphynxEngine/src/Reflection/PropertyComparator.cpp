#include "PropertyComparator.h"
#include "Enum.h"


namespace Sphynx
{
	namespace Reflection
	{
		namespace Utils
		{
			bool IsPropertyTypeClassOrStruct(const Reflection::Property* property)
			{
				return property->GetType().Kind == Reflection::TypeKind::CLASS || property->GetType().Kind == Reflection::TypeKind::STRUCT;
			}
		}

		PropertyComparator::PropertyComparator(void* target, const Class& reflectionClass) :
			m_RootProperty(reflectionClass, reflectionClass.Name, 0),
			m_TargetsStack()
		{
			SPX_CORE_ASSERT(target != nullptr, "PropertyComparator target is nullptr!!");
			PushPropertyNode(&m_RootProperty, target);
		}

		void PropertyComparator::Visit(const Reflection::Property* property, bool& data)
		{
			VisitPrimitive(property, data);
		}

		void PropertyComparator::Visit(const Reflection::Property* property, char& data)
		{
			VisitPrimitive(property, data);
		}

		void PropertyComparator::Visit(const Reflection::Property* property, signed char& data)
		{
			VisitPrimitive(property, data);
		}

		void PropertyComparator::Visit(const Reflection::Property* property, wchar_t& data)
		{
			VisitPrimitive(property, data);
		}

		void PropertyComparator::Visit(const Reflection::Property* property, short& data)
		{
			VisitPrimitive(property, data);
		}

		void PropertyComparator::Visit(const Reflection::Property* property, int& data)
		{
			VisitPrimitive(property, data);
		}

		void PropertyComparator::Visit(const Reflection::Property* property, long& data)
		{
			VisitPrimitive(property, data);
		}

		void PropertyComparator::Visit(const Reflection::Property* property, long long& data)
		{
			VisitPrimitive(property, data);
		}

		void PropertyComparator::Visit(const Reflection::Property* property, float& data)
		{
			VisitPrimitive(property, data);
		}

		void PropertyComparator::Visit(const Reflection::Property* property, double& data)
		{
			VisitPrimitive(property, data);
		}

		void PropertyComparator::Visit(const Reflection::Property* property, long double& data)
		{
			VisitPrimitive(property, data);
		}

		void PropertyComparator::Visit(const Reflection::Property* property, unsigned char& data)
		{
			VisitPrimitive(property, data);
		}

		void PropertyComparator::Visit(const Reflection::Property* property, unsigned short& data)
		{
			VisitPrimitive(property, data);
		}

		void PropertyComparator::Visit(const Reflection::Property* property, unsigned int& data)
		{
			VisitPrimitive(property, data);
		}

		void PropertyComparator::Visit(const Reflection::Property* property, unsigned long& data)
		{
			VisitPrimitive(property, data);
		}

		void PropertyComparator::Visit(const Reflection::Property* property, unsigned long long& data)
		{
			VisitPrimitive(property, data);
		}

		void PropertyComparator::Visit(const Reflection::Property* property, ::std::string& data)
		{
			VisitPrimitive(property, data);
		}

		void PropertyComparator::Visit(const Reflection::Property* property, ::std::wstring& data)
		{
			VisitPrimitive(property, data);
		}

		void PropertyComparator::Visit(const Reflection::Property* property, ::std::filesystem::path& data)
		{
			VisitPrimitive(property, data);
		}

		void PropertyComparator::VisitEnum(const Reflection::Property* property, void* data)
		{
			const Reflection::Property* foundProperty = FindSimilarProperty(GetTopPropertyNode(), property);
			if (foundProperty == nullptr)
			{
				OnPropertyDiffFound.Broadcast({ property, data, nullptr, nullptr, GetPropertyNotFoundType(GetTopPropertyNode()), m_TargetsStack });
				return;
			}

			const Reflection::Enum& rEnum = static_cast<const Reflection::Enum&>(property->GetType());
			void* targetAddress = reinterpret_cast<std::byte*>(GetTopPropertyNode().Address) + foundProperty->Offset;
			if (rEnum.GetValue((const void*)targetAddress) != rEnum.GetValue((const void*)data))
			{
				OnPropertyDiffFound.Broadcast({ property, data, foundProperty, targetAddress, PropertyDiffType::DIFFERENT_VALUE, m_TargetsStack });
			}
		}

		bool PropertyComparator::VisitClass(const Reflection::Property* property, void* data)
		{
			if (IsRootNode(property))
			{
				return true;
			}

			const Reflection::Property* foundProperty = FindSimilarProperty(GetTopPropertyNode(), property);
			if (foundProperty == nullptr)
			{
				OnPropertyDiffFound.Broadcast({ property, data, nullptr, nullptr, GetPropertyNotFoundType(GetTopPropertyNode()), m_TargetsStack });
				return false;
			}

			PushPropertyNode(foundProperty);

			return true;
		}

		bool PropertyComparator::VisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::IndexedCollection& collection)
		{
			if (!IsRootNode(property))
			{
				const Reflection::Property* foundProperty = FindSimilarProperty(GetTopPropertyNode(), property);
				if (foundProperty == nullptr)
				{
					OnPropertyDiffFound.Broadcast({ property, data, nullptr, nullptr, GetPropertyNotFoundType(GetTopPropertyNode()), m_TargetsStack });
					return false;
				}

				PushPropertyNode(foundProperty);
			}

			PropertyNode& propertyNode = GetTopPropertyNode();
			const size_t size = collection.GetSize(propertyNode.Address);
			propertyNode.CollectionProperties.reserve(size);
			propertyNode.IndexedCollectionPropertyNames.reserve(size);

			const std::byte* collectionAddress = (std::byte*)propertyNode.Address;
			for (size_t i = 0; i < size; ++i)
			{
				const std::string& name = propertyNode.IndexedCollectionPropertyNames.emplace_back(std::to_string(i));
				const std::byte* elemAddress = (std::byte*)collection.Get(propertyNode.Address, i);
				propertyNode.CollectionProperties.emplace_back(collection.GetValueType(), name.c_str(), std::distance(collectionAddress, elemAddress));
			}

			return true;
		}

		bool PropertyComparator::VisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::AssociativeCollection& collection)
		{
			if (!IsRootNode(property))
			{
				const Reflection::Property* foundProperty = FindSimilarProperty(GetTopPropertyNode(), property);
				if (foundProperty == nullptr)
				{
					OnPropertyDiffFound.Broadcast({ property, data, nullptr, nullptr, GetPropertyNotFoundType(GetTopPropertyNode()), m_TargetsStack });
					return false;
				}

				PushPropertyNode(foundProperty);
			}

			PropertyNode& propertyNode = GetTopPropertyNode();
			propertyNode.VisitingKey = true;
			propertyNode.CollectionAddress = data;
			const size_t size = collection.GetSize(propertyNode.Address);
			propertyNode.CollectionProperties.reserve(size * 2);

			const std::byte* collectionAddress = (std::byte*)propertyNode.Address;
			for (size_t i = 0; i < size; ++i)
			{
				const std::byte* keyAddress = (std::byte*)collection.GetKey(propertyNode.Address, i);
				const std::byte* valueAddress = (std::byte*)collection.GetValue(propertyNode.Address, i);
				propertyNode.CollectionProperties.emplace_back(collection.GetKeyType(), collection.GetKeyType().Name, std::distance(collectionAddress, keyAddress));
				propertyNode.CollectionProperties.emplace_back(collection.GetValueType(), collection.GetValueType().Name, std::distance(collectionAddress, valueAddress));
			}

			return true;
		}

		void PropertyComparator::OnAfterVisitClass(const Reflection::Property* property, void* data)
		{
			if (IsRootNode(property))
			{
				return;
			}
			PopPropertyNode();
		}

		void PropertyComparator::OnAfterVisitClass(const Property* property, void* data, const CommonAttribute::IndexedCollection& collection)
		{
			// check if there are any additional properties not found visited
			PropertyNode& propertyNode = GetTopPropertyNode();
			const void* sourceCollectionAddress = (std::byte*)data + property->Offset;
			void* targetCollectionAddress = propertyNode.Address;
			const size_t sourceSize = collection.GetSize(sourceCollectionAddress);
			const size_t targetSize = collection.GetSize(targetCollectionAddress);

			for (size_t i = sourceSize; i < targetSize; ++i)
			{
				OnPropertyDiffFound.Broadcast({ nullptr, nullptr, &propertyNode.CollectionProperties[i], collection.Get(targetCollectionAddress, i), PropertyDiffType::SOURCE_VALUE_PROPERTY_NOT_FOUND, m_TargetsStack });
			}

			if (IsRootNode(property))
			{
				return;
			}
			PopPropertyNode();
		}

		void PropertyComparator::OnAfterVisitClass(const Property* property, void* data, const CommonAttribute::AssociativeCollection& collection)
		{
			// check if there are any additional properties not found visited
			PropertyNode& propertyNode = GetTopPropertyNode();
			const void* sourceCollectionAddress = (std::byte*)data + property->Offset;
			void* targetCollectionAddress = propertyNode.Address;
			const size_t targetSize = collection.GetSize(targetCollectionAddress);

			for (size_t i = 0; i < targetSize; ++i)
			{
				if (!collection.ContainsKey(sourceCollectionAddress, collection.GetKey(targetCollectionAddress, i)))
				{
					OnPropertyDiffFound.Broadcast({ nullptr, nullptr, &propertyNode.CollectionProperties[i], collection.GetKey(targetCollectionAddress, i), PropertyDiffType::SOURCE_KEY_PROPERTY_NOT_FOUND, m_TargetsStack });
				}
			}

			if (IsRootNode(property))
			{
				return;
			}
			PopPropertyNode();
		}

		const PropertyNode& PropertyComparator::GetTopPropertyNode() const
		{
			return m_TargetsStack[m_TargetsStack.Size() - 1];
		}

		PropertyNode& PropertyComparator::GetTopPropertyNode()
		{
			return m_TargetsStack[m_TargetsStack.Size() - 1];
		}

		bool PropertyComparator::IsRootNode(const Reflection::Property* property) const
		{
			return property->Name == m_RootProperty.Name;
		}

		void PropertyComparator::PushPropertyNode(const Property* property)
		{
			PushPropertyNode(property, GetTopPropertyNode().Address);
		}

		void PropertyComparator::PushPropertyNode(const Property* property, void* address)
		{
			std::byte* targetBytePointer = reinterpret_cast<std::byte*>(address);
			targetBytePointer += property->Offset;
			void* targetAddress = reinterpret_cast<void*>(targetBytePointer);

			PropertyNode::NodeType nodeType = PropertyNode::NodeType::NonCollection;
			if (Utils::IsPropertyTypeClassOrStruct(property))
			{
				const Reflection::Class& reflectionClass = static_cast<const Reflection::Class&>(property->GetType());
				if (reflectionClass.HasAttribute<CommonAttribute::IndexedCollection>()) nodeType = PropertyNode::NodeType::IndexedCollection;
				else if (reflectionClass.HasAttribute<CommonAttribute::AssociativeCollection>()) nodeType = PropertyNode::NodeType::AssociativeCollection;
			}

			m_TargetsStack.Add({ property, targetAddress, nodeType });
		}

		void PropertyComparator::PopPropertyNode()
		{
			m_TargetsStack.RemoveAt(m_TargetsStack.Size() - 1);
		}

		PropertyDiffType PropertyComparator::GetPropertyNotFoundType(const PropertyNode& propertyNode, bool source)
		{
			if (propertyNode.VisitingKey)
			{
				return source ? PropertyDiffType::SOURCE_KEY_PROPERTY_NOT_FOUND : PropertyDiffType::TARGET_KEY_PROPERTY_NOT_FOUND;
			}
			else
			{
				return source ? PropertyDiffType::SOURCE_VALUE_PROPERTY_NOT_FOUND : PropertyDiffType::TARGET_VALUE_PROPERTY_NOT_FOUND;
			}
		}

		const Reflection::Property* PropertyComparator::FindSimilarProperty(PropertyNode& propertyNode, const Reflection::Property* property)
		{
			SPX_CORE_ASSERT(Utils::IsPropertyTypeClassOrStruct(propertyNode.Property), "PropertyNode Type must be a class or a struct to perform this operation!!");

			const Reflection::Class& reflectionClass = static_cast<const Reflection::Class&>(propertyNode.Property->GetType());

			if (propertyNode.Type == PropertyNode::NonCollection)
			{
				for (const Reflection::Property& p : reflectionClass)
				{
					if (&p.GetType() == &property->GetType()
						&& p.Name == property->Name
						&& p.QualifiedType.ValueType == property->QualifiedType.ValueType
						&& p.Offset == property->Offset
						&& p.QualifiedType.Qualifiers == property->QualifiedType.Qualifiers
						&& p.GetPointerIndirection() == property->GetPointerIndirection())
					{
						return &p;
					}
				}
			}
			else if (propertyNode.Type == PropertyNode::IndexedCollection)
			{
				for (const Reflection::Property& p : propertyNode.CollectionProperties)
				{
					if (&p.GetType() == &property->GetType()
						&& !std::strcmp(p.Name, property->Name)
						&& p.QualifiedType.ValueType == property->QualifiedType.ValueType
						&& p.QualifiedType.Qualifiers == property->QualifiedType.Qualifiers
						&& p.GetPointerIndirection() == property->GetPointerIndirection())
					{
						return &p;
					}
				}
			}
			else if (propertyNode.Type == PropertyNode::AssociativeCollection)
			{
				const Reflection::Property* result = nullptr;

				const CommonAttribute::AssociativeCollection* collection = reflectionClass.GetAttribute<CommonAttribute::AssociativeCollection>();
				SPX_CORE_ASSERT(collection != nullptr, "Associative Collection attribute expected in collection class");

				if (propertyNode.VisitingKey)
				{
					propertyNode.ValueProperty = nullptr;

					const void* collectionAddress = propertyNode.CollectionAddress; // source
					for (size_t i = 0; i < propertyNode.CollectionProperties.size(); i += 2)
					{
						const Reflection::Property& keyProperty = propertyNode.CollectionProperties[i];
						if (&keyProperty.GetType() == &property->GetType() &&
							keyProperty.Name == property->Name)
						{
							const bool keyFound = collection->CompareKeys(
								(std::byte*)collectionAddress + property->Offset,
								(std::byte*)propertyNode.Address + keyProperty.Offset
							);

							if (keyFound)
							{
								propertyNode.ValueProperty = &propertyNode.CollectionProperties[i + 1];
								result = &keyProperty;
								break;
							}
						}
					}
				}
				else
				{
					result = propertyNode.ValueProperty != nullptr ? propertyNode.ValueProperty : nullptr;
				}

				propertyNode.VisitingKey = !propertyNode.VisitingKey;
				return result;
			}

			return nullptr;
		}

		template<typename T>
		void PropertyComparator::VisitPrimitive(const Reflection::Property* property, T& data)
		{
			const Reflection::Property* foundProperty = FindSimilarProperty(GetTopPropertyNode(), property);
			if (foundProperty == nullptr)
			{
				OnPropertyDiffFound.Broadcast({ property, &data, nullptr, nullptr, GetPropertyNotFoundType(GetTopPropertyNode()), m_TargetsStack });
				return;
			}
			std::byte* targetBytePointer = reinterpret_cast<std::byte*>(GetTopPropertyNode().Address);
			targetBytePointer += foundProperty->Offset;
			T* value = reinterpret_cast<T*>(targetBytePointer);
			if (*value != data)
			{
				OnPropertyDiffFound.Broadcast({ property, &data, foundProperty, GetTopPropertyNode().Address, PropertyDiffType::DIFFERENT_VALUE, m_TargetsStack });
			}
		}
	}
}