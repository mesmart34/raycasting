#pragma once
#include <memory>
#include <vector>
#include "vec2.h"
#include "Texture.h"


struct Sprite
{
	std::shared_ptr<Texture> Atlas;
	int Id;
};

