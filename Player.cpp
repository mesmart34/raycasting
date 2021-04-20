#include "Player.h"

Player::Player(const vec2& position, const float angle) : m_position(position), m_angle(angle), m_radius(0.5f), m_fieldOfView(90.0f), m_plane(0, 0.96), m_direction(-1, 0)
{
	m_plane = MathUtils::Rotate(m_plane, angle);
	m_direction = MathUtils::Rotate(m_direction, angle);
}

void Player::Move(const vec2& other)
{
	m_position += other;
}

void Player::Rotate(const float angle)
{
	m_plane = MathUtils::Rotate(m_plane, angle);
	m_direction = MathUtils::Rotate(m_direction, angle);
	m_angle += angle;
}

vec2 Player::GetPosition() const
{
	return m_position;
}

vec2 Player::GetDirection() const
{
	return m_direction;
}

vec2 Player::GetPlane() const
{
	return m_plane;
}

float Player::GetRadius() const
{
	return m_radius;
}

float Player::GetAngle() const
{
	return m_angle;
}

void Player::SetPosition(const vec2& position)
{
	m_position = position;
}
