#pragma once

#include "Base/Panel.h"


namespace Sphynx
{
	class Scene;
	class ButtonWidget;

	class SceneOptionsPanel : public Panel
	{
	public:
		SceneOptionsPanel(const Scene* scene = nullptr);

	public:
		void SetContext(const Scene* scene);
		class ButtonWidget* GetSaveButton() const { return m_SaveButton; }
		class ButtonWidget* GetPlayButton() const { return m_PlayButton; }

	protected:
		virtual void RenderGUI() override;
	
	private:
		const Scene* m_Scene;
		ButtonWidget* m_SaveButton;
		ButtonWidget* m_PlayButton;
		//char m_SceneNameBuffer[1024];
	};
}
