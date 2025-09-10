#pragma once

#include "Core/Core.h"
#include "Reflection/PropertyTree.h"


namespace Sphynx
{
	class Scene;
	class Actor;
	class Writer;

	namespace Reflection
	{
		struct Class;
	}

	class SPHYNX_API SceneSerializer : public Reflection::IPropertyTreeVisitor
	{
	public:
		SceneSerializer(const Scene& scene, Writer&& writer);

	public:
		void Serialize();

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

		virtual void OnBeforeVisitEnum(const Reflection::Property* property, void* data) override;
		virtual void OnAfterVisitEnum(const Reflection::Property* property, void* data) override;
		virtual void OnBeforeVisitClass(const Reflection::Property* property, void* data) override;
		virtual void OnAfterVisitClass(const Reflection::Property* property, void* data) override;
		virtual void OnBeforeVisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::IndexedCollection& collection) override;
		virtual void OnAfterVisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::IndexedCollection& collection) override;
		virtual void OnBeforeVisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::AssociativeCollection& collection) override;
		virtual void OnAfterVisitClass(const Reflection::Property* property, void* data, const Reflection::CommonAttribute::AssociativeCollection& collection) override;

	protected:
		const Scene& m_Scene;
		Writer& m_Writer;
	};

	namespace Utils
	{
		SPHYNX_API void ActorCoreTraversal(Reflection::PropertyTree& tree, const Reflection::Property* property, void* data, Reflection::IPropertyTreeVisitor& visitor);
		SPHYNX_API void ActorTraversal(Reflection::PropertyTree& tree, const Reflection::Property* property, void* data, Reflection::IPropertyTreeVisitor& visitor);
	}
}