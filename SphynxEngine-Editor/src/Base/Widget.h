#pragma once

#include "Container/Array.h"
#include "Math/Vector.h"


namespace Sphynx
{
	class Event;

	struct WidgetAction
	{
		const char* Name;
		std::function<void(void*)> Function;
	};

	class Widget
	{
	public:
		using WidgetID = unsigned int;

		Widget(Widget* parent = nullptr);
		Widget(const char* name, Widget* parent = nullptr);
		virtual ~Widget();

	public:
		virtual void HandleEvent(Event& event);
		virtual void PreRenderGUI();
		virtual void RenderGUI();
		virtual void PostRenderGUI();
		virtual void PreRenderUpdate(float deltaTime);
		virtual void PostRenderUpdate(float deltaTime);

	public:
		void SetParent(Widget* widget);
		void AddChild(Widget* widget);
		void RemoveChild(Widget* widget);
		void AddWidget(Widget* widget);
		void RemoveWidget(Widget* widget);

		const char* GetName() const { return m_Name; }
		void SetName(const char* name) { m_Name = name; }

		Widget* GetParent() const { return m_Parent; }
		const Array<Widget*>& GetChildren() const { return m_Children; }

		WidgetID GetID() const;
		const Vector2u& GetPosition() const;
		const Vector2u& GetSize() const;
		bool IsActive() const;
		bool IsHovered() const;

	private:
		const char* m_Name;
		Widget* m_Parent;
		Array<Widget*> m_Children;

	protected:
		Vector2u m_Position;
		Vector2u m_Size;

	};
}
