#pragma once

#include "Widget.h"
#include <vector>
#include <string>

class Panel;

class Editor : public Widget
{
public:
	Editor(const std::string& name);
	virtual ~Editor();

public:
	void AddPanel(Panel* panel);
	void RemovePanel(Panel* panel);

protected:
	virtual void RenderGUI() override;

protected:
	std::vector<Panel*> m_Panels;
	std::string m_Name;

};
