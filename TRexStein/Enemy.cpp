#include "pch.h"
#include "Enemy.h"

Enemy::Enemy(const vec2& position, const float angle, const Sprite& sprite)
	: Object(position, true, sprite),
	m_state(ObjectState::Idle),
	m_isAlive(true),
	m_health(100),
	m_spriteIndexCounter(0),
	m_end(false),
	m_diff(0),
	m_isDamaged(false)
{
	m_spriteRowIndex = rand() % 8;
}

Enemy::Enemy()
	: Object(vec2(), true, Sprite()),
	m_state(ObjectState::Idle),
	m_isAlive(true),
	m_health(100),
	m_spriteIndexCounter(0),
	m_end(false),
	m_diff(0),
	m_isDamaged(false)
{

}

void Enemy::Update(const float deltaTime)
{
	if (m_end)
		return;
	m_spriteIndexCounter += deltaTime * 3;
	if (!m_isAlive)
	{
		
		m_state = ObjectState::Die;
	}
	else {
		if (m_isDamaged)
		{
			m_state = ObjectState::Damaged;
			
		}
		else {
			if (vec2::get_magnitude(m_velocity) > 0.001f)
			{
				m_state = ObjectState::Walk;
			}
			else
			{
				
				m_state = ObjectState::Idle;
			}
		}
	}
	
}

void Enemy::CalculateSprite(const vec2& position)
{
	if (m_end)
		return;
	auto angle = atan2(m_position.x - position.x, m_position.y - position.y) / float(M_PI) / 2 - .5f - (360 - m_angle - 45.0f / 2) / 360.0f;
	m_diff = (((angle * 360)) / -45);

	switch (m_state)
	{
	case ObjectState::Idle:
	{
		m_spriteRowIndex = 0;
		m_sprite.Id = m_diff - 8;

	} break;
	case ObjectState::Walk:
	{
		if (m_spriteIndexCounter > 4)
			m_spriteIndexCounter = 0;
		m_spriteRowIndex = (int)m_spriteIndexCounter + 1;
		m_sprite.Id = m_diff + 8 * 2 + m_spriteRowIndex;
	} break;
	case ObjectState::Attack:
	{
		if (m_spriteIndexCounter > 3)
			m_spriteIndexCounter = 0;
		m_diff = (int)m_spriteIndexCounter;
		m_spriteRowIndex = 6;
		m_sprite.Id = m_diff + 8 * (m_spriteRowIndex);
	} break;
	case ObjectState::Die:
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
	case ObjectState::Damaged:
	{
		//m_spriteIndexCounter += deltaTime;
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
}


void Enemy::Die()
{
	m_state = ObjectState::Die;
	m_isAlive = false;
	m_isCollidable = false;
}

void Enemy::OnRaycastHit(int damage)
{
	if (!m_isAlive)
		return;
	//m_lastState = m_state;
	m_health -= damage;
	m_state = ObjectState::Damaged;
	m_spriteIndexCounter = 0;
	m_isDamaged = true;
	if (m_health <= 0)
	{
		Die();
	}
}
