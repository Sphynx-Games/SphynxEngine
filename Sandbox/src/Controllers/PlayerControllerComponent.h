#pragma once

#include "Sandbox.h"
#include <Component/ScriptComponent.h>


class CharacterComponent;

class SANDBOX_API PlayerControllerComponent : public Sphynx::ScriptComponent
{
	SPX_REFLECT_GENERATED_BODY()

public:
	PlayerControllerComponent();

	//void CheckResetGame();

	virtual void BeginPlay() override;
	virtual void Update(float deltaTime) override;

protected:
	CharacterComponent* m_characterComponent;
};

/*class StartGamePlayerController : public PlayerController
{
public:
	StartGamePlayerController();

	virtual void Update(float deltaTime) override;
};*/



#include "Reflection/Reflection.h"

SPX_REFLECT_CLASS_BEGIN(PlayerControllerComponent, SANDBOX_API)
SPX_REFLECT_INHERITANCE(Sphynx::ScriptComponent)
SPX_REFLECT_ATTRIBUTE(InternalComponent);

SPX_REFLECT_CLASS_END(PlayerControllerComponent, SANDBOX_API)