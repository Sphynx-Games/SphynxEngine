#pragma once

#include "Base/Panel.h"

#include <Scene/Actor.h>


namespace Sphynx
{
	class Scene;

	class SceneOutlinerPanel : public Panel
	{
	public:
		SceneOutlinerPanel(const Scene* scene = nullptr);

	public:
		void SetContext(const Scene* scene);
		const Actor& GetSelectedActor() const;

	protected:
		virtual void RenderGUI() override;

	private:
		void RenderActorGUI(const Actor& actor);

	private:
		const Scene* m_Scene;
		Actor m_SelectedActor;
	};
}
