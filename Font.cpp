#include "pch.h"
#include "Font.h"

Font::Font(const std::string& name)
{
	auto file = std::fstream("fonts/" + name + ".txt", std::ios::in);
	if (!file.is_open())
		throw "Invalid font name!";

	auto line = std::string();
	while (getline(file, line))
	{
		std::stringstream sstr(line);
		auto c = '0';
		auto start = 0, length = 0;
		sstr >> c >> start >> length;
		m_meta.push_back(std::pair<int, int>(start, length));
	}
	file.close();
	m_texture = ResourceManager::GetTexture("font");
}

vec2 Font::GetTextSize(const std::string& text) const
{
	auto size = vec2();
	size.y = m_texture.get()->GetHeight();
	for (auto s : text)
	{
		if (s == (char)32)
		{
			size.x += 5;
			continue;
		}
		auto meta = GetMeta(s);
		size.x += (meta.second + 5);
	}
	return size;
}

Ref<Texture> Font::GetTexture() const
{
	return m_texture;
}

std::pair<int, int> Font::GetMeta(const char symbol) const
{
	return m_meta.at(((int)symbol) - '!');
}
