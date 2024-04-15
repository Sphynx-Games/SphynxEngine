#pragma once

#include <Sphynx.h>

class Widget;

class EditorLayer : public Sphynx::Layer
{
public:
	EditorLayer();
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
	Sphynx::Framebuffer* m_Framebuffer;
	bool m_BlockEventsEnabled;
	std::vector<Widget*> m_Widgets;

	// TODO: remove
public:
	static Sphynx::Framebuffer* s_Framebuffer;
private:
	std::unique_ptr<class Sphynx::OrthographicCameraController> m_CameraController;
	Sphynx::Scene m_Scene;

};
