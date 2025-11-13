#include "PlayerCharacterComponent.h"
/*#include "../../Components/Pick/PickComponent.h"
#include "../../Components/Sprites/SpriteRendererComponent.h"
#include "../../Components/Sprites/Animations/PlayerAnimComponent.h"
#include "../../Components/Movement/MovementComponent.h"
#include "../../Components/Scene/SceneComponent.h"
#include "../../Components/SFX/SFXComponent.h"
#include "../Items/Item.h"
#include "../Items/ItemData.h"
#include "../../Assets.h"*/

PlayerCharacterComponent::PlayerCharacterComponent()/* :
	m_pickComponent(AddComponent<PickComponent>()),
	m_item(nullptr)*/
{
	/*m_animationComponent = AddComponent<PlayerAnimComponent>();
	m_movementComponent->SetSpeed(220.0f);*/
}

//IPickable* PlayerCharacterComponent::Pick()
//{
//	if (m_pickComponent != nullptr)
//	{
//		return m_pickComponent->Pick();
//	}
//	return nullptr;
//}
//
//void PlayerCharacterComponent::AddToInventory(IPickable* pickable)
//{
//	Item* item = HelperFunctions::cast<Item>(pickable);
//	if (item != nullptr)
//	{
//		if (m_item != nullptr)
//		{
//			m_item->Destroy();
//		}
//		ItemData* itemData = item->GrantItem(this);
//		AddComponent(itemData);
//		m_item = itemData;
//	}
//}
//
//void PlayerCharacterComponent::RemoveFromInventory(IPickable* pickable)
//{
//	Component* c = HelperFunctions::cast<Component>(pickable);
//	if (c != nullptr && HelperFunctions::contains(m_components, c))
//	{
//		HelperFunctions::remove(m_components, c);
//	}
//}
//
//void PlayerCharacterComponent::ActivateItem()
//{
//	if (m_item != nullptr)
//	{
//		m_item->Activate();
//	}
//}

/*void PlayerCharacterComponent::BeginPlay()
{
	CharacterComponent::BeginPlay();

	m_spriteRenderer->SetSpriteAsset(Assets::GetInstance().playerSpriteAsset);
	m_animationComponent->SetAnimationAsset(Assets::GetInstance().playerAnimationAsset);
	m_rootComponent->SetScale(0.6f);
}

void PlayerCharacterComponent::EndPlay()
{
	m_pickComponent = nullptr;
	m_item = nullptr;

	CharacterComponent::EndPlay();
}*/