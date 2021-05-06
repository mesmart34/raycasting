#include "pch.h"
#include "Map.h"

Map::Map(const vec2& playerSpawnPoint) : m_playerSpawnPoint(playerSpawnPoint)
{
	m_data = {
		{32, 32,32,32, 74, 32, 32, 32},
		{32, 0, 0, 0, 0, 0, 0, 32},
		{32, 0, 0, 0, 0, 0, 0, 32},
		{32, 0, 0, 0, 0, 0, 0, 32},
		{32, 0, 0, 0, 0, 0, 0, 32},
		{32, 0, 0, 0, 0, 0, 0, 32},
		{32, 0, 0, 0, 0, 0, 0, 32},
		{32, 0, 0, 0, 0, 0, 0, 32},
		{32, 0, 0, 0, 0, 0, 0, 32},
		{32, 0, 0, 0, 0, 0, 0, 32},
		{32, 0, 0, 0, 0, 0, 0, 32},
		{32, 98, 32, 98, 32, 32, 32, 32},
		{32, 0, 98, 0, 0, 32, 0, 32},
		{32, 0, 32, 0, 0, 0, 0, 32},
		{74, 0, 32, 32, 0, 0, 0, 74},
		{32, 0, 0, 0, 0, 74, 0, 32},
		{32, 32,32,32, 74, 32, 32, 32}
	};

	for (auto x = 0; x < m_data.size(); x++)
	{
		for (auto y = 0; y < m_data[x].size(); y++)
		{
			if (m_data[x][y] == 98)
				m_doors[x + y * GetWidth()] = CreateRef<Door>();
		}
	}

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
		d.second->Update(deltaTime);
	}
}

bool Map::IsInBounds(const int x, const int y) const
{
	return x >= 0 && x < m_data.size() && y >= 0 && y < m_data[x].size();
}
