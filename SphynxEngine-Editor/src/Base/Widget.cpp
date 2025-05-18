#include "Widget.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "Events/Event.h"


namespace Sphynx
{

	Widget::Widget(Widget* parent) :
		Widget(nullptr, parent)
	{
	}

	Widget::Widget(const char* name, Widget* parent) :
		m_Name(name),
		m_Parent(parent)
	{
		SetParent(parent);
	}

	Widget::~Widget()
	{
		SetParent(nullptr);

		size_t size = m_Children.Size();
		for (size_t i = 0; i < size; ++i)
		{
			delete m_Children[size - i - 1];
		}

		m_Children.RemoveAll();
	}

	void Widget::HandleEvent(Event& event)
	{
		for (Widget* widget : m_Children)
		{
			widget->HandleEvent(event);
			if (event.IsHandled()) break;
		}
	}

	void Widget::PreRenderGUI()
	{
	}

	void Widget::RenderGUI()
	{
		for (Widget* widget : m_Children)
		{
			widget->PreRenderGUI();
			widget->RenderGUI();
			widget->PostRenderGUI();
		}
	}

	void Widget::PostRenderGUI()
	{
		m_Position = { (uint32_t)ImGui::GetItemRectMin().x, (uint32_t)ImGui::GetItemRectMin().y };
		m_Size = { (uint32_t)ImGui::GetItemRectSize().x, (uint32_t)ImGui::GetItemRectSize().y };
	}

	void Widget::PreRenderUpdate(float deltaTime)
	{
		for (Widget* widget : m_Children)
		{
			widget->PreRenderUpdate(deltaTime);
		}
	}

	void Widget::PostRenderUpdate(float deltaTime)
	{
		for (Widget* widget : m_Children)
		{
			widget->PostRenderUpdate(deltaTime);
		}
	}

	void Widget::SetParent(Widget* widget)
	{
		if (m_Parent != nullptr)
		{
			m_Parent->m_Children.Remove(this);
		}

		m_Parent = widget;

		if (m_Parent != nullptr)
		{
			m_Parent->m_Children.Add(this);
		}
	}

	void Widget::AddChild(Widget* widget)
	{
		SPX_CORE_ASSERT(widget->m_Parent == nullptr);
		widget->SetParent(this);
	}

	void Widget::RemoveChild(Widget* widget)
	{
		SPX_CORE_ASSERT(widget->m_Parent == this);
		widget->SetParent(nullptr);
	}

	void Widget::AddWidget(Widget* widget)
	{
		AddChild(widget);
	}

	void Widget::RemoveWidget(Widget* widget)
	{
		RemoveChild(widget);
	}

	Widget::WidgetID Widget::GetID() const
	{
		return ImGui::GetIDWithSeed(GetName(), nullptr, m_Parent != nullptr ? m_Parent->GetID() : 0);
	}

	const Vector2u& Widget::GetPosition() const
	{
		return m_Position;
	}

	const Vector2u& Widget::GetSize() const
	{
		return m_Size;
	}

	bool Widget::IsActive() const
	{
		return GetID() == ImGui::GetActiveID();
	}

	bool Widget::IsHovered() const
	{
		return GetID() == ImGui::GetHoveredID();
	}

}
