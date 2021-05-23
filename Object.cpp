#include "pch.h"
#include "Object.h"

Object::Object(const Sprite& sprite, const vec2& position, const bool isCollidable)
    : m_sprite(sprite),
    m_position(position),
    m_isCollidable(isCollidable),
    m_isEnabled(true),
    m_velocity(vec2()),
    m_type(ObjectType::OBJECT),
    m_radius(0.25f)
{
}

Sprite Object::GetSprite() const
{
    return m_sprite;
}

void Object::SetPosition(const vec2& position)
{
    m_position = position;
}

vec2 Object::GetPosition() const
{
    return m_position;
}

vec2 Object::GetVelocity() const
{
    return m_velocity;
}

void Object::SetVelocity(const vec2 velocity)
{
    m_velocity = velocity;
}

bool Object::IsCollidable() const
{
    return m_isCollidable;
}

float Object::GetRadius() const
{
    return m_radius;
}

float Object::GetAngle() const
{
    return m_angle;
}

void Object::AddVelocity(const vec2& vel)
{
    m_velocity += vel;
}

bool Object::IsEnabled() const
{
    return m_isEnabled;
}

void Object::SetEnable(const bool value)
{
    m_isEnabled = value;
}

ObjectType Object::GetType() const
{
    return m_type;
}

void Object::Physics(const float deltaTime)
{
    m_position += m_velocity;
	m_velocity.x = MathUtils::Clamp(m_velocity.x, -0.05, 0.05);
	m_velocity.y = MathUtils::Clamp(m_velocity.y, -0.05, 0.05);
	m_velocity *= 0.95;
	if (fabs(m_velocity.x) < 0.001f)
		m_velocity.x = 0;
	if (fabs(m_velocity.y) < 0.001f)
		m_velocity.y = 0;
}
