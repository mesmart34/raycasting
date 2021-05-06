#include "pch.h"
#include "Object.h"

Object::Object(const Sprite& sprite, const vec2& position, const bool isCollidable)
    : m_sprite(sprite), m_position(position), m_isCollidable(isCollidable)
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

bool Object::IsCollidable() const
{
    return m_isCollidable;
}
