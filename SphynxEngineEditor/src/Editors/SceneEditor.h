#pragma once

#include "Base/Editor.h"

class SceneEditor : public Editor
{
public:
	SceneEditor();
	virtual ~SceneEditor();

protected:
	class ContentBrowserPanel* m_ContentBrowserPanel;
	class ViewportPanel* m_ViewportPanel;
	class DetailsPanel* m_DetailsPanel;
};
