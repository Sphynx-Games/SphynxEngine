#pragma once

#include "Sandbox.h"
#include <Component/ScriptComponent.h>

namespace Sphynx
{
	struct BoxCollider2DComponent;
	struct SpriteRendererComponent;
}
class MovementComponent;

class SANDBOX_API CharacterComponent : public Sphynx::ScriptComponent
{
	SPX_REFLECT_GENERATED_BODY()

public:
	CharacterComponent();

	virtual void MoveRight();
	virtual void MoveLeft();
	virtual void MoveUp();
	virtual void MoveDown();

	virtual void BeginPlay() override;
	virtual void EndPlay() override;

/*private:
	void OnSpriteChanged();*/

protected:
	Sphynx::BoxCollider2DComponent* m_collider;
	Sphynx::SpriteRendererComponent* m_spriteRenderer;
	MovementComponent* m_movementComponent;

	Sphynx::DelegateHandle m_spriteChangedHandle;
};


#include "Reflection/Reflection.h"

SPX_REFLECT_CLASS_BEGIN(CharacterComponent, SANDBOX_API)
SPX_REFLECT_INHERITANCE(Sphynx::ScriptComponent)
SPX_REFLECT_ATTRIBUTE(InternalComponent);

SPX_REFLECT_CLASS_END(CharacterComponent, SANDBOX_API)