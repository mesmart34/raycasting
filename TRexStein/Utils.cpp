#include "Utils.h"
#include "tinyxml2.h"

Map* Utils::LoadMap(const std::string& path)
{
	using namespace tinyxml2;
	TinyXMLDocument doc;
	if (doc.LoadFile(path.c_str()) != tinyxml2::XML_SUCCESS)
	{
		throw "Error";
		return nullptr;
	}
	auto map = new Map();
	auto root = doc.FirstChildElement();
	auto mapElement = root->FirstChildElement("Map");
	auto playerPos = doc.FirstChildElement("Map")->FirstChildElement("Player");
	auto playerX = playerPos->FindAttribute("x")->FloatValue();
	auto playerY = playerPos->FindAttribute("y")->FloatValue();

	auto walls = playerPos->NextSiblingElement("Level")->FirstChildElement();
	auto width = walls->FindAttribute("width")->IntValue();
	auto height = walls->FindAttribute("height")->IntValue();
	map->SetPlayerSpawnPoint(vec2(playerX, playerY));
	auto row = walls->FirstChildElement();
	for (auto y = 0; y < height; y++)
	{
		auto rowText = row->GetText();
		auto ids = StringUtils::ParseCommand(rowText, ", ");
		map->GetData().push_back(std::vector<int>());
		for (auto id : ids)
		{
			map->GetData()[y].push_back(atoi(id.c_str()));
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
		auto enabled = objNode->FindAttribute("enabled")->BoolValue();
		auto atlasName = objNode->FindAttribute("atlasName")->Value();
		auto spriteId = objNode->FindAttribute("spriteId")->IntValue();
		auto angle = objNode->FindAttribute("angle")->FloatValue();
		auto collidable = objNode->FindAttribute("collidable")->BoolValue();
		auto type = (ObjectType)objNode->FindAttribute("type")->IntValue();
		auto sprite = Sprite();
		sprite.Id = spriteId;
		sprite.Atlas = atlasName;
		auto obj = Object(vec2(x, y), collidable, sprite);
		obj.SetEnable(enabled);

		switch (type)
		{
		case ObjectType::STATIC:
		{
			map->AddObject(CreateRef<Object>(Object(vec2(x, y), collidable, sprite)));
		} break;
		case ObjectType::ENEMY:
		{
			map->AddObject(CreateRef<Enemy>(Enemy(vec2(x, y), 0, sprite)));
		} break;
		default:
			break;
		}


		objNode = objNode->NextSiblingElement();
	}
	return map;
}

void Utils::SaveMap(const Scope<Map>& map, const std::string& path)
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
				for (auto obj : map->GetObjects())
				{
					printer.OpenElement("Object", false);
					{
						if (auto enemy = dynamic_cast<NetPlayer*>(obj.get()))
							continue;
						auto pos = obj->GetPosition();
						auto enabled = obj->IsEnabled();
						auto atlasName = obj->GetSprite().Atlas;
						auto spriteId = obj->GetSprite().Id;
						auto angle = obj->GetAngle();
						auto type = 0;
						printer.PushAttribute("x", pos.x);
						printer.PushAttribute("y", pos.y);
						printer.PushAttribute("enabled", enabled);
						printer.PushAttribute("atlasName", atlasName.c_str());
						printer.PushAttribute("spriteId", spriteId);
						printer.PushAttribute("angle", angle);
						printer.PushAttribute("collidable", obj->IsCollidable());
						if (auto enemy = dynamic_cast<Enemy*>(obj.get()))
						{
							type = (int)ObjectType::ENEMY;
						}
						else {
							type = (int)ObjectType::STATIC;
						}
						printer.PushAttribute("type", type);
					}
					printer.CloseElement();
				}
			}
			printer.CloseElement();
		}
		printer.CloseElement();
	}
	printer.CloseElement();
	std::fstream file(path, std::ios::out);
	file.write(printer.CStr(), printer.CStrSize());
	file.close();
}

void Utils::SolveWallCollision(const Scope<Map>& map, vec2& position, const float radius)
{
	
}

void Utils::DoPhysics(const Scope<Map>& map)
{
	for (auto& obj : map->GetObjects())
	{
		for (auto i = -1; i <= 1; i++)
		{
			for (auto j = -1; j <= 1; j++)
			{
				if (i == 0 && j == 0)
					continue;
				auto x = (int)(obj->GetPosition().x + i);
				auto y = (int)(obj->GetPosition().y + j);
				if (map->GetIndexAt(x, y) == 0)
					continue;
				if (map->GetIndexAt(x, y) == 98)
				{
					if (map->IsDoorOpened(x, y))
						continue;
				}
				auto delta = Physics::Intersection(obj->GetRadius(), obj->GetPosition(), vec2(x, y), vec2(1, 1));
				obj->SetPosition(obj->GetPosition() - delta);
			}
		}
	}
}
