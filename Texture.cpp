#include "Texture.h"

Texture::Texture(const std::string& path)
{
	LoadFromFile(path);
}

void Texture::LoadFromFile(const std::string& path)
{
	m_data = std::shared_ptr<uint32_t[]>((uint32_t*)stbi_load(path.c_str(), &m_width, &m_height, &m_channels, 4));
}

uint32_t Texture::operator[](const int idx) const
{
	return m_data[idx];
}

int Texture::GetWidth() const
{
	return m_width;
}

int Texture::GetHeight() const
{
	return m_height;
}

std::vector<uint32_t> Texture::GetScaledColumn(const int screenHeight, const int width, const int height, const int x, const int wallHeight, const vec2& offset) const
{
	auto column = std::vector<uint32_t>(height);
	for (auto y = 0; y < height; y++) {
		auto _y = (int)((float)(y * 64) / (float)height);
		column[y] = m_data[(x / 6) + offset.x * 64 + ((_y + offset.y * 64) * m_width)];
	}
	return column;
}
