#pragma once
#include "Ray.h"
#include "Player.h"
#include <vector>

class Raycaster
{
public:
	Raycaster() = default;

	Ray CastRay(const int strip, const int width, const std::vector<std::vector<int>>& map, const Player& player) const;

private:

};

