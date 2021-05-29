#include "pch.h"
#include "Map.h"


int Map::GetIndexAt(const int x, const int y) const
{
	if (IsInBounds(x, y))
		return m_data[x][y];
	return -1;
}

void Map::AddObject(const Ref<Object>& obj)
{
	obj->SetID(m_objectCounter);
	m_objects.push_back(obj);
	m_objectCounter++;
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

void Map::SetPlayerSpawnPoint(const vec2& point)
{
	m_playerSpawnPoint = point;
}

std::vector<std::vector<int>>& Map::GetData()
{
	return m_data;
}

void Map::EraseObjectWithId(const int id)
{
	for (auto i = m_objects.begin(); i != m_objects.end(); i++)
	{
		if ((*i)->GetID() == id)
		{
			m_objects.erase(m_objects.begin() + id);
			break;
		}
	}
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

Map::Map()
	: m_objectCounter(0)
{

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
