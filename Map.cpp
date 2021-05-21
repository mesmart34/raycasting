#include "pch.h"
#include "Map.h"


Map* Map::LoadMap(const std::string& name)
{
	using namespace tinyxml2;
	XMLDocument doc;
	auto map = new Map();
	if (doc.LoadFile(name.c_str()) != tinyxml2::XML_SUCCESS)
		return nullptr;
	auto root = doc.FirstChildElement();
	auto mapElement = root->FirstChildElement("Map");
	auto playerPos = doc.FirstChildElement("Map")->FirstChildElement("Player");
	auto playerX =  playerPos->FindAttribute("x")->FloatValue();
	auto playerY = playerPos->FindAttribute("y")->FloatValue();

	auto walls = playerPos->NextSiblingElement("Level")->FirstChildElement();
	auto width = walls->FindAttribute("width")->IntValue();
	auto height = walls->FindAttribute("height")->IntValue();
	map->m_playerSpawnPoint = vec2(playerX, playerY);
	auto row = walls->FirstChildElement();
	for (auto y = 0; y < height; y++)
	{
		auto rowText = row->GetText();
		auto ids = StringUtils::ParseCommand(rowText, ", ");
		map->m_data.push_back(std::vector<int>());
		for (auto id : ids)
		{
			map->m_data[y].push_back(atoi(id.c_str()));
		}
		row = row->NextSiblingElement();
	}
	map->InitDoors();

	auto objects = walls->NextSiblingElement();
	auto objNode = objects->FirstChildElement();
	while (objNode != nullptr)
	{
		auto x = objNode->FindAttribute("x")->FloatValue();
		auto y = objNode->FindAttribute("y")->FloatValue();
		auto type = (ObjectType)objNode->FindAttribute("type")->IntValue();
		auto enabled = objNode->FindAttribute("enabled")->BoolValue();
		auto atlasName = objNode->FindAttribute("atlasName")->Value();
		auto spriteId = objNode->FindAttribute("spriteId")->IntValue();
		auto angle = objNode->FindAttribute("angle")->FloatValue();
		auto collidable = objNode->FindAttribute("collidable")->BoolValue();

		auto sprite = Sprite();
		sprite.Id = spriteId;
		sprite.Atlas = ResourceManager::GetTexture(atlasName);
		if (type == ObjectType::OBJECT)
		{
			auto obj = CreateRef<Object>(sprite, vec2(x, y), collidable);
			obj->SetEnable(enabled);
			map->m_objects.push_back(obj);
		}
		else if (type == ObjectType::ENEMY)
		{
			auto obj = CreateRef<Enemy>(sprite, vec2(x, y), angle);
			obj->SetEnable(enabled);
			map->m_objects.push_back(obj);
		}
		

		objNode = objNode->NextSiblingElement();
	}

	return map;
	
}

void Map::SaveMap(const std::string& name, Map* map)
{
	using namespace tinyxml2;
	XMLPrinter printer;
	printer.OpenElement("Map");
	{
		printer.OpenElement("Player");
		printer.PushAttribute("x", map->GetPlayerSpawnPoint().x);
		printer.PushAttribute("y", map->GetPlayerSpawnPoint().y);
		printer.CloseElement();
		printer.OpenElement("Level");
		{
			printer.OpenElement("Walls");
			printer.PushAttribute("width", map->GetWidth());
			printer.PushAttribute("height", map->GetHeight());
			for (auto x = 0; x < map->GetHeight(); x++)
			{
				printer.OpenElement("Row");
				{
					std::string row;
					for (auto y = 0; y < map->GetWidth(); y++)
					{
						row += std::to_string(map->GetIndexAt(x, y));
						if (y != map->GetWidth() - 1)
						{
							row += ", ";
						}
					}
					printer.PushText(row.c_str());
				}
				printer.CloseElement();
			}
			printer.CloseElement();
			printer.OpenElement("Objects");
			{
				for (auto obj : map->m_objects)
				{
					printer.OpenElement("Object", false);
					{
						auto pos = obj->GetPosition();
						auto enabled = obj->IsEnabled();
						auto atlasName = obj->GetSprite().Atlas;
						auto spriteId = obj->GetSprite().Id;
						auto type = (char)obj->GetType();
						printer.PushAttribute("x", pos.x);
						printer.PushAttribute("y", pos.y);
						printer.PushAttribute("type", type);
						printer.PushAttribute("enabled", enabled);
						printer.PushAttribute("atlasName", atlasName->GetName().c_str());
						printer.PushAttribute("spriteId", spriteId);
						auto angle = 0;
						if (obj->GetType() == ObjectType::ENEMY)
						{
							auto enemy = (Enemy*)(obj).get();
							angle = enemy->GetAngle();
						}
						printer.PushAttribute("angle", angle);
						printer.PushAttribute("collidable", obj->IsCollidable());
					}
					printer.CloseElement();
				}
			}
			printer.CloseElement();
		}
		printer.CloseElement();
	}
	printer.CloseElement();
	std::fstream file(name, std::ios::out);
	file.write(printer.CStr(), printer.CStrSize());
	file.close();
}

int Map::GetIndexAt(const int x, const int y) const
{
	if (IsInBounds(x, y))
		return m_data[x][y];
	return -1;
}

void Map::AddObject(const Ref<Object>& obj)
{
	m_objects.push_back(obj);
}

float Map::GetDoorInfo(const int x, const int y) const
{
	auto idx = (int)(x + y * m_data[0].size());
	if (IsInBounds(x, y))
		return m_doors.at(x + y * GetWidth())->GetTextureOffset();
	return 1.0f;
}

bool Map::IsDoorOpened(const int x, const int y) const
{
	auto idx = (int)(x + y * m_data[0].size());
	if (IsInBounds(x, y))
		return m_doors.at(x + y * GetWidth())->IsOpened();
	return false;
}

std::vector<Ref<Object>>& Map::GetObjects()
{
	return m_objects;
}

int Map::GetWidth() const
{
	return m_data[0].size();
}

int Map::GetHeight() const
{
	return m_data.size();
}

vec2 Map::GetPlayerSpawnPoint() const
{
	return m_playerSpawnPoint;
}

void Map::OpenDoorAt(const int x, const int y)
{
	m_doors[x + y * GetWidth()]->Open();
}

void Map::UpdateDoors(const float deltaTime)
{
	for (auto& d : m_doors)
	{
	//std::cout << deltaTime << std::endl;
		d.second->Update(deltaTime);
	}
}

void Map::InitDoors()
{
	for (auto x = 0; x < m_data.size(); x++)
	{
		for (auto y = 0; y < m_data[x].size(); y++)
		{
			if (m_data[x][y] == 98)
				m_doors[x + y * GetWidth()] = CreateRef<Door>();
		}
	}
}

bool Map::IsInBounds(const int x, const int y) const
{
	return x >= 0 && x < m_data.size() && y >= 0 && y < m_data[x].size();
}
