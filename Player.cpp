#include "Player.h"

Player::Player(const vec2& position, const float angle, const vec2& plane) : m_position(position), m_angle(angle), m_radius(0.3f), m_fieldOfView(90.0f), m_plane(plane), m_direction(-1, 0)
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
	if (m_angle < 0)
		m_angle += 360;
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
	return (int)(m_angle) % 360;
}

void Player::Update(const float deltaTime)
{
	Rotate(-Input::GetMouseAxis().x * deltaTime * 50.0f);

	if (Input::IsKeyPressed(SDL_SCANCODE_W))
		m_velocity += m_direction * deltaTime * 0.2;
	else if (Input::IsKeyPressed(SDL_SCANCODE_S))
		m_velocity += m_direction * deltaTime * -0.2;
	if (Input::IsKeyPressed(SDL_SCANCODE_A))
		m_velocity += vec2(-m_direction.y, m_direction.x) * deltaTime * 0.2;
	else if (Input::IsKeyPressed(SDL_SCANCODE_D))
		m_velocity += vec2(m_direction.y, -m_direction.x) * deltaTime * 0.2;

	m_velocity.x = MathUtils::Clamp(m_velocity.x, -0.5, 0.5);
	m_velocity.y = MathUtils::Clamp(m_velocity.y, -0.5, 0.5);
	Move(m_velocity);
	m_velocity *= 0.9;
}

void Player::SetPosition(const vec2& position)
{
	m_position = position;
}

vec2 Player::GetVelocity() const
{
	return m_velocity;
}

void Player::AddVelocity(const vec2& vel)
{
	m_velocity += vel;
}
