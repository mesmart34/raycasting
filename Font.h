#pragma once

#include <memory>
#include "Texture.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class Font
{
public:
	Font() = default;
	Font(const std::string& name);



	std::shared_ptr<Texture> GetTexture() const;
	std::pair<int, int> GetMeta(const char symbol) const;

private:
	std::vector<std::pair<int, int>> m_meta;
	std::shared_ptr<Texture> m_texture;
};

