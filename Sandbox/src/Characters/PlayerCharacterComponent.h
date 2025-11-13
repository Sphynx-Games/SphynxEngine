#pragma once

#include "Sandbox.h"
#include <Component/ScriptComponent.h>
#include "CharacterComponent.h"

class IPickable;
class PickComponent;
//class ItemData;

class SANDBOX_API PlayerCharacterComponent : public CharacterComponent
{
	SPX_REFLECT_GENERATED_BODY()

public:
	PlayerCharacterComponent();

	IPickable* Pick();
	//void AddToInventory(IPickable* pickable);
	//void RemoveFromInventory(IPickable* pickable);
	//void ActivateItem();

	//virtual void BeginPlay() override;
	//virtual void EndPlay() override;

private:
	//PickComponent* m_pickComponent;
	//ItemData* m_item;
};



#include "Reflection/Reflection.h"

SPX_REFLECT_CLASS_BEGIN(PlayerCharacterComponent, SANDBOX_API)
SPX_REFLECT_PARENT(CharacterComponent)
SPX_REFLECT_INHERITANCE(CharacterComponent)
SPX_REFLECT_INHERITANCE(Sphynx::ScriptComponent)
SPX_REFLECT_ATTRIBUTE(InternalComponent);

SPX_REFLECT_CLASS_END(PlayerCharacterComponent, SANDBOX_API)