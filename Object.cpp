#include "pch.h"
#include "Object.h"

Object::Object(const int id, const Sprite& sprite, const vec2& position, const bool isCollidable)
    : m_sprite(sprite),
    m_position(position),
    m_isCollidable(isCollidable),
    m_isEnabled(true),
    m_velocity(vec2()),
    m_radius(0.25f),
    m_id(id),
    m_angle(0.0)
{
    
}

Object::Object(const vec2& position, const bool isCollidable, const Sprite& sprite)
    : m_sprite(sprite),
    m_position(position),
    m_isCollidable(isCollidable),
    m_isEnabled(true),
    m_velocity(vec2()),
    m_radius(0.25f),
    m_id(-1),
    m_angle(0.0)
{

}

void Object::SetID(const int id)
{
    m_id = id;
}

int Object::GetID() const
{
    return m_id;
}

Sprite Object::GetSprite() const
{
    return m_sprite;
}

void Object::SetSprite(const Sprite& sprite)
{
    m_sprite = sprite;
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

void Object::SetColliable(const bool value)
{
    m_isCollidable = value;
}

void Object::SetAngle(const float angle)
{
    m_angle = angle;
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
