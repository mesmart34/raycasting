#include "pch.h"
#include "NetPlayer.h"

NetPlayer::NetPlayer(const int clientId, const vec2& position, const float angle, const Sprite& sprite)
	: Enemy(position, angle, sprite), m_clientID(clientId)
{
	
}

NetPlayer::NetPlayer()
	: Enemy(vec2(), 0, Sprite()), m_clientID(0)
{
	
}

int NetPlayer::GetClientID() const
{
	return m_clientID;
}

ObjectState NetPlayer::GetState() const
{
	return m_state;
}

void NetPlayer::SetState(const ObjectState state)
{
	m_state = state;
}

void NetPlayer::Update(const float deltaTime)
{
	m_spriteIndexCounter += deltaTime * 5;
	//m_position += m_velocity * deltaTime;
}

void NetPlayer::CalculateSprite(const vec2& position)
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
		if (m_sprite.Id < 0)
			m_sprite.Id = 8 + m_sprite.Id;
	} break;
	case ObjectState::Walk:
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
