#include "pch.h"
#include "Enemy.h"

Enemy::Enemy(const Sprite& sprite, const vec2& position, const float angle)
	: Object(sprite, position, true), m_angle(angle), m_state(EnemyState::Walk), m_isAlive(true), m_health(100), m_spriteIndexCounter(0)
{
	m_spriteRowIndex = rand() % 8;
}

void Enemy::Update(const float deltaTime, const Player& player)
{
	if (!m_isAlive)
		return;
	
	auto diff = (int)((MathUtils::GetAngularDifference(player.GetAngle(), m_angle - 25.5f)) / 45.0f);
	m_spriteIndexCounter += deltaTime * 3;

	if (m_state == EnemyState::Idle)
	{
		m_spriteRowIndex = 0;
		m_sprite.Id = diff + 8 * (m_spriteRowIndex);
	}
	else if (m_state == EnemyState::Walk)
	{
		if (m_spriteIndexCounter > 4)
			m_spriteIndexCounter = 0;
		m_spriteRowIndex = (int)m_spriteIndexCounter + 1;
		m_sprite.Id = diff + 8 * (m_spriteRowIndex);

	}
	else if (m_state == EnemyState::Attack)
	{
		if (m_spriteIndexCounter > 3)
			m_spriteIndexCounter = 0;
		diff = (int)m_spriteIndexCounter;
		m_spriteRowIndex = 6;
		m_sprite.Id = diff + 8 * (m_spriteRowIndex);
	}
	else if (m_state == EnemyState::Die)
	{
		diff = (int)m_spriteIndexCounter;
		if (m_spriteIndexCounter > 5)
		{
			m_spriteIndexCounter = 0;
			m_isAlive = false;
		}
		m_spriteRowIndex = 5;
		m_sprite.Id = diff + 8 * (m_spriteRowIndex);
	}
	else if (m_state == EnemyState::Damaged)
	{
		m_spriteIndexCounter += deltaTime;
		if (m_spriteIndexCounter > 3)
		{
			m_state = EnemyState::Walk;
			m_spriteIndexCounter = 0;
		}
		m_sprite.Id = 8 * 6;
	}


	/*m_angle -= deltaTime * 100;
	if (m_angle >= 360)
		m_angle -= 360;
	if (m_angle < 0)
		m_angle += 360;*/

	//printf("%d\n", diff);
	
}

float Enemy::GetAngle() const
{
	return m_angle;
}

void Enemy::Die()
{
	m_state = EnemyState::Die;
	m_isCollidable = false;
}

void Enemy::OnRaycastHit(int damage)
{
	m_health -= damage;
	m_state = EnemyState::Damaged;
	m_spriteIndexCounter = 0;
	if (m_health <= 0)
	{
		Die();

	}
}
