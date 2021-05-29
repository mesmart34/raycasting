#pragma once
#include "SDL/SDL.h"
#include "SDL/SDL_main.h"
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
#include "ResourceManager.h"

#undef main

#define TREXSTEIN_API __declspec(dllexport)


class TREXSTEIN_API RaycastEngine {
public:
	RaycastEngine(const int width, const int height, const std::string& title);

	static int SDLCALL WindowEventListener(void* data, SDL_Event* ev);
	void Run();


protected:
	virtual void OnStart() = 0;
	virtual void OnUpdate(const float deltaTime) = 0;
	virtual void OnRender() = 0;
	friend class Console;
	//void DrawUI();
	void ConnectToServer(const std::string& ip, const int port);
	void DisconnectFromServer();
	void Shutdown();
	void DrawUI();

	virtual void OnConsoleCommand(const std::vector<std::string>& commandTokens) {};

private:
	void Update(const float deltaTime);
	void Render();

protected:
	Scope<Window>					m_window;
	Scope<Renderer>					m_renderer;
	Scope<UDPClient>				m_client;
	float							m_fps;


private:
	Scope<EventHandler>				m_eventHandler;
	bool							m_running;
	std::vector<Scope<UIElement>>	m_uiElements;

};

