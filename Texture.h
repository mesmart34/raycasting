#pragma once

#include "stbi_image.h"
#include <string>
#include <vector>
#include <memory>
#include "vec2.h"
#include "Base.h"

class Texture
{
public:
	Texture() = default;
	Texture(const std::string& path, const int columns, const int rows);

	int GetWidth() const;
	int GetHeight() const;

	std::vector<uint32_t> GetScaledColumn(const float wallX, const int screenHeight, const int height, const vec2& textureOffset, const int id) const;

	void LoadFromFile(const std::string& path);

	uint32_t operator[](const int idx) const;

private:
	int m_width;
	int m_height;
	int m_channels;
	int m_columns;
	int m_rows;
	Ref<uint32_t[]> m_data;
};