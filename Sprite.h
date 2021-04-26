#pragma once
#include "Base.h"
#include <memory>
#include <vector>
#include "vec2.h"
#include "Texture.h"


struct Sprite
{
	Ref<Texture> Atlas;
	int Id;
};

