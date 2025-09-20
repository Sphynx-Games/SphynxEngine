#include "spxpch.h"
#include "Animation2DManager.h"
#include "Scene/Scene.h"
#include "Component/AnimationComponent.h"
#include "Component/SpriteRendererComponent.h"
#include "Asset/AssetManager.h"
#include "Asset/Animation/AnimationAsset.h"


namespace Sphynx
{
	void Animation2DManager::Init(Scene* scene)
	{
		for (auto& [animation, sprite] : scene->GetTupledComponents<AnimationComponent, SpriteRendererComponent>())
		{
			s_AnimationComponents.Add(animation, {0.0f, sprite});
		}
	}

	void Animation2DManager::Shutdown()
	{
		s_AnimationComponents.RemoveAll();
	}

	void Animation2DManager::Update(float deltaTime)
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
			Animation2D* animation = AssetManager::GetAsset<Animation2D>(component->Animation)->Asset;
			uint32_t spriteIndex = static_cast<uint32_t>(animProgress * animation->Sprites.Size());
			if (spriteIndex >= animation->Sprites.Size())
			{
				spriteIndex = animation->Sprites.Size() - 1;
			}
			data.SpriteComponent->Sprite = animation->Sprites[spriteIndex];
		}
	}
}
