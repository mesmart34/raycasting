#include "pch.h"
#include "Enemy.h"

Enemy::Enemy(const Sprite& sprite, const vec2& position, const float angle)
	: Object(sprite, position, true),
	m_angle(angle), 
	m_state(EnemyState::Idle), 
	m_isAlive(true), 
	m_health(100), 
	m_spriteIndexCounter(0), 
	m_end(false)
{
	m_spriteRowIndex = rand() % 8;
	m_type = ObjectType::ENEMY;
}

void Enemy::Update(const float deltaTime, const Player& player)
{
	if (m_end)
		return;
	auto angle = atan2(m_position.x - player.GetPosition().x, m_position.y - player.GetPosition().y) / float(M_PI) / 2 - .5f - (360 - GetAngle() - 45.0f / 2) / 360.0f;
	m_diff = (((angle * 360)) / -45);
	m_spriteIndexCounter += deltaTime * 5;
	if (!m_isAlive)
	{
		
		m_state = EnemyState::Die;
	}
	else {
		if (m_isDamaged)
		{
			m_state = EnemyState::Damaged;
			
		}
		else {
			if (vec2::get_magnitude(m_velocity) > 0.001f)
			{
				m_state = EnemyState::Walk;
			}
			else
			{
				
				m_state = EnemyState::Idle;
			}
		}
	}

	switch (m_state)
	{
	case EnemyState::Idle:
	{
		m_spriteRowIndex = 0;
		m_sprite.Id = m_diff - 8;
		
	} break;
	case EnemyState::Walk:
	{
		if (m_spriteIndexCounter > 4)
			m_spriteIndexCounter = 0;
		m_spriteRowIndex = (int)m_spriteIndexCounter + 1;
		m_sprite.Id = m_diff + 8 * 2 + m_spriteRowIndex;
	} break;
	case EnemyState::Attack:
	{
		if (m_spriteIndexCounter > 3)
			m_spriteIndexCounter = 0;
		m_diff = (int)m_spriteIndexCounter;
		m_spriteRowIndex = 6;
		m_sprite.Id = m_diff + 8 * (m_spriteRowIndex);
	} break;
	case EnemyState::Die:
	{
		m_diff = (int)m_spriteIndexCounter;
		if (m_spriteIndexCounter > 5)
		{
			m_spriteIndexCounter = 0;
			m_end = true;
		}
		m_spriteRowIndex = 5;
		m_sprite.Id = m_diff + 8 * (m_spriteRowIndex);
	} break;
	case EnemyState::Damaged:
	{
		m_spriteIndexCounter += deltaTime;
		if (m_spriteIndexCounter > 3)
		{
			m_spriteIndexCounter = 0;
			m_isDamaged = false;
		}
		m_sprite.Id = 8 * 6;
	} break;
	default:
		break;
	}
	/*if (vec2::get_magnitude(m_velocity) > 0.001f && m_state != EnemyState::Die && m_state != EnemyState::Damaged)
	{
		m_state = EnemyState::Walk;
	}
	else {

		m_state = EnemyState::Idle;
	}*/
	m_position += m_velocity;
}

float Enemy::GetAngle() const
{
	return m_angle;
}

void Enemy::Die()
{
	m_state = EnemyState::Die;
	m_isAlive = false;
	m_isCollidable = false;
}

void Enemy::OnRaycastHit(int damage)
{
	if (!m_isAlive)
		return;
	//m_lastState = m_state;
	m_health -= damage;
	m_state = EnemyState::Damaged;
	m_spriteIndexCounter = 0;
	m_isDamaged = true;
	if (m_health <= 0)
	{
		Die();
	}
}
