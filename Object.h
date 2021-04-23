#pragma once

#include "Sprite.h"
#include "vec2.h"

class Object
{
public:
	Object() = default;
	Object(const Sprite& sprite, const vec2& position, const bool isCollidable);

	virtual ~Object() = default;

	Sprite GetSprite() const;
	vec2 GetPosition() const;
	bool IsCollidable() const;

protected:
	Sprite m_sprite;
	vec2 m_position;
	bool m_isCollidable;
};