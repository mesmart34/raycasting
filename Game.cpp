#include "Game.h"

using namespace std;

vector<vector<int>> Game::m_map = { 
	{33, 33,33,33, 35, 33, 33, 33},
	{33, 0, 0, 0, 0, 0, 0, 33},
	{33, 0, 33, 0, 0, 0, 0, 33},
	{33, 0, 0, 0, 0, 0, 0, 33},
	{33, 0, 0, 0, 0, 0, 0, 33},
	{35, 0, 0, 33, 0, 0, 0, 35},
	{33, 0, 0, 0, 0, 33, 0, 33},
	{33, 33,33,33, 35, 33, 33, 33}
};

Game::Game(const int width, const int height)
{
	m_window = Window();
	m_renderer = Renderer(m_window, width, height);
	m_eventHandler = EventHandler();
	InitGameWorld();
}

void Game::Run()
{
	m_running = true;
	auto lastTime = SDL_GetPerformanceCounter();
	while (m_running)
	{
		m_eventHandler.Handle();
		auto currentTime = SDL_GetPerformanceCounter();
		auto timeSlice = currentTime - lastTime;
		auto deltaTime = timeSlice / (float)SDL_GetPerformanceFrequency();
		Update(deltaTime);
		m_eventHandler.PostUpdate();
		Render();	
		lastTime = currentTime;
	}
}

void Game::InitGameWorld()
{
	m_player = Player(vec2(5, 5), 0.0f);
	m_raycaster = Raycaster();
	m_wallTexture = Texture("textures/walls.png");
}

void Game::Update(const float deltaTime)
{
	if (Input::IsKeyDown(SDL_SCANCODE_ESCAPE))
		m_running = false;
	m_player.Rotate(-Input::GetMouseAxis().x * deltaTime * 50.0f);

	auto oldPosition = m_player.GetPosition();

	auto GetTile = [&](const int x, const int y)
	{
		if (x >= 0 && x < m_map.size() && y >= 0 && y < m_map[x].size())
			return m_map[x][y];
	};
	auto oldPlayerPos = m_player.GetPosition();
	if (Input::IsKeyPressed(SDL_SCANCODE_W))
		m_player.Move(m_player.GetDirection() * deltaTime * 2);
	else if (Input::IsKeyPressed(SDL_SCANCODE_S))
		m_player.Move(m_player.GetDirection() * deltaTime * -2);
	if (Input::IsKeyPressed(SDL_SCANCODE_A))
		m_player.Move(vec2(-m_player.GetDirection().y, m_player.GetDirection().x) * deltaTime * 2);
	else if (Input::IsKeyPressed(SDL_SCANCODE_D))
		m_player.Move(vec2(m_player.GetDirection().y, -m_player.GetDirection().x) * deltaTime * 2);
	auto newPos = m_player.GetPosition();
	auto delta = (m_player.GetPosition() - oldPlayerPos);

	if (delta.x <= 0) 
	{
		if (GetTile(newPos.x - m_player.GetRadius(), newPos.y) != 0)
		{
			newPos.x = (int)newPos.x + m_player.GetRadius();
		}
	}
	else 
	{
		if (GetTile(newPos.x + m_player.GetRadius(), newPos.y) != 0)
		{
			newPos.x = (int)newPos.x + 1 -  m_player.GetRadius();
		}
	}

	if (delta.y <= 0) 
	{
		if (GetTile(newPos.x, newPos.y - m_player.GetRadius()) != 0)
		{
			newPos.y = (int)newPos.y + m_player.GetRadius();
		}
	}
	else 
	{
		if (GetTile(newPos.x, newPos.y + m_player.GetRadius()) != 0)
		{
			newPos.y = (int)newPos.y + 1 +- m_player.GetRadius();
		}
	}

	m_player.SetPosition(newPos);
}

void Game::Render()
{
	m_renderer.Clear();
	DrawWorld();
	DrawMap();
	m_renderer.Draw();
}

void Game::DrawWorld()
{
	for (auto strip = 0; strip < m_renderer.GetWidth(); strip++)
	{
		auto ray = m_raycaster.CastRay(strip, m_renderer.GetWidth(), m_map, m_player);
		m_renderer.DrawWallStrip(ray, m_wallTexture, strip);
	}
}

void Game::DrawMap()
{
	
	for (auto x = 0; x < m_map.size(); x++)
	{
		for (auto y = 0; y < m_map[x].size(); y++)
		{
			auto color = SDL_Color{ 100, 100, 100, 100 };
			if (m_map[x][y] == 35)
				color = SDL_Color{ 255, 100, 100, 100 };
			else if (m_map[x][y] == 33)
				color = SDL_Color{ 200, 100, 100, 100 };
			m_renderer.DrawRect(m_renderer, vec2(x * 4, y * 4), vec2(4, 4), MathUtils::PackRGBA(color), m_renderer.GetWidth());
		}
	}
	m_renderer.DrawRect(m_renderer, 
		m_player.GetPosition() * 4, 
		vec2(m_player.GetRadius(), m_player.GetRadius() * 4),
		MathUtils::PackRGBA(
		SDL_Color{ 50, 255, 100, 255 }), m_renderer.GetWidth());
}