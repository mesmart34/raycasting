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
#include <queue>
#include <mutex>
#include "Map.h"
#include "Base.h"
#include "UIElement.h"
#include "Label.h"
#include "Button.h"
#include "Panel.h"
#include "TextBox.h"

class RaycastEngine {
public:
	RaycastEngine(const int width, const int height);

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
	void CastRay();
	void Shutdown();
	
	void ThreadRender(const int start, const int end);

private:
	Window m_window;
	Renderer m_renderer;
	bool m_running;
	Player m_player;
	Raycaster m_raycaster;
	EventHandler m_eventHandler;
	Texture m_wallTexture;
	float m_fps;
	Map m_map;
	Ref<Label> m_fpsLabel;
	std::vector<Ref<UIElement>> m_uiElements;
	static std::mutex m_mutex;
	Ref<Label> playerPosText;
	Ref<Panel> panel;
	//std::vector<std::vector<uint32_t>> m_buffer;
};