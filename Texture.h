#pragma once

#include "stbi_image.h"
#include <string>
#include <vector>
#include <memory>
#include "vec2.h"

class Texture
{
public:
	Texture() = default;
	Texture(const std::string& path);

	int GetWidth() const;
	int GetHeight() const;

	std::vector<uint32_t> GetScaledColumn(const int screenHeight, const int width, int height, const int x, const int wallHeight, const vec2& offset) const;

	void LoadFromFile(const std::string& path);

	uint32_t operator[](const int idx) const;

private:
	int m_width;
	int m_height;
	int m_channels;
	std::shared_ptr<uint32_t[]> m_data;
};