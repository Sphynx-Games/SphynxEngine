#pragma once

#include "Base/Panel.h"


namespace Sphynx
{
	class Scene;

	class SceneOptionsPanel : public Panel
	{
	public:
		SceneOptionsPanel(const Scene* scene = nullptr);

	public:
		void SetContext(const Scene* scene);
		const class SaveButtonWidget* GetSaveButton() const { return m_SaveButton; }

	protected:
		virtual void RenderGUI() override;
	
	private:
		const Scene* m_Scene;
		class SaveButtonWidget* m_SaveButton;
		//char m_SceneNameBuffer[1024];
	};
}
