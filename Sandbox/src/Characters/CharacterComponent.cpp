#include "CharacterComponent.h"
#include <Component/Physics/BoxCollider2DComponent.h>
#include <Component/SpriteRendererComponent.h>
#include "MovementComponent.h"

using namespace Sphynx;

CharacterComponent::CharacterComponent() :
	m_collider(nullptr),
	m_spriteRenderer(nullptr),
	m_movementComponent(nullptr)
{
}

void CharacterComponent::MoveRight()
{
	m_movementComponent->AddInputVector(Vector2f(1.0f, 0.0f));
}

void CharacterComponent::MoveLeft()
{
	m_movementComponent->AddInputVector(Vector2f(-1.0f, 0.0f));
}

void CharacterComponent::MoveUp()
{
	m_movementComponent->AddInputVector(Vector2f(0.0f, 1.0f));
}

void CharacterComponent::MoveDown()
{
	m_movementComponent->AddInputVector(Vector2f(0.0f, -1.0f));
}

void CharacterComponent::BeginPlay()
{
	m_collider = GetActor()->TryGetComponent<BoxCollider2DComponent>();
	m_spriteRenderer = GetActor()->TryGetComponent<SpriteRendererComponent>();
	m_movementComponent = GetActor()->TryGetComponent<MovementComponent>();

	ScriptComponent::BeginPlay();

	/*if (m_spriteRenderer != nullptr)
	{
		m_spriteChangedHandle = m_spriteRenderer->OnSpriteChanged.Subscribe(this, &CharacterComponent::OnSpriteChanged);
	}*/
}

void CharacterComponent::EndPlay()
{
	/*if (m_spriteRenderer != nullptr)
	{
		m_spriteRenderer->OnSpriteChanged.Unsubscribe(m_spriteChangedHandle);
	}*/
	m_collider = nullptr;
	m_spriteRenderer = nullptr;
	m_movementComponent = nullptr;

	ScriptComponent::EndPlay();
}

/*void CharacterComponent::OnSpriteChanged()
{
	Vector2i size = m_spriteRenderer->Sprite->GetSize();
	m_collider->SetSize({ size.X, size .Y });
}*/