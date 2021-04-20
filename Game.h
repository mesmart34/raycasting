#pragma once
#include "SDL/SDL.h"
#include <vector>
#include "vec2.h"
#include <cmath>
#include <iostream>
#include "Raycaster.h"
#include "Ray.h"
#include "Texture.h"
#include "Event.h"
#include "Physics.h"
#include "Window.h"
#include "Renderer.h"

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

private:
	Window m_window;
	Renderer m_renderer;
	bool m_running;
	static std::vector<std::vector<int>> m_map;
	Player m_player;
	Raycaster m_raycaster;
	EventHandler m_eventHandler;
	Texture m_wallTexture;
};