#include "MovementComponent.h"
#include <Component/TransformComponent.h>

#define APP_VIRTUAL_WIDTH		(1024)					// This will be the effective X resolution regardless of actual screen/window res.
#define APP_VIRTUAL_HEIGHT		(768)					// This will be the effective Y resolution regardless of actual screen/window res.

#include <algorithm>

using namespace Sphynx;

MovementComponent::MovementComponent() : 
	m_speed(150.0f), 
	m_inputVector(Vector2f::Zero),
	m_velocityVector(Vector2f::Zero)
{
}

float MovementComponent::GetSpeed() const
{
	return m_speed;
}

void MovementComponent::SetSpeed(float speed)
{
	if (speed > 0.0f)
	{
		m_speed = speed;
	}
}

Vector2f MovementComponent::GetVelocity() const
{
	return m_velocityVector;
}

void MovementComponent::AddInputVector(const Vector2f& direction)
{
	m_inputVector.X += direction.X;
	m_inputVector.Y += direction.Y;
	m_inputVector.Normalize();
}

void MovementComponent::Update(float deltaTime)
{
	ScriptComponent::Update(deltaTime);

	// calculate moveAmount
	m_velocityVector = Vector2f(m_inputVector.X * m_speed, m_inputVector.Y * m_speed);              // direction * speed
	Vector2f moveAmount = Vector2f(m_velocityVector.X * deltaTime, m_velocityVector.Y * deltaTime); // velocity * deltatime

	// set position
	TransformComponent& transformComponent = GetActor()->GetComponent<TransformComponent>();
	Vector3f currentPosition = transformComponent.Transform.Position;
	Vector3f newPosition;
	/*newPosition.X = std::clamp<float>(currentPosition.X + moveAmount.X, 0 + 40, APP_VIRTUAL_WIDTH - 40);
	newPosition.Y = std::clamp<float>(currentPosition.Y + moveAmount.Y, 0 + 50, APP_VIRTUAL_HEIGHT - 50);*/
	newPosition.X = currentPosition.X + moveAmount.X;
	newPosition.Y = currentPosition.Y + moveAmount.Y;

	transformComponent.Transform.Position = { newPosition.X, newPosition.Y, 0.0f };
	
	// reset input vector
	m_inputVector = Vector2f::Zero;
}