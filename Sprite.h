#pragma once
#include "Base.h"
#include <memory>
#include <vector>
#include "vec2.h"
#include "Texture.h"


struct Sprite
{
	Sprite() = default;
	Sprite(const int id, const Ref<Texture> atlas);
	Ref<Texture> Atlas;
	int Id;
};

