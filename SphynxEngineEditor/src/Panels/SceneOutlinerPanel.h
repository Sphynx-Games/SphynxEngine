#pragma once

#include "Base/Panel.h"

#include <Scene/Actor.h>
#include <Container/Array.h>

#include <type_traits>


namespace Sphynx
{
	class Scene;

	class SceneOutlinerPanel : public Panel
	{
	public:
		SceneOutlinerPanel(Scene* scene = nullptr, Widget* parent = nullptr);

	public:
		void SetContext(Scene* scene);
		const Actor& GetSelectedActor() const;

	protected:
		virtual void RenderGUI() override;

	private:
		void RenderActorGUI(const Actor& actor);
		void RenderActorGUI_ContextMenu(const Actor& actor);

		void DeleteActor(const Actor& actor);

	private:
		Scene* m_Scene;
		Actor m_SelectedActor;
		Array<std::reference_wrapper<const Actor>> m_ActorsToDeleted;
	};
}
