#pragma once

#include <vector>
#include "Object.h"
#include <map>
#include <utility>
#include "vec2.h"
#include "Door.h"
#include "Map.h"
#include "StringUtils.h"
#include "ResourceManager.h"

class __declspec(dllexport) Map
{
public:
	Map();
	
	void InitDoors();

	int GetIndexAt(const int x, const int y) const;
	void AddObject(const Ref<Object>& obj);
	float GetDoorInfo(const int x, const int y) const;
	bool IsDoorOpened(const int x, const int y) const;
	std::vector<Ref<Object>>& GetObjects();
	int GetWidth() const;
	int GetHeight() const;
	vec2 GetPlayerSpawnPoint() const;
	void SetPlayerSpawnPoint(const vec2& point);
	std::vector<std::vector<int>>& GetData();
	void EraseObjectWithId(const int id);

	void OpenDoorAt(const int x, const int y);
	void UpdateDoors(const float deltaTime);

private:
	bool IsInBounds(const int x, const int y) const;

private:
	std::vector<std::vector<int>> m_data;
	std::vector<Ref<Object>> m_objects;
	std::map<int, Ref<Door>> m_doors;
	vec2 m_playerSpawnPoint;
	int m_objectCounter;

};

