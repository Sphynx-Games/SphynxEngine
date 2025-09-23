#pragma once

#include "Base/Editor.h"
#include "EditorCameraController.h"


namespace Sphynx
{
	namespace Reflection
	{
		class Class;
	}
	class Event;


	class PropertyEditor : public Editor
	{
	public:
		PropertyEditor(const Reflection::Class& reflectionClass, void* object);
		virtual ~PropertyEditor();

	protected:
		virtual void RenderGUI() override;

		// Menu bar
		virtual bool HasMenuBar() const override { return true; }
		virtual void RenderMenuBar() override;
	
	private:
		void SaveObject();

	private:
		const Reflection::Class& m_ReflectionClass;
		void* m_Object;
	};
}