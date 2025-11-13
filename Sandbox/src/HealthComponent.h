#pragma once

#include "Sandbox.h"
#include <Component/ScriptComponent.h>
#include <Core/Delegate.h>


class SANDBOX_API HealthComponent : public Sphynx::ScriptComponent
{
	SPX_REFLECT_GENERATED_BODY()

public:
	HealthComponent();

	float GetMaxHealth() const;
	void SetMaxHealth(float health);
	float GetCurrentHealth() const;
	void SetCurrentHealth(float health);
	void SetHealth(float health);
	bool GetIsInvulnerable() const;
	void SetIsInvulnerable(bool invulnerable);

	void TakeDamage(float damage);
	void TakeHealing(float healing);

public:
	Sphynx::MulticastDelegate<void(float)> OnHealthUpdated;

private:
	float m_maxHealth;
	float m_currentHealth;
	bool m_isInvulnerable;
};


#include "Reflection/Reflection.h"

SPX_REFLECT_CLASS_BEGIN(HealthComponent, SANDBOX_API)
SPX_REFLECT_INHERITANCE(Sphynx::ScriptComponent)
SPX_REFLECT_ATTRIBUTE(InternalComponent);

SPX_REFLECT_PROPERTY(m_maxHealth)
SPX_REFLECT_PROPERTY(m_isInvulnerable)

SPX_REFLECT_CLASS_END(HealthComponent, SANDBOX_API)