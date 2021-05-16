#include "pch.h"
#include "NetPlayer.h"

NetPlayer::NetPlayer(const Sprite& sprite, const vec2& position, const float angle)
	: Enemy(sprite, position, angle)
{

}

void NetPlayer::SetPosition(const vec2& position)
{
	m_position = position;
}

void NetPlayer::SetAngle(const float angle)
{
	m_angle = angle;
}
