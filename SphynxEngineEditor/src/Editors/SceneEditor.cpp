#include "SceneEditor.h"

#include "Panels/ContentBrowserPanel.h"
#include "Panels/ViewportPanel.h"
#include "Panels/DetailsPanel.h"


SceneEditor::SceneEditor() : 
	Editor("SceneEditor"),
	m_ContentBrowserPanel(new ContentBrowserPanel()),
	m_ViewportPanel(new ViewportPanel()),
	m_DetailsPanel(new DetailsPanel())
{
	AddPanel(m_ContentBrowserPanel);
	AddPanel(m_ViewportPanel);
	AddPanel(m_DetailsPanel);
}

SceneEditor::~SceneEditor()
{
}
