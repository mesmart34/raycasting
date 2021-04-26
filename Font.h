#pragma once

#include <memory>
#include "Texture.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "Base.h"
#include <sstream>

class Font
{
public:
	Font() = default;
	Font(const std::string& name);



	Ref<Texture> GetTexture() const;
	std::pair<int, int> GetMeta(const char symbol) const;

private:
	std::vector<std::pair<int, int>> m_meta;
	Ref<Texture> m_texture;
};

