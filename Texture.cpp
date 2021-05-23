#include "pch.h"
#include "Texture.h"

Texture::Texture(const std::string& name, const std::string& path, const int columns = 0, const int rows = 0)
	: m_columns(columns), m_rows(rows), m_name(name)
{
	LoadFromFile(path);
}

void Texture::LoadFromFile(const std::string& path)
{
	m_data = Ref<uint32_t[]>((uint32_t*)stbi_load(path.c_str(), &m_width, &m_height, &m_channels, 4));
}

uint32_t Texture::operator[](const int idx) const
{
	return m_data[idx];
}

std::string Texture::GetName() const
{
	return m_name;
}

int Texture::GetWidth() const
{
	return m_width;
}

int Texture::GetHeight() const
{
	return m_height;
}

void Texture::GetScaledColumn(uint32_t  *column, const float wallX, const int screenHeight, const int height, const vec2& textureOffset, const int id, const float horizont) const
{
	auto delta = 0;
	auto trueHeight = height;
	if (height > screenHeight)
	{
		delta = (height - screenHeight) / 2;
		trueHeight = screenHeight;
	}
	auto offset = vec2((id - 1) % m_columns, (id - 1) / m_columns) * 64;
	auto fixedWallX = wallX;
	auto oldIndex = 0;
	auto cache = (uint32_t)0;
	auto x = fabs((fixedWallX - textureOffset.x) * 64 + offset.x);
	for (auto i = 0; i < trueHeight; i++)
	{
		auto _y = (int)((float)((i + delta) * 64) / (float)height);
		auto y = _y + offset.y;
		auto index = (int)(x + y * m_width);
		if (index != oldIndex)
			cache = m_data[index];
		oldIndex = index;
		column[i] = m_data[index];
	}
}
