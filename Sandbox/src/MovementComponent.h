#pragma once

#include "Sandbox.h"
#include <Component/ScriptComponent.h>
#include <Math/Vector.h>


class SANDBOX_API MovementComponent : public Sphynx::ScriptComponent
{
	SPX_REFLECT_GENERATED_BODY()

public:
	MovementComponent();

	float GetSpeed() const;
	void SetSpeed(float speed);
	Sphynx::Vector2f GetVelocity() const;

	void AddInputVector(const Sphynx::Vector2f& direction);

	virtual void Update(float deltaTime) override;

protected:
	float m_speed;
	Sphynx::Vector2f m_inputVector;
	Sphynx::Vector2f m_velocityVector;
};


#include "Reflection/Reflection.h"

SPX_REFLECT_CLASS_BEGIN(MovementComponent, SANDBOX_API)
SPX_REFLECT_INHERITANCE(Sphynx::ScriptComponent)
SPX_REFLECT_ATTRIBUTE(InternalComponent);

SPX_REFLECT_PROPERTY(m_speed)

SPX_REFLECT_CLASS_END(MovementComponent, SANDBOX_API)