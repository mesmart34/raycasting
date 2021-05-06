#pragma once

#include "vec2.h"
#include "Object.h"
#include "Player.h"

class Enemy : public Object
{
public:
	Enemy(const Sprite& sprite, const vec2& position, const float angle);

	void Update(const float deltaTime, const Player& player);

	float GetAngle() const;

private:
	float m_angle = 0.0f;
};

