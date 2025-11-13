#include "PlayerControllerComponent.h"
#include "Characters/CharacterComponent.h"
#include <Input/Input.h>
#include <Input/Keycode.h>

//#include "../../Pawns/PlayerCharacter.h"
//#include "../../App/app.h"
//#include "../../Items/Item.h"
//#include "../../../Managers/GameManager.h"
//#include "../../../Utils/GameplayStatics.h"
//#include "../../../Worlds/GameWorld.h"

using namespace Sphynx;

PlayerControllerComponent::PlayerControllerComponent():
	m_characterComponent(nullptr)
{
	//m_updateWhenPaused = true;
}

/*void PlayerControllerComponent::CheckResetGame()
{
	// when game is over, allow to press button to start again
	GameManager* gameManager = HelperFunctions::cast<GameManager>(GetWorld()->GetGameplayManager());
	if (gameManager != nullptr && gameManager->IsGameOver())
	{
		if (m_inputControl->CheckButton(XINPUT_GAMEPAD_B) || App::IsKeyPressed('P'))
		{
			GameplayStatics::GetInstance().SetCurrentWorld<GameWorld>();
		}
	}
}*/

void PlayerControllerComponent::BeginPlay()
{
	m_characterComponent = GetActor()->TryGetComponent<CharacterComponent>();

	ScriptComponent::BeginPlay();
}

void PlayerControllerComponent::Update(float deltaTime)
{
	ScriptComponent::Update(deltaTime);

	// when game is running, perform normal game actions
	// MOVEMENT MECHANICH: calculate input vector

	if (m_characterComponent == nullptr) return;

	if (Input::IsKeyDown(SPX_KEY_D))
	{
		m_characterComponent->MoveRight();
	}
	if (Input::IsKeyDown(SPX_KEY_A))
	{
		m_characterComponent->MoveLeft();
	}
	if (Input::IsKeyDown(SPX_KEY_W))
	{
		m_characterComponent->MoveUp();
	}
	if (Input::IsKeyDown(SPX_KEY_S))
	{
		m_characterComponent->MoveDown();
	}

	// ITEM MECHANICS: pick or activate item
	/*if (m_inputControl->CheckButton(XINPUT_GAMEPAD_B) || App::IsKeyPressed('P'))
	{
		IPickable* pickable = m_player->Pick(); // try pick
		if (pickable != nullptr)
		{
			m_player->AddToInventory(pickable);
		}
		else
		{
			m_player->ActivateItem();
		}
	}*/
}

/*StartPlayerControllerComponent::StartPlayerControllerComponent()
{
}

void StartPlayerControllerComponent::Update(float deltaTime)
{
	PlayerController::Update(deltaTime);

	if (m_inputControl == nullptr) return;

	// OPEN GAME WORLD
	if (m_inputControl->CheckButton(XINPUT_GAMEPAD_B) || App::IsKeyPressed('P'))
	{
		GameplayStatics::GetInstance().SetCurrentWorld<GameWorld>();
	}
}*/
