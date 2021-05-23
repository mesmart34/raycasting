#include "pch.h"
#include "NetPlayer.h"

NetPlayer::NetPlayer(const Sprite& sprite, const vec2& position, const float angle)
	: Enemy(sprite, position, angle)
{
	SetEnable(false);
	m_type = ObjectType::NET_PLAYER;
}

void NetPlayer::SetPosition(const vec2& position)
{
	m_position = position;
}

void NetPlayer::SetAngle(const float angle)
{
	m_angle = angle;
}

void NetPlayer::SetState(const EnemyState state)
{
	m_state = state;
}

void NetPlayer::Update(const float deltaTime)
{
	m_spriteIndexCounter += deltaTime * 5;
	m_position += m_velocity * deltaTime;
}

void NetPlayer::CalculateSprite(const vec2& position)
{
	if (m_end)
		return;
	auto angle = atan2(m_position.x - position.x, m_position.y - position.y) / float(M_PI) / 2 - .5f - (360 - GetAngle() - 45.0f / 2) / 360.0f;
	m_diff = (((angle * 360)) / -45);
	
	switch (m_state)
	{
	case EnemyState::Idle:
	{
		m_spriteRowIndex = 0;
		m_sprite.Id = m_diff - 8;
		if (m_sprite.Id < 0)
			m_sprite.Id = 8 + m_sprite.Id;
	} break;
	case EnemyState::Walk:
	{
		if (m_spriteIndexCounter > 4)
			m_spriteIndexCounter = 0;
		m_spriteRowIndex = (int)m_spriteIndexCounter;
		m_sprite.Id = m_diff + m_spriteRowIndex * 8;
	} break;
	default:
		break;
	}
}

void NetPlayer::OnRaycastHit(int damage)
{
}
