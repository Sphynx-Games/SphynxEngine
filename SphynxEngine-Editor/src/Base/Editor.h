#pragma once

#include "Base/Widget.h"
#include <vector>
#include <string>
#include <Core/Delegate.h>


namespace Sphynx
{
	class Event;
	class Toolbar;
	class OperationManager;

	class Editor : public Widget
	{
	public:
		Editor(const char* name, Widget* parent = nullptr);
		virtual ~Editor();

	public:
		void SetToolbar(Toolbar* toolbar);
		void SetOverrideID(WidgetID id);
		void SetIsClosable(bool isClosable);

		bool GetIsClosable() const;
		bool GetShouldClose() const;

		OperationManager* GetOperationManager() const;
		void SetOperationManager(OperationManager* operationManager);

	protected:
		virtual void PreRenderGUI() override;
		virtual void RenderGUI() override;
		virtual void PostRenderGUI() override {}
		virtual void PostRenderUpdate(float deltaTime);

		// Menu bar
		virtual bool HasMenuBar() const { return false; }
		virtual void RenderMenuBar();

	public:
		MulticastDelegate<void()> OnClose;

	protected:
		Toolbar* m_Toolbar;
		WidgetID m_ID;
		bool m_StartedRendering;
		bool m_IsClosable;
		bool m_ShouldClose;

		OperationManager* m_OperationManager;
	};
}
