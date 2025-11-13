#include "HealthComponent.h"


HealthComponent::HealthComponent() : 
	m_maxHealth(100.0f),
	m_currentHealth(100.0f),
	m_isInvulnerable(false)
{
}

float HealthComponent::GetMaxHealth() const
{
	return m_maxHealth;
}

void HealthComponent::SetMaxHealth(float health)
{
	m_maxHealth = health;
}

float HealthComponent::GetCurrentHealth() const
{
	return m_currentHealth;
}

void HealthComponent::SetCurrentHealth(float health)
{
	m_currentHealth = health;
}

void HealthComponent::SetHealth(float health)
{
	m_maxHealth = health;
	m_currentHealth = health;
}

bool HealthComponent::GetIsInvulnerable() const
{
	return m_isInvulnerable;
}

void HealthComponent::SetIsInvulnerable(bool invulnerable)
{
	m_isInvulnerable = invulnerable;
}

void HealthComponent::TakeDamage(float damage)
{
	if (m_isInvulnerable) return;
		
	m_currentHealth -= damage;
	OnHealthUpdated.Broadcast(m_currentHealth);
	if (m_currentHealth <= 0.0f)
	{
		SPX_LOG_DEBUG("DESTROYED!!!:  DAMAGE '{}'   -   CURRENT HEALTH '{}'", damage, m_currentHealth);

		//GetOwner()->Destroy(); // TODO
	} 
	else
	{
		SPX_LOG_DEBUG("DAMAGE '{}'   -   CURRENT HEALTH '{}'", damage, m_currentHealth);
	}
}

void HealthComponent::TakeHealing(float healing)
{
	float health = m_currentHealth + healing;
	if (health <= m_maxHealth)
	{
		m_currentHealth = health;
	}
	else 
	{
		m_currentHealth = m_maxHealth;
	}
	OnHealthUpdated.Broadcast(m_currentHealth);
}