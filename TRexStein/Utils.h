#pragma once
#include "Map.h"
#include <string>
#include "Enemy.h"
#include "NetPlayer.h"
#include <fstream>
#include "Physics.h"


class __declspec(dllexport) Utils
{
public:
	static Map* LoadMap(const std::string& name);
	static void SaveMap(const Scope<Map>& map, const std::string& name);
	static void SolveWallCollision(const Scope<Map>& map, vec2& position, const float radius);
	static void DoPhysics(const Scope<Map>& map);
};

