#include "pch.h"
#include "Player.h"

Camera::Camera(const vec2& position, const float angle, const vec2& plane)
	: Object(position, true, Sprite()), m_fieldOfView(90.0f), m_plane(plane), m_direction(-1, 0)
{
	m_angle = 0.0f;
	m_plane = MathUtils::Rotate(m_plane, angle);
	m_direction = MathUtils::Rotate(m_direction, angle);
}

void Camera::Move(const vec2& other)
{
	m_position += other;
}

void Camera::Rotate(const float angle)
{
	m_plane = MathUtils::Rotate(m_plane, angle);
	m_direction = MathUtils::Rotate(m_direction, angle);
	m_angle += angle;
	if (m_angle < 0)
		m_angle += 360;
	if (m_angle > 360)
		m_angle -= 360;
}


vec2 Camera::GetDirection() const
{
	return m_direction;
}	

vec2 Camera::GetPlane() const
{
	return m_plane;
}


void Camera::Update(const float deltaTime)
{

	
}

