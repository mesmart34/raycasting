#include "pch.h"
#include "Object.h"

Object::Object(const Sprite& sprite, const vec2& position, const bool isCollidable)
    : m_sprite(sprite),
    m_position(position), 
    m_isCollidable(isCollidable), 
    m_isEnabled(true), 
    m_velocity(vec2()), 
    m_type(ObjectType::OBJECT)
{
}

Sprite Object::GetSprite() const
{
    return m_sprite;
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
