#pragma once

#include "Base/Toolbar.h"
#include "Utils/PlaybackState.h"


namespace Sphynx
{
	class ButtonWidget;

	class SceneToolbar : public Toolbar
	{
	public:
		SceneToolbar();

	public:
		ButtonWidget* GetSaveButton() const { return m_SaveButton; }
		ButtonWidget* GetPlayAndPauseButton() const { return m_PlayAndPauseButton; }
		ButtonWidget* GetStopButton() const { return m_StopButton; }

		void SetPlaybackState(PlaybackState state);
	
	private:
		ButtonWidget* m_SaveButton;
		ButtonWidget* m_PlayAndPauseButton;
		ButtonWidget* m_StopButton;
		//char m_SceneNameBuffer[1024];
	};
}
