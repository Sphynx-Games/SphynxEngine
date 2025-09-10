#pragma once

#include <Core/Core.h>
#include <Reflection/PropertyTree.h>
#include <Container/Map.h>
#include <Container/Array.h>


namespace Sphynx
{
	class MatchingVisitor : public Reflection::IPropertyTreeVisitor
	{
	public:
		MatchingVisitor(Reflection::IPropertyTreeVisitor& visitor, HashMap<Pair<const Reflection::Property*, void*>, Array<Pair<const Reflection::Property*, void*>>> tree);

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

		void VisitEnum(const Reflection::Property* property, void* data) override;
		bool VisitClass(const Reflection::Property* property, void* data) override;
		bool VisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::IndexedCollection& collection) override;
		bool VisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::AssociativeCollection& collection) override;

		void OnBeforeVisitEnum(const Reflection::Property* property, void* data) override;
		void OnAfterVisitEnum(const Reflection::Property* property, void* data) override;
		void OnBeforeVisitClass(const Reflection::Property* property, void* data) override;
		void OnAfterVisitClass(const Reflection::Property* property, void* data) override;
		void OnBeforeVisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::IndexedCollection& collection) override;
		void OnAfterVisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::IndexedCollection& collection) override;
		void OnBeforeVisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::AssociativeCollection& collection) override;
		void OnAfterVisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::AssociativeCollection& collection) override;

	private:
		bool MatchFound(const Reflection::Property* property, void* data);

	private:
		Reflection::IPropertyTreeVisitor& m_Visitor;
		HashMap<Pair<const Reflection::Property*, void*>, Array<Pair<const Reflection::Property*, void*>>> m_Tree;
	};
}