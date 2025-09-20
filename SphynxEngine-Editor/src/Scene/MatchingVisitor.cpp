#include "MatchingVisitor.h"


namespace Sphynx
{
	MatchingVisitor::MatchingVisitor(Reflection::IPropertyTreeVisitor& visitor, HashMap<Pair<const Reflection::Property*, void*>, Array<Pair<const Reflection::Property*, void*>>> tree) :
		m_Visitor(visitor),
		m_Tree(tree)
	{
	}

	void MatchingVisitor::Visit(const Reflection::Property* property, bool& data)
	{
		if (MatchFound(property, &data)) m_Visitor.Visit(property, data);
	}

	void MatchingVisitor::Visit(const Reflection::Property* property, char& data)
	{
		if (MatchFound(property, &data)) m_Visitor.Visit(property, data);
	}

	void MatchingVisitor::Visit(const Reflection::Property* property, signed char& data)
	{
		if (MatchFound(property, &data)) m_Visitor.Visit(property, data);
	}

	void MatchingVisitor::Visit(const Reflection::Property* property, wchar_t& data)
	{
		if (MatchFound(property, &data)) m_Visitor.Visit(property, data);
	}

	void MatchingVisitor::Visit(const Reflection::Property* property, short& data)
	{
		if (MatchFound(property, &data)) m_Visitor.Visit(property, data);
	}

	void MatchingVisitor::Visit(const Reflection::Property* property, int& data)
	{
		if (MatchFound(property, &data)) m_Visitor.Visit(property, data);
	}

	void MatchingVisitor::Visit(const Reflection::Property* property, long& data)
	{
		if (MatchFound(property, &data)) m_Visitor.Visit(property, data);
	}

	void MatchingVisitor::Visit(const Reflection::Property* property, long long& data)
	{
		if (MatchFound(property, &data)) m_Visitor.Visit(property, data);
	}

	void MatchingVisitor::Visit(const Reflection::Property* property, float& data)
	{
		if (MatchFound(property, &data)) m_Visitor.Visit(property, data);
	}

	void MatchingVisitor::Visit(const Reflection::Property* property, double& data)
	{
		if (MatchFound(property, &data)) m_Visitor.Visit(property, data);
	}

	void MatchingVisitor::Visit(const Reflection::Property* property, long double& data)
	{
		if (MatchFound(property, &data)) m_Visitor.Visit(property, data);
	}

	void MatchingVisitor::Visit(const Reflection::Property* property, unsigned char& data)
	{
		if (MatchFound(property, &data)) m_Visitor.Visit(property, data);
	}

	void MatchingVisitor::Visit(const Reflection::Property* property, unsigned short& data)
	{
		if (MatchFound(property, &data)) m_Visitor.Visit(property, data);
	}

	void MatchingVisitor::Visit(const Reflection::Property* property, unsigned int& data)
	{
		if (MatchFound(property, &data)) m_Visitor.Visit(property, data);
	}

	void MatchingVisitor::Visit(const Reflection::Property* property, unsigned long& data)
	{
		if (MatchFound(property, &data)) m_Visitor.Visit(property, data);
	}

	void MatchingVisitor::Visit(const Reflection::Property* property, unsigned long long& data)
	{
		if (MatchFound(property, &data)) m_Visitor.Visit(property, data);
	}

	void MatchingVisitor::Visit(const Reflection::Property* property, ::std::string& data)
	{
		if (MatchFound(property, &data)) m_Visitor.Visit(property, data);
	}

	void MatchingVisitor::Visit(const Reflection::Property* property, ::std::wstring& data)
	{
		if (MatchFound(property, &data)) m_Visitor.Visit(property, data);
	}

	void MatchingVisitor::Visit(const Reflection::Property* property, ::std::filesystem::path& data)
	{
		if (MatchFound(property, &data)) m_Visitor.Visit(property, data);
	}

	void MatchingVisitor::VisitEnum(const Reflection::Property* property, void* data)
	{
		if (MatchFound(property, data)) m_Visitor.VisitEnum(property, data);
	}

	bool MatchingVisitor::VisitClass(const Reflection::Property* property, void* data)
	{
		return MatchFound(property, nullptr) && m_Visitor.VisitClass(property, data);
	}

	bool MatchingVisitor::VisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::IndexedCollection& collection)
	{
		return MatchFound(property, nullptr) && m_Visitor.VisitClass(property, data, collection);
	}

	bool MatchingVisitor::VisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::AssociativeCollection& collection)
	{
		return MatchFound(property, nullptr) && m_Visitor.VisitClass(property, data, collection);
	}

	void MatchingVisitor::OnBeforeVisitEnum(const Reflection::Property* property, void* data)
	{
		if (MatchFound(property, data)) m_Visitor.OnBeforeVisitEnum(property, data);
	}

	void MatchingVisitor::OnAfterVisitEnum(const Reflection::Property* property, void* data)
	{
		if (MatchFound(property, data)) m_Visitor.OnAfterVisitEnum(property, data);
	}

	void MatchingVisitor::OnBeforeVisitClass(const Reflection::Property* property, void* data)
	{
		if (MatchFound(property, nullptr)) m_Visitor.OnBeforeVisitClass(property, data);
	}

	void MatchingVisitor::OnBeforeVisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::IndexedCollection& collection)
	{
		if (MatchFound(property, nullptr)) m_Visitor.OnBeforeVisitClass(property, data, collection);
	}

	void MatchingVisitor::OnBeforeVisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::AssociativeCollection& collection)
	{
		if (MatchFound(property, nullptr)) m_Visitor.OnBeforeVisitClass(property, data, collection);
	}

	void MatchingVisitor::OnAfterVisitClass(const Reflection::Property* property, void* data)
	{
		if (MatchFound(property, nullptr)) m_Visitor.OnAfterVisitClass(property, data);
	}

	void MatchingVisitor::OnAfterVisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::IndexedCollection& collection)
	{
		if (MatchFound(property, nullptr)) m_Visitor.OnAfterVisitClass(property, data, collection);
	}

	void MatchingVisitor::OnAfterVisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::AssociativeCollection& collection)
	{
		if (MatchFound(property, nullptr)) m_Visitor.OnAfterVisitClass(property, data, collection);
	}

	bool MatchingVisitor::MatchFound(const Reflection::Property* property, void* data)
	{
		return m_Tree.ContainsKey({property, data}) || (property->Name == property->GetType().Name && !m_Tree.IsEmpty()) /*Is Root Property*/;
	}
}