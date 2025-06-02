#pragma once

#include <Reflection/Reflection.h>
#include <vector>


namespace Sphynx
{
	class IPropertyDrawer
	{
	public:
		virtual ~IPropertyDrawer() = default;

		virtual void Draw(const Reflection::Property& property, void* data) = 0;
		virtual const Reflection::Type& GetType() const = 0;
		static void DrawDefaultLabel(const Reflection::Property& property);
		static void DrawDefaultLabel(const char* label);

	private:
		struct RegisterImpl
		{
			static void Register(IPropertyDrawer* drawer);
			static void Unregister(IPropertyDrawer* drawer);
		};

		static void EnsureInitialized();
		static std::vector<IPropertyDrawer*>* s_Registry;

		template<typename TDerived, typename T>
		friend class PropertyDrawer;
		friend class PropertyDrawerManager;

	};


	template<typename TDerived, typename T>
	class PropertyDrawer : public IPropertyDrawer
	{
	public:
		PropertyDrawer(const PropertyDrawer&) = delete;
		PropertyDrawer& operator=(const PropertyDrawer&) = delete;

	protected:
		PropertyDrawer() { IPropertyDrawer::RegisterImpl::Register(this); }
		virtual ~PropertyDrawer() { IPropertyDrawer::RegisterImpl::Unregister(this); }

	protected:
		const Reflection::Type& GetType() const override final { return Reflection::GetType<T>(); }

	private:
		static PropertyDrawer& GetInstance()
		{
			static TDerived sInstance;
			return sInstance;
		}

	private:
		struct AutoRegister 
		{
			AutoRegister() { (void)PropertyDrawer::GetInstance(); }
		};
		inline static AutoRegister s_Register{};
	};
}
