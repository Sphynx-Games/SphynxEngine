#include "spxpch.h"
#include "ReflectionDeserializer.h"

#include "Serialization/Reader.h"
#include <charconv>
#include "Core/UUID.h"
#include "Reflection/TypedValue.h"
#include "Asset/Asset.h"


namespace Sphynx
{
	ReflectionDeserializer::ReflectionDeserializer(void* obj, const Reflection::Type& type, const Reader& reader, bool visitRoot) :
		m_Obj(obj),
		m_Type(type),
		m_Reader(reader),
		m_IsRootPropertyVisitEnabled(visitRoot),
		m_ShouldContinue(true),
		m_VisitedProperties()
	{

	}

	ReflectionDeserializer::~ReflectionDeserializer()
	{

	}

	void ReflectionDeserializer::Deserialize()
	{
		Reflection::PropertyTree propertyTree{ m_Type, m_Obj };
		propertyTree.Traverse(*this);
	}

	void ReflectionDeserializer::SetRootPropertyVisitEnabled(bool enable)
	{
		m_IsRootPropertyVisitEnabled = enable;
	}

	bool ReflectionDeserializer::IsRootPropertyVisitEnabled() const
	{
		return m_IsRootPropertyVisitEnabled;
	}

	void ReflectionDeserializer::Visit(const Reflection::Property* property, bool& data)
	{
		if (property->IsPointer()) return;

		if (IsRootProperty(property) && !IsRootPropertyVisitEnabled()) return;

		if (ShouldContinue()) m_Reader.Read(data);
	}

	void ReflectionDeserializer::Visit(const Reflection::Property* property, char& data)
	{
		if (property->IsPointer()) return;

		if (IsRootProperty(property) && !IsRootPropertyVisitEnabled()) return;

		if (ShouldContinue()) m_Reader.Read(data);
	}

	void ReflectionDeserializer::Visit(const Reflection::Property* property, signed char& data)
	{
		if (property->IsPointer()) return;

		if (IsRootProperty(property) && !IsRootPropertyVisitEnabled()) return;

		if (ShouldContinue()) m_Reader.Read(data);
	}

	void ReflectionDeserializer::Visit(const Reflection::Property* property, wchar_t& data)
	{
		if (property->IsPointer()) return;

		if (IsRootProperty(property) && !IsRootPropertyVisitEnabled()) return;

		if (ShouldContinue()) m_Reader.Read(data);
	}

	void ReflectionDeserializer::Visit(const Reflection::Property* property, short& data)
	{
		if (property->IsPointer()) return;

		if (IsRootProperty(property) && !IsRootPropertyVisitEnabled()) return;

		if (ShouldContinue()) m_Reader.Read(data);
	}

	void ReflectionDeserializer::Visit(const Reflection::Property* property, int& data)
	{
		if (property->IsPointer()) return;

		if (IsRootProperty(property) && !IsRootPropertyVisitEnabled()) return;

		if (ShouldContinue()) m_Reader.Read(data);
	}

	void ReflectionDeserializer::Visit(const Reflection::Property* property, long& data)
	{
		if (property->IsPointer()) return;

		if (IsRootProperty(property) && !IsRootPropertyVisitEnabled()) return;

		int32_t tmpData{ 0 };
		m_Reader.Read(tmpData);
		data = tmpData;
	}

	void ReflectionDeserializer::Visit(const Reflection::Property* property, long long& data)
	{
		if (property->IsPointer()) return;

		if (IsRootProperty(property) && !IsRootPropertyVisitEnabled()) return;

		if (ShouldContinue()) m_Reader.Read(data);
	}

	void ReflectionDeserializer::Visit(const Reflection::Property* property, float& data)
	{
		if (property->IsPointer()) return;

		if (IsRootProperty(property) && !IsRootPropertyVisitEnabled()) return;

		if (ShouldContinue()) m_Reader.Read(data);
	}

	void ReflectionDeserializer::Visit(const Reflection::Property* property, double& data)
	{
		if (property->IsPointer()) return;

		if (IsRootProperty(property) && !IsRootPropertyVisitEnabled()) return;

		if (ShouldContinue()) m_Reader.Read(data);
	}

	void ReflectionDeserializer::Visit(const Reflection::Property* property, long double& data)
	{
		if (property->IsPointer()) return;

		if (IsRootProperty(property) && !IsRootPropertyVisitEnabled()) return;

		double tmpData{ 0.0 };
		m_Reader.Read(tmpData);
		data = tmpData;
	}

	void ReflectionDeserializer::Visit(const Reflection::Property* property, unsigned char& data)
	{
		if (property->IsPointer()) return;

		if (IsRootProperty(property) && !IsRootPropertyVisitEnabled()) return;

		if (ShouldContinue()) m_Reader.Read(data);
	}

	void ReflectionDeserializer::Visit(const Reflection::Property* property, unsigned short& data)
	{
		if (property->IsPointer()) return;

		if (IsRootProperty(property) && !IsRootPropertyVisitEnabled()) return;

		if (ShouldContinue()) m_Reader.Read(data);
	}

	void ReflectionDeserializer::Visit(const Reflection::Property* property, unsigned int& data)
	{
		if (property->IsPointer()) return;

		if (IsRootProperty(property) && !IsRootPropertyVisitEnabled()) return;

		if (ShouldContinue()) m_Reader.Read(data);
	}

	void ReflectionDeserializer::Visit(const Reflection::Property* property, unsigned long& data)
	{
		if (property->IsPointer()) return;

		if (IsRootProperty(property) && !IsRootPropertyVisitEnabled()) return;

		uint32_t tmpData = 0;
		m_Reader.Read(tmpData);
		data = tmpData;
	}

	void ReflectionDeserializer::Visit(const Reflection::Property* property, unsigned long long& data)
	{
		if (property->IsPointer()) return;

		if (IsRootProperty(property) && !IsRootPropertyVisitEnabled()) return;

		if (ShouldContinue()) m_Reader.Read(data);
	}

	void ReflectionDeserializer::Visit(const Reflection::Property* property, ::std::string& data)
	{
		if (property->IsPointer()) return;

		if (IsRootProperty(property) && !IsRootPropertyVisitEnabled()) return;

		if (ShouldContinue()) m_Reader.Read(data);
	}

	void ReflectionDeserializer::Visit(const Reflection::Property* property, ::std::wstring& data)
	{
		if (property->IsPointer()) return;

		if (IsRootProperty(property) && !IsRootPropertyVisitEnabled()) return;

		if (ShouldContinue()) m_Reader.Read(data);
	}

	void ReflectionDeserializer::Visit(const Reflection::Property* property, ::std::filesystem::path& data)
	{
		if (property->IsPointer()) return;

		if (IsRootProperty(property) && !IsRootPropertyVisitEnabled()) return;

		if (ShouldContinue()) m_Reader.Read(data);
	}

	void ReflectionDeserializer::VisitEnum(const Reflection::Property* property, void* data)
	{
		if (property->IsPointer()) return;

		if (IsRootProperty(property) && !IsRootPropertyVisitEnabled()) return;

		const Reflection::Enum& rEnum = static_cast<const Reflection::Enum&>(property->GetType());
		std::string tmpData{};
		m_Reader.Read(tmpData);
		rEnum.SetName(data, tmpData);
	}

	bool ReflectionDeserializer::VisitClass(const Reflection::Property* property, void* data)
	{
		if (property->IsPointer()) return false;

		const Reflection::Class& rClass = static_cast<const Reflection::Class&>(property->GetType());

		/*
		using CustomDeserializer = ::Sphynx::Serialization::CustomDeserializer<TReader>;
		if (const CustomDeserializer* deserializer = rClass.GetAttribute<CustomDeserializer>())
		{
			deserializer->Read(data, m_Reader);
			return false;
		}
		*/

		// TODO: bring back custom deserializers
		if (Reflection::GetClass<AssetType>() == property->GetType())
		{
			AssetType* assetType = static_cast<AssetType*>(data);
			std::string str{};
			m_Reader.Read(str);
			assetType->Type = Reflection::Registry::TryGetType(str.c_str());
			return false;
		}

		if (Reflection::GetClass<UUID>() == property->GetType())
		{
			UUID* uuid = static_cast<UUID*>(data);
			std::string str{};
			m_Reader.Read(str);
			*uuid = UUID::FromString(str);
			return false;
		}

		if (m_Reader.SupportsBinary())
		{
			using POD = Reflection::CommonAttribute::POD;
			// Treat Plain Old Data as directly copyable
			if (const POD* pod = rClass.GetAttribute<POD>())
			{
				m_Reader.Read(data, rClass.Size);
				return false;
			}
		}

		return true;
	}

	bool ReflectionDeserializer::VisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::IndexedCollection& collection)
	{
		if (property->IsPointer()) return false;

		const size_t size = GetCollectionCount();
		for (size_t i = 0; i < size; ++i)
		{
			collection.Add(data);
		}

		return true;
	}

	bool ReflectionDeserializer::VisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::AssociativeCollection& collection)
	{
		if (property->IsPointer()) return false;

		// We need to visit children here as random access maps are not 
		// reliable when accessing key-value pairs by index once a key is added
		// This way we already have both key and value
		auto tmpCount = m_VisitedProperties.top().TmpCount;
		const size_t size = GetCollectionCount();
		for (size_t i = 0; i < size; ++i)
		{
			void* key = collection.GetKeyType().Alloc();
			void* value = collection.GetValueType().Alloc();

			// TODO: we might need to use the same tree we are visiting instead
			{
				Reflection::PropertyTree tree{ collection.GetKeyType(), key };
				tree.Traverse(*this);
			}
			{
				Reflection::PropertyTree tree{ collection.GetValueType(), value };
				tree.Traverse(*this);
			}

			collection.Add(data, key, value);

			collection.GetValueType().Dealloc(value);
			collection.GetKeyType().Dealloc(key);
		}
		m_VisitedProperties.top().TmpCount = tmpCount;

		return false; // avoid visiting children as we already did here
	}

	void ReflectionDeserializer::OnBeforeVisit(const Reflection::Property* property, bool& data)
	{
		if (property->IsPointer()) return;

		OnBeforeVisitProperty(property);
	}

	void ReflectionDeserializer::OnAfterVisit(const Reflection::Property* property, bool& data)
	{
		if (property->IsPointer()) return;

		OnAfterVisitProperty(property);
	}

	void ReflectionDeserializer::OnBeforeVisit(const Reflection::Property* property, char& data)
	{
		if (property->IsPointer()) return;

		OnBeforeVisitProperty(property);
	}

	void ReflectionDeserializer::OnAfterVisit(const Reflection::Property* property, char& data)
	{
		if (property->IsPointer()) return;

		OnAfterVisitProperty(property);
	}

	void ReflectionDeserializer::OnBeforeVisit(const Reflection::Property* property, signed char& data)
	{
		if (property->IsPointer()) return;

		OnBeforeVisitProperty(property);
	}

	void ReflectionDeserializer::OnAfterVisit(const Reflection::Property* property, signed char& data)
	{
		if (property->IsPointer()) return;

		OnAfterVisitProperty(property);
	}

	void ReflectionDeserializer::OnBeforeVisit(const Reflection::Property* property, wchar_t& data)
	{
		if (property->IsPointer()) return;

		OnBeforeVisitProperty(property);
	}

	void ReflectionDeserializer::OnAfterVisit(const Reflection::Property* property, wchar_t& data)
	{
		if (property->IsPointer()) return;

		OnAfterVisitProperty(property);
	}

	void ReflectionDeserializer::OnBeforeVisit(const Reflection::Property* property, short& data)
	{
		if (property->IsPointer()) return;

		OnBeforeVisitProperty(property);
	}

	void ReflectionDeserializer::OnAfterVisit(const Reflection::Property* property, short& data)
	{
		if (property->IsPointer()) return;

		OnAfterVisitProperty(property);
	}

	void ReflectionDeserializer::OnBeforeVisit(const Reflection::Property* property, int& data)
	{
		if (property->IsPointer()) return;

		OnBeforeVisitProperty(property);
	}

	void ReflectionDeserializer::OnAfterVisit(const Reflection::Property* property, int& data)
	{
		if (property->IsPointer()) return;

		OnAfterVisitProperty(property);
	}

	void ReflectionDeserializer::OnBeforeVisit(const Reflection::Property* property, long& data)
	{
		if (property->IsPointer()) return;

		OnBeforeVisitProperty(property);
	}

	void ReflectionDeserializer::OnAfterVisit(const Reflection::Property* property, long& data)
	{
		if (property->IsPointer()) return;

		OnAfterVisitProperty(property);
	}

	void ReflectionDeserializer::OnBeforeVisit(const Reflection::Property* property, long long& data)
	{
		if (property->IsPointer()) return;

		OnBeforeVisitProperty(property);
	}

	void ReflectionDeserializer::OnAfterVisit(const Reflection::Property* property, long long& data)
	{
		if (property->IsPointer()) return;

		OnAfterVisitProperty(property);
	}

	void ReflectionDeserializer::OnBeforeVisit(const Reflection::Property* property, float& data)
	{
		if (property->IsPointer()) return;

		OnBeforeVisitProperty(property);
	}

	void ReflectionDeserializer::OnAfterVisit(const Reflection::Property* property, float& data)
	{
		if (property->IsPointer()) return;

		OnAfterVisitProperty(property);
	}

	void ReflectionDeserializer::OnBeforeVisit(const Reflection::Property* property, double& data)
	{
		if (property->IsPointer()) return;

		OnBeforeVisitProperty(property);
	}

	void ReflectionDeserializer::OnAfterVisit(const Reflection::Property* property, double& data)
	{
		if (property->IsPointer()) return;

		OnAfterVisitProperty(property);
	}

	void ReflectionDeserializer::OnBeforeVisit(const Reflection::Property* property, long double& data)
	{
		if (property->IsPointer()) return;

		OnBeforeVisitProperty(property);
	}

	void ReflectionDeserializer::OnAfterVisit(const Reflection::Property* property, long double& data)
	{
		if (property->IsPointer()) return;

		OnAfterVisitProperty(property);
	}

	void ReflectionDeserializer::OnBeforeVisit(const Reflection::Property* property, unsigned char& data)
	{
		if (property->IsPointer()) return;

		OnBeforeVisitProperty(property);
	}

	void ReflectionDeserializer::OnAfterVisit(const Reflection::Property* property, unsigned char& data)
	{
		if (property->IsPointer()) return;

		OnAfterVisitProperty(property);
	}

	void ReflectionDeserializer::OnBeforeVisit(const Reflection::Property* property, unsigned short& data)
	{
		if (property->IsPointer()) return;

		OnBeforeVisitProperty(property);
	}

	void ReflectionDeserializer::OnAfterVisit(const Reflection::Property* property, unsigned short& data)
	{
		if (property->IsPointer()) return;

		OnAfterVisitProperty(property);
	}

	void ReflectionDeserializer::OnBeforeVisit(const Reflection::Property* property, unsigned int& data)
	{
		if (property->IsPointer()) return;

		OnBeforeVisitProperty(property);
	}

	void ReflectionDeserializer::OnAfterVisit(const Reflection::Property* property, unsigned int& data)
	{
		if (property->IsPointer()) return;

		OnAfterVisitProperty(property);
	}

	void ReflectionDeserializer::OnBeforeVisit(const Reflection::Property* property, unsigned long& data)
	{
		if (property->IsPointer()) return;

		OnBeforeVisitProperty(property);
	}

	void ReflectionDeserializer::OnAfterVisit(const Reflection::Property* property, unsigned long& data)
	{
		if (property->IsPointer()) return;

		OnAfterVisitProperty(property);
	}

	void ReflectionDeserializer::OnBeforeVisit(const Reflection::Property* property, unsigned long long& data)
	{
		if (property->IsPointer()) return;

		OnBeforeVisitProperty(property);
	}

	void ReflectionDeserializer::OnAfterVisit(const Reflection::Property* property, unsigned long long& data)
	{
		if (property->IsPointer()) return;

		OnAfterVisitProperty(property);
	}

	void ReflectionDeserializer::OnBeforeVisit(const Reflection::Property* property, ::std::string& data)
	{
		if (property->IsPointer()) return;

		OnBeforeVisitProperty(property);
	}

	void ReflectionDeserializer::OnAfterVisit(const Reflection::Property* property, ::std::string& data)
	{
		if (property->IsPointer()) return;

		OnAfterVisitProperty(property);
	}

	void ReflectionDeserializer::OnBeforeVisit(const Reflection::Property* property, ::std::wstring& data)
	{
		if (property->IsPointer()) return;

		OnBeforeVisitProperty(property);
	}

	void ReflectionDeserializer::OnAfterVisit(const Reflection::Property* property, ::std::wstring& data)
	{
		if (property->IsPointer()) return;

		OnAfterVisitProperty(property);
	}

	void ReflectionDeserializer::OnBeforeVisit(const Reflection::Property* property, ::std::filesystem::path& data)
	{
		if (property->IsPointer()) return;

		OnBeforeVisitProperty(property);
	}

	void ReflectionDeserializer::OnAfterVisit(const Reflection::Property* property, ::std::filesystem::path& data)
	{
		if (property->IsPointer()) return;

		OnAfterVisitProperty(property);
	}

	void ReflectionDeserializer::OnBeforeVisitEnum(const Reflection::Property* property, void* data)
	{
		if (property->IsPointer()) return;

		OnBeforeVisitProperty(property);
	}

	void ReflectionDeserializer::OnAfterVisitEnum(const Reflection::Property* property, void* data)
	{
		if (property->IsPointer()) return;

		OnAfterVisitProperty(property);
	}

	void ReflectionDeserializer::OnBeforeVisitClass(const Reflection::Property* property, void* data)
	{
		if (property->IsPointer()) return;

		OnBeforeVisitProperty(property);

		const Reflection::Class& rClass = static_cast<const Reflection::Class&>(property->GetType());
		/*
		using CustomDeserializer = ::Sphynx::Serialization::CustomDeserializer<TReader>;
		if (const CustomDeserializer* deserializer = rClass.GetAttribute<CustomDeserializer>())
		{
			return;
		}
		*/

		if (Reflection::GetClass<AssetType>() == property->GetType())
		{
			return;
		}

		if (Reflection::GetClass<UUID>() == property->GetType())
		{
			return;
		}

		if (m_Reader.SupportsBinary())
		{
			using POD = Reflection::CommonAttribute::POD;
			// Treat Plain Old Data as directly copyable
			if (const POD* pod = rClass.GetAttribute<POD>())
			{
				return;
			}
		}

		if (property->Name == property->GetType().Name)
		{
			m_Reader.PushMap();
		}

		m_Reader.PushMap();
	}

	void ReflectionDeserializer::OnAfterVisitClass(const Reflection::Property* property, void* data)
	{
		if (property->IsPointer()) return;

		const Reflection::Class& rClass = static_cast<const Reflection::Class&>(property->GetType());

		/*
		using CustomSerializer = ::Sphynx::Serialization::CustomSerializer<TWriter>;
		if (const CustomSerializer* serializer = rClass.GetAttribute<CustomSerializer>())
		{
			OnAfterVisitProperty(property);
			return;
		}
		*/

		if (Reflection::GetClass<AssetType>() == property->GetType())
		{
			OnAfterVisitProperty(property);
			return;
		}

		if (Reflection::GetClass<UUID>() == property->GetType())
		{
			OnAfterVisitProperty(property);
			return;
		}

		if (m_Reader.SupportsBinary())
		{
			using POD = Reflection::CommonAttribute::POD;
			if (const POD* pod = rClass.GetAttribute<POD>())
			{
				OnAfterVisitProperty(property);
				return;
			}
		}

		m_Reader.PopMap();

		if (property->Name == property->GetType().Name)
		{
			m_Reader.PopMap();
		}

		OnAfterVisitProperty(property);
	}

	void ReflectionDeserializer::OnBeforeVisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::IndexedCollection& collection)
	{
		if (property->IsPointer()) return;

		Node tmpNode{};
		OnBeforeVisitProperty(property, false, &tmpNode);
		const size_t size = m_Reader.PushSequence();

		PushVisitedProperty(Node{ property, size, size, tmpNode.ValueFound });
	}

	void ReflectionDeserializer::OnAfterVisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::IndexedCollection& collection)
	{
		if (property->IsPointer()) return;

		m_Reader.PopSequence();
		OnAfterVisitProperty(property);
	}

	void ReflectionDeserializer::OnBeforeVisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::AssociativeCollection& collection)
	{
		if (property->IsPointer()) return;

		Node tmpNode{};
		OnBeforeVisitProperty(property, false, &tmpNode);
		const size_t size = m_Reader.PushMap();
		PushVisitedProperty(Node{ property, size, size * 2, tmpNode.ValueFound });
	}

	void ReflectionDeserializer::OnAfterVisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::AssociativeCollection& collection)
	{
		if (property->IsPointer()) return;

		m_Reader.PopMap();
		OnAfterVisitProperty(property);
	}

	bool ReflectionDeserializer::IsRootProperty(const Reflection::Property* property) const
	{
		return property->GetType() == m_Type && property->Name == m_Type.Name;
	}

	void ReflectionDeserializer::OnBeforeVisitProperty(const Reflection::Property* property, bool push, Node* node)
	{
		if (node != nullptr)
		{
			node->Property = property;
			node->ValueFound = false;
		}

		bool valueFound = false;
		if (IsParentIndexedCollection())
		{
			const size_t index = GetIndexFromProperty(property);
			m_Reader.PushIndex(index);
			m_VisitedProperties.top().TmpCount = m_VisitedProperties.top().CollectionCount - index - 1;
		}
		else if (IsParentAssociativeCollection())
		{
			const size_t index = GetKeyValueIndex();
			if (IsKeyToggled())
			{
				m_Reader.PushKey(index);
			}
			else
			{
				m_Reader.PushValue(index);
			}
			m_VisitedProperties.top().TmpCount--;
		}
		else if (IsParentValue())
		{
			size_t index{ 0 };
			if (m_Reader.FindKey(property->Name, index))
			{
				m_Reader.PushValue(index);
				valueFound = true;
			}

			if (node != nullptr)
			{
				node->ValueFound = valueFound;
			}
		}

		if (push)
		{
			PushVisitedProperty(Node{ property, 0, 0, valueFound });
		}
	}

	void ReflectionDeserializer::OnAfterVisitProperty(const Reflection::Property* property, bool pop)
	{
		// We need to pop this to be able to correctly check the parent node
		auto node = m_VisitedProperties.top();
		m_VisitedProperties.pop();

		if (IsParentIndexedCollection())
		{
			m_Reader.PopIndex();
		}
		else if (IsParentAssociativeCollection())
		{
			if (IsKeyToggled())
			{
				m_Reader.PopKey();
			}
			else
			{
				m_Reader.PopValue();
			}
		}
		else if (IsParentValue())
		{
			if (node.ValueFound)
			{
				m_Reader.PopValue();
			}
		}

		// Push the node back in
		m_VisitedProperties.push(node);
		if (pop)
		{
			PopVisitedProperty();
		}
	}

	void ReflectionDeserializer::PushVisitedProperty(const Reflection::Property* property)
	{
		m_VisitedProperties.push({ property });
	}

	void ReflectionDeserializer::PushVisitedProperty(const Node& node)
	{
		m_ShouldContinue = IsParentValue() && node.ValueFound;
		m_VisitedProperties.push(node);
	}

	void ReflectionDeserializer::PopVisitedProperty()
	{
		m_VisitedProperties.pop();
		m_ShouldContinue = IsParentValue() && (m_VisitedProperties.empty() || m_VisitedProperties.top().ValueFound);
	}

	bool ReflectionDeserializer::IsParentValue() const
	{
		if (m_VisitedProperties.empty()) return true;

		const Reflection::Property* property = m_VisitedProperties.top().Property;
		if (property->GetType().Kind == Reflection::TypeKind::PRIMITIVE)
		{
			return true;
		}

		return !IsParentIndexedCollection() && !IsParentAssociativeCollection();
	}

	bool ReflectionDeserializer::IsParentIndexedCollection() const
	{
		if (m_VisitedProperties.empty()) return false;

		const Reflection::Property* property = m_VisitedProperties.top().Property;
		if (property->GetType().Kind == Reflection::TypeKind::PRIMITIVE || property->GetType().Kind == Reflection::TypeKind::ENUM)
		{
			return false;
		}

		return static_cast<const Reflection::Class&>(property->GetType()).GetAttribute<Reflection::CommonAttribute::IndexedCollection>() != nullptr;
	}

	bool ReflectionDeserializer::IsParentAssociativeCollection() const
	{
		if (m_VisitedProperties.empty()) return false;

		const Reflection::Property* property = m_VisitedProperties.top().Property;
		if (property->GetType().Kind == Reflection::TypeKind::PRIMITIVE || property->GetType().Kind == Reflection::TypeKind::ENUM)
		{
			return false;
		}

		return static_cast<const Reflection::Class&>(property->GetType()).GetAttribute<Reflection::CommonAttribute::AssociativeCollection>() != nullptr;
	}

	bool ReflectionDeserializer::IsKeyToggled() const
	{
		auto count = m_VisitedProperties.top().TmpCount;
		return count != 0 ? count % 2 == 0 : false;
	}

	size_t ReflectionDeserializer::GetIndexFromProperty(const Reflection::Property* property) const
	{
		size_t value = 0;
		auto [ptr, ec] = std::from_chars(property->Name, property->Name + strlen(property->Name), value);
		bool isIndex = ec == std::errc() && ptr == property->Name + strlen(property->Name);
		if (!isIndex)
		{
			return GetElementIndex();
		}
		return value;
	}

	size_t ReflectionDeserializer::GetElementIndex() const
	{
		auto count = m_VisitedProperties.top().TmpCount;
		return m_VisitedProperties.top().CollectionCount - count;
	}

	size_t ReflectionDeserializer::GetKeyValueIndex() const
	{
		auto count = m_VisitedProperties.top().TmpCount;
		count = (count + 1) / 2;
		return m_VisitedProperties.top().CollectionCount - count;
	}

	size_t ReflectionDeserializer::GetCollectionCount() const
	{
		auto count = m_VisitedProperties.top().CollectionCount;
		return count;
	}

	bool ReflectionDeserializer::ShouldContinue() const
	{
		return m_ShouldContinue || IsRootProperty(m_VisitedProperties.top().Property);
	}

}
