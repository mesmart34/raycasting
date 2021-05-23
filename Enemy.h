#pragma once

#include "vec2.h"
#include "Object.h"

enum class EnemyState
{
	Idle, Walk, Attack, Die, Damaged
};

class Enemy : public Object
{
public:

	
	Enemy(const Sprite& sprite, const vec2& position, const float angle);

	virtual void Update(const float deltaTime) override;
	virtual void CalculateSprite(const vec2& position) override;

	float GetAngle() const;
	void Die();

protected:
	virtual void OnRaycastHit(int damage) override;

protected:
	float m_angle = 0.0f;
	int m_health;
	int m_diff;
	EnemyState m_state;
	bool m_isAlive;
	int m_spriteRowIndex;
	float m_spriteIndexCounter;
	bool m_isDamaged;
	bool m_end;


};

