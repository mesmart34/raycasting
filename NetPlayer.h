#pragma once
#include "Enemy.h"
class NetPlayer : public Enemy
{
public:
	NetPlayer(const Sprite& sprite, const vec2& position, const float angle);

	void SetPosition(const vec2& position);
	void SetAngle(const float angle);



private:

};

