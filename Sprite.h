#pragma once
#include "Base.h"
#include <memory>
#include <vector>
#include "vec2.h"
#include "Texture.h"
#include <string>

struct __declspec(dllexport) Sprite
{
	Sprite() = default;
	Sprite(const int id, const std::string& atlas);
	virtual ~Sprite() {};
	std::string Atlas = "";
	int Id = 0;
};

