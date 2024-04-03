#pragma once

#include <Sphynx.h>

class Widget;

class EditorLayer : public Sphynx::Layer
{
public:
	using Sphynx::Layer::Layer;
	virtual ~EditorLayer();

	virtual void Attach() override;
	virtual void Detach() override;
	virtual void Update(float deltaTime) override;

	virtual void Begin() = 0;
	virtual void End() = 0;
	virtual void RenderGUI();

	bool IsBlockEventsEnabled() const;
	void SetBlockEventsEnabled(bool enabled);

	void AddWidget(Widget* widget);
	void RemoveWidget(Widget* widget);

public:
	static EditorLayer* Create();
	
protected:
	bool m_BlockEventsEnabled = false;
	std::vector<Widget*> m_Widgets;

};
