#include "spxpch.h"
#include "Animation2D.h"
#include "Scene/Scene.h"
#include "Component/AnimationComponent.h"
#include "Component/SpriteRendererComponent.h"
#include "Asset/AssetManager.h"


namespace Sphynx
{
	void Animation2D::Init(Scene* scene)
	{
		for (auto& [animation, sprite] : scene->GetTupledComponents<AnimationComponent, SpriteRendererComponent>())
		{
			s_AnimationComponents.Add(animation, {0.0f, sprite});
		}
	}

	void Animation2D::Shutdown()
	{
		s_AnimationComponents.RemoveAll();
	}

	void Animation2D::Update(float deltaTime)
	{
		for (auto& [component, data] : s_AnimationComponents)
		{
			if (component == nullptr) continue;

			// calculate timeElapsed
			data.TimeElapsed += deltaTime;
			if (data.TimeElapsed >= component->Duration)
			{
				data.TimeElapsed -= component->Duration;
			}

			// calculate and change current sprite
			float animProgress = data.TimeElapsed / component->Duration;
			uint32_t spriteIndex = static_cast<uint32_t>(animProgress * component->Sprites.Size());
			if (spriteIndex >= component->Sprites.Size())
			{
				spriteIndex = component->Sprites.Size() - 1;
			}
			data.SpriteComponent->Sprite = component->Sprites[spriteIndex];
		}
	}
}
