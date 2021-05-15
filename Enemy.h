#pragma once

#include "vec2.h"
#include "Object.h"
#include "Player.h"

class Enemy : public Object
{
public:

	enum class EnemyState
	{
		Idle, Walk, Attack, Die, Damaged
	};
	Enemy(const Sprite& sprite, const vec2& position, const float angle);

	void Update(const float deltaTime, const Player& player);

	float GetAngle() const;
	void Die();

protected:
	virtual void OnRaycastHit(int damage) override;

private:
	float m_angle = 0.0f;
	int m_spriteRowIndex;
	float m_spriteIndexCounter;
	EnemyState m_state;
	bool m_isAlive;
	int m_health;
};

