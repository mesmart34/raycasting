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
#include <future>
#include "UIManager.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "UDPClient.h"
#include "Console.h"
#include "NetPlayer.h"
#include "ResourceManager.h"

class RaycastEngine {
public:
	RaycastEngine(const int width, const int height);

	static int SDLCALL WindowEventListener(void* data, SDL_Event* ev);
	void Run();
private:
	void ConnectToServer(const std::string& ip, const int port);
	void DisconnectFromServer();
	void InitGameWorld();
	void InitNetworking();
	void Update(const float deltaTime);
	void UpdateNetwork(const float deltaTime);
	void Render();
	void DrawWorld();
	void DrawObjects();
	void DrawUI();
	void DoPhysics();
	void Use();
	void Attack();
	void Shutdown();
	void LoadLevel(Map* map);
	void SolveWallCollision(vec2& position, const float radius);

	friend class Console;

private:
	Scope<Window>					m_window;
	Scope<Renderer>					m_renderer;
	Scope<Raycaster>				m_raycaster;
	Scope<EventHandler>				m_eventHandler;
	Scope<Map>						m_map;
	Scope<UDPClient>				m_client;
	Scope<UIManager>				m_uiManager;
	Scope<Player>					m_player;
	Ref<Texture>					m_wallTexture;
	bool							m_running;
	float							m_fps;
	std::vector<Scope<UIElement>>	m_uiElements;
	std::map<int, Ref<NetPlayer>>	m_players;
};