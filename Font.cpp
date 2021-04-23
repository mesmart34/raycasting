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

	m_texture = std::make_shared<Texture>("fonts/" + name + ".png", 0, 0);
}

std::shared_ptr<Texture> Font::GetTexture() const
{
	return m_texture;
}

std::pair<int, int> Font::GetMeta(const char symbol) const
{
	return m_meta.at(((int)symbol) - '!');
}
