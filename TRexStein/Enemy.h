#pragma once

#include "vec2.h"
#include "Object.h"

enum class ObjectType
{
	STATIC, ENEMY, NET_PLAYER
};

class __declspec(dllexport) Enemy : public Object
{
public:

	Enemy();
	Enemy(const vec2& position, const float angle, const Sprite& sprite);

	virtual void Update(const float deltaTime) override;
	virtual void CalculateSprite(const vec2& position) override;

	void Die();

protected:
	virtual void OnRaycastHit(int damage) override;

protected:
	int m_health;
	int m_diff;
	ObjectState m_state;
	bool m_isAlive;
	int m_spriteRowIndex;
	float m_spriteIndexCounter;
	bool m_isDamaged;
	bool m_end;


};

