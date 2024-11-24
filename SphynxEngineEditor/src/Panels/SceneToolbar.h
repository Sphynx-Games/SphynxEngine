#pragma once

#include "Base/Panel.h"
#include "Utils/PlaybackState.h"


namespace Sphynx
{
	class ButtonWidget;

	class SceneToolbar : public Panel
	{
	public:
		SceneToolbar();

	public:
		ButtonWidget* GetSaveButton() const { return m_SaveButton; }
		ButtonWidget* GetPlayAndPauseButton() const { return m_PlayAndPauseButton; }
		ButtonWidget* GetStopButton() const { return m_StopButton; }

		void SetPlaybackState(PlaybackState state);

	protected:
		virtual void RenderGUI() override;
	
	private:
		ButtonWidget* m_SaveButton;
		ButtonWidget* m_PlayAndPauseButton;
		ButtonWidget* m_StopButton;
		//char m_SceneNameBuffer[1024];
	};
}
