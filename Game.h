#pragma once
#include "SDL/SDL.h"
#include <vector>
#include <map>
#include "vec2.h"
#include <cmath>
#include <iostream>
#include "Raycaster.h"
#include "Texture.h"
#include "Event.h"
#include "Physics.h"
#include "Window.h"
#include "Renderer.h"
#include "Object.h"
#include "Enemy.h"
#include "Font.h"
#include <thread>

class Game {
public:
	Game(const int width, const int height);

	void Run();
private:
	void InitGameWorld();
	void Update(const float deltaTime);
	void Render();
	void DrawWorld();
	void DrawMap();
	void DrawObjects();
	void DrawUI();
	void DoPhysics();
	void CastRays(const int start, const int end);

private:
	Window m_window;
	Renderer m_renderer;
	bool m_running;
	std::vector<std::vector<int>> m_map;
	std::vector<Object*> m_objects;
	std::map<int, float> m_doors;
	Player m_player;
	Raycaster m_raycaster;
	EventHandler m_eventHandler;
	Texture m_wallTexture;
	Font m_miniFont;
	Font m_font;
	float m_fps;
};