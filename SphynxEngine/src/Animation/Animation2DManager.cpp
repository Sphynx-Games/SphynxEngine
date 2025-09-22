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
		if (scene == nullptr) return;

		s_Scene = scene;
		for (auto& [animComp, spriteComp] : scene->GetTupledComponents<AnimationComponent, SpriteRendererComponent>())
		{
			if (animComp->PlayOnBegin)
			{
				animComp->m_PlaybackState = PlaybackState::PLAYING;
			}
			s_AnimationComponents.Add(animComp, {0.0f, spriteComp });
		}

		s_OnCreateAnimationHandle = scene->SubscribeOnAddComponent<AnimationComponent>(&Animation2DManager::OnCreateAnimationComponent);
		s_OnDestroyAnimationHandle = scene->SubscribeOnRemoveComponent<AnimationComponent>(&Animation2DManager::OnDestroyAnimationComponent);
		s_OnCreateSpriteHandle = scene->SubscribeOnAddComponent<SpriteRendererComponent>(&Animation2DManager::OnCreateSpriteComponent);
		s_OnDestroySpriteHandle = scene->SubscribeOnRemoveComponent<SpriteRendererComponent>(&Animation2DManager::OnDestroySpriteComponent);
	}

	void Animation2DManager::Shutdown()
	{
		if (s_Scene == nullptr) return;

		s_Scene->UnsubscribeOnAddComponent<AnimationComponent>(s_OnCreateAnimationHandle);
		s_Scene->UnsubscribeOnRemoveComponent<AnimationComponent>(s_OnDestroyAnimationHandle);
		s_Scene->UnsubscribeOnAddComponent<SpriteRendererComponent>(s_OnCreateSpriteHandle);
		s_Scene->UnsubscribeOnRemoveComponent<SpriteRendererComponent>(s_OnDestroySpriteHandle);

		/*for (auto& [animComp, data] : s_AnimationComponents)
		{
			animComp->m_PlaybackState = PlaybackState::STOPPED;
		}*/
		s_AnimationComponents.RemoveAll();
		s_Scene = nullptr;
	}

	void Animation2DManager::Update(float deltaTime)
	{
		for (auto& [component, data] : s_AnimationComponents)
		{
			// only play animations if they are looping or if they haven't ended without loop
			if (data.SpriteComponent == nullptr || component == nullptr
				|| component->m_PlaybackState == PlaybackState::PAUSED || component->m_PlaybackState == PlaybackState::STOPPED)
			{
				continue;
			}
			std::shared_ptr<Asset<Animation2D>> animationAsset = AssetManager::GetAsset<Animation2D>(component->Animation);
			if (animationAsset == nullptr || animationAsset->Asset == nullptr) continue;

			Animation2D* animation = animationAsset->Asset;
			if (!component->Loop && data.SpriteComponent->Sprite == animation->Sprites[animation->Sprites.Size() - 1])
			{
				StopAnimation(component);
				continue;
			}

			// calculate timeElapsed
			data.TimeElapsed += deltaTime;
			if (data.TimeElapsed >= component->Duration)
			{
				data.TimeElapsed -= component->Duration;
			}

			// calculate and change current sprite
			float animProgress = data.TimeElapsed / component->Duration;
			uint32_t spriteIndex = static_cast<uint32_t>(animProgress * animation->Sprites.Size());
			if (spriteIndex >= animation->Sprites.Size())
			{
				spriteIndex = animation->Sprites.Size() - 1;
			}
			data.SpriteComponent->Sprite = animation->Sprites[spriteIndex];
		}
	}

	void Animation2DManager::PlayAnimation(AnimationComponent* component)
	{
		component->m_PlaybackState = PlaybackState::PLAYING;
	}

	void Animation2DManager::PauseAnimation(AnimationComponent* component)
	{
		component->m_PlaybackState = PlaybackState::PAUSED;
	}

	void Animation2DManager::StopAnimation(AnimationComponent* component)
	{
		component->m_PlaybackState = PlaybackState::STOPPED;
		if (s_AnimationComponents.ContainsKey(component))
		{
			Animation2D* animation = AssetManager::GetAsset<Animation2D>(component->Animation)->Asset;
			AnimationData data = s_AnimationComponents[component];
			data.TimeElapsed = 0.0f;
			data.SpriteComponent->Sprite = animation->Sprites[0];
		}
	}

	void Animation2DManager::OnCreateAnimationComponent(void* component, Actor* actor)
	{
		AnimationComponent* animComp = static_cast<AnimationComponent*>(component);
		if (actor->HasComponent<SpriteRendererComponent>()) // has SpriteRendererComponent
		{
			if (animComp->PlayOnBegin)
			{
				animComp->m_PlaybackState = PlaybackState::PLAYING;
			}
			s_AnimationComponents.Add(animComp, { 0.0f, &actor->GetComponent<SpriteRendererComponent>() });
		}
		else
		{
			s_AnimationComponents.Add(animComp, { 0.0f, nullptr });
		}
	}

	void Animation2DManager::OnDestroyAnimationComponent(void* component, Actor* actor)
	{
		AnimationComponent* animComp = static_cast<AnimationComponent*>(component);
		s_AnimationComponents.Remove(animComp);
	}

	void Animation2DManager::OnCreateSpriteComponent(void* component, Actor* actor)
	{
		if (actor->HasComponent<AnimationComponent>()) // has AnimationComponent
		{
			AnimationComponent* animComp = &actor->GetComponent<AnimationComponent>();
			if (animComp->PlayOnBegin)
			{
				animComp->m_PlaybackState = PlaybackState::PLAYING;
			}
			SpriteRendererComponent* spriteComp = static_cast<SpriteRendererComponent*>(component);
			s_AnimationComponents[animComp] = { 0.0f, spriteComp };
		}
	}

	void Animation2DManager::OnDestroySpriteComponent(void* component, Actor* actor)
	{
		if (actor->HasComponent<AnimationComponent>()) // has AnimationComponent
		{
			AnimationComponent& animComp = actor->GetComponent<AnimationComponent>();
			animComp.m_PlaybackState = PlaybackState::STOPPED;
			s_AnimationComponents[&animComp] = { 0.0f, nullptr };
		}
	}

}
