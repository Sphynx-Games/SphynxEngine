#pragma once

#include "Core/Core.h"
#include "Reflection/PropertyTree.h"
#include "Serialization/Serialization.h"
#include <stack>


namespace Sphynx
{
	class SPHYNX_API ReflectionDeserializer : public Reflection::IPropertyTreeVisitor
	{
	public:
		ReflectionDeserializer(void* obj, const Reflection::Type& type, const class Reader& reader, bool visitRoot = true);
		virtual ~ReflectionDeserializer();

	public:
		virtual void Deserialize();
		void SetRootPropertyVisitEnabled(bool enable);
		bool IsRootPropertyVisitEnabled() const;

	protected:
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

		virtual void OnBeforeVisit(const Reflection::Property* property, bool& data) override;
		virtual void OnAfterVisit(const Reflection::Property* property, bool& data) override;
		virtual void OnBeforeVisit(const Reflection::Property* property, char& data) override;
		virtual void OnAfterVisit(const Reflection::Property* property, char& data) override;
		virtual void OnBeforeVisit(const Reflection::Property* property, signed char& data) override;
		virtual void OnAfterVisit(const Reflection::Property* property, signed char& data) override;
		virtual void OnBeforeVisit(const Reflection::Property* property, wchar_t& data) override;
		virtual void OnAfterVisit(const Reflection::Property* property, wchar_t& data) override;
		virtual void OnBeforeVisit(const Reflection::Property* property, short& data) override;
		virtual void OnAfterVisit(const Reflection::Property* property, short& data) override;
		virtual void OnBeforeVisit(const Reflection::Property* property, int& data) override;
		virtual void OnAfterVisit(const Reflection::Property* property, int& data) override;
		virtual void OnBeforeVisit(const Reflection::Property* property, long& data) override;
		virtual void OnAfterVisit(const Reflection::Property* property, long& data) override;
		virtual void OnBeforeVisit(const Reflection::Property* property, long long& data) override;
		virtual void OnAfterVisit(const Reflection::Property* property, long long& data) override;
		virtual void OnBeforeVisit(const Reflection::Property* property, float& data) override;
		virtual void OnAfterVisit(const Reflection::Property* property, float& data) override;
		virtual void OnBeforeVisit(const Reflection::Property* property, double& data) override;
		virtual void OnAfterVisit(const Reflection::Property* property, double& data) override;
		virtual void OnBeforeVisit(const Reflection::Property* property, long double& data) override;
		virtual void OnAfterVisit(const Reflection::Property* property, long double& data) override;
		virtual void OnBeforeVisit(const Reflection::Property* property, unsigned char& data) override;
		virtual void OnAfterVisit(const Reflection::Property* property, unsigned char& data) override;
		virtual void OnBeforeVisit(const Reflection::Property* property, unsigned short& data) override;
		virtual void OnAfterVisit(const Reflection::Property* property, unsigned short& data) override;
		virtual void OnBeforeVisit(const Reflection::Property* property, unsigned int& data) override;
		virtual void OnAfterVisit(const Reflection::Property* property, unsigned int& data) override;
		virtual void OnBeforeVisit(const Reflection::Property* property, unsigned long& data) override;
		virtual void OnAfterVisit(const Reflection::Property* property, unsigned long& data) override;
		virtual void OnBeforeVisit(const Reflection::Property* property, unsigned long long& data) override;
		virtual void OnAfterVisit(const Reflection::Property* property, unsigned long long& data) override;
		virtual void OnBeforeVisit(const Reflection::Property* property, ::std::string& data) override;
		virtual void OnAfterVisit(const Reflection::Property* property, ::std::string& data) override;
		virtual void OnBeforeVisit(const Reflection::Property* property, ::std::wstring& data) override;
		virtual void OnAfterVisit(const Reflection::Property* property, ::std::wstring& data) override;
		virtual void OnBeforeVisit(const Reflection::Property* property, ::std::filesystem::path& data) override;
		virtual void OnAfterVisit(const Reflection::Property* property, ::std::filesystem::path& data) override;

		virtual void OnBeforeVisitEnum(const Reflection::Property* property, void* data) override;
		virtual void OnAfterVisitEnum(const Reflection::Property* property, void* data) override;
		virtual void OnBeforeVisitClass(const Reflection::Property* property, void* data) override;
		virtual void OnAfterVisitClass(const Reflection::Property* property, void* data) override;
		virtual void OnBeforeVisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::IndexedCollection& collection) override;
		virtual void OnAfterVisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::IndexedCollection& collection) override;
		virtual void OnBeforeVisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::AssociativeCollection& collection) override;
		virtual void OnAfterVisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::AssociativeCollection& collection) override;

	protected:
		struct Node;
		bool IsRootProperty(const Reflection::Property* property) const;

		void OnBeforeVisitProperty(const Reflection::Property* property, bool push = true, Node* node = nullptr);
		void OnAfterVisitProperty(const Reflection::Property* property, bool pop = true);

		void PushVisitedProperty(const Reflection::Property* property);
		void PushVisitedProperty(const Node& node);
		void PopVisitedProperty();

		bool IsParentValue() const;
		bool IsParentIndexedCollection() const;
		bool IsParentAssociativeCollection() const;
		bool IsKeyToggled() const;

		size_t GetIndexFromProperty(const Reflection::Property* property) const;
		size_t GetElementIndex() const;
		size_t GetKeyValueIndex() const;
		size_t GetCollectionCount() const;

		bool ShouldContinue() const;

	protected:
		struct Node
		{
			const Reflection::Property* Property{ nullptr };
			const size_t CollectionCount{ 0 };					// Pair count in a collection property
			size_t TmpCount{ 0 };								// Used on associative collection for key/value detection
			bool ValueFound{ false };
		};

		void* m_Obj;
		const Reflection::Type& m_Type;
		const Reader& m_Reader;
		bool m_IsRootPropertyVisitEnabled;

		bool m_ShouldContinue;
		std::stack<Node> m_VisitedProperties;

	};
}
