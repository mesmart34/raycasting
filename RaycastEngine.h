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
#include "UDPServer.h"
#include "UDPClient.h"
#include "Console.h"
#include "NetPlayer.h"

class RaycastEngine {
public:
	RaycastEngine(const int width, const int height);

	static int SDLCALL Quit(void* data, SDL_Event* ev);
	void Run();
private:
	void ConnectToServer(const std::string& ip, const int port);
	void InitGameWorld();
	void InitNetworking();
	void Update(const float deltaTime);
	void Render();
	void DrawWorld();
	void DrawObjects();
	void DrawUI();
	void DoPhysics();
	void Use();
	void Attack();
	void Shutdown();

	friend class Console;

private:
	Window m_window;
	Renderer m_renderer;
	bool m_running;
	Player m_player;
	Raycaster m_raycaster;
	EventHandler m_eventHandler;
	Texture m_wallTexture;
	UIManager m_uiManager;
	Ref<Console> m_console;
	float m_fps;
	Map m_map;
	Ref<Label> m_fpsLabel;
	std::vector<Ref<UIElement>> m_uiElements;
	Ref<Label> playerPosText;
	Ref<Panel> panel;
	Scope<UDPClient> m_client;
	Scope<UDPServer> m_server;
	Ref<NetPlayer> m_secondPlayer;
};