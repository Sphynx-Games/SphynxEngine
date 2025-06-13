#pragma once

#include <Reflection/PropertyTree.h>


namespace Sphynx
{
	class PropertyViewer : public Reflection::IPropertyTreeVisitor
	{
	public:
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

		virtual void OnBeforeVisitEnum(const Reflection::Property* property, void* data) override;
		virtual void OnAfterVisitEnum(const Reflection::Property* property, void* data) override;
		virtual void OnBeforeVisitClass(const Reflection::Property* property, void* data) override;
		virtual void OnAfterVisitClass(const Reflection::Property* property, void* data) override;

	};
}

