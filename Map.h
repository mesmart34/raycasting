#pragma once

#include <vector>
#include "Object.h"
#include <map>
#include "vec2.h"
#include "Door.h"
#include "Enemy.h"
#include "tinyxml2.h"
#include "Map.h"
#include "NetPlayer.h"
#include "StringUtils.h"
#include "ResourceManager.h"

class Map
{
public:
	Map() = default;
	static Map* LoadMap(const std::string& name);
	static void SaveMap(const std::string& name, Map* map);

	int GetIndexAt(const int x, const int y) const;
	void AddObject(const Ref<Object>& obj);
	float GetDoorInfo(const int x, const int y) const;
	bool IsDoorOpened(const int x, const int y) const;
	std::vector<Ref<Object>>& GetObjects();
	int GetWidth() const;
	int GetHeight() const;
	vec2 GetPlayerSpawnPoint() const;

	void OpenDoorAt(const int x, const int y);
	void UpdateDoors(const float deltaTime);

private:
	void InitDoors();
	bool IsInBounds(const int x, const int y) const;

private:
	std::vector<std::vector<int>> m_data;
	std::vector<Ref<Object>> m_objects;
	std::map<int, Ref<Door>> m_doors;
	vec2 m_playerSpawnPoint;

};

