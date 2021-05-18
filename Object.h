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
	vec2 GetVelocity() const;
	void SetVelocity(const vec2 velocity);
	bool IsCollidable() const;

	bool IsEnabled() const;
	void SetEnable(const bool value);

	virtual void OnRaycastHit(int damage) { }

protected:
	Sprite m_sprite;
	vec2 m_position;
	vec2 m_velocity;
	bool m_isCollidable;
	bool m_isEnabled;
};