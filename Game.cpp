#include "Game.h"

using namespace std;


Game::Game(const int width, const int height)
{
	m_window = Window();
	m_renderer = Renderer(m_window.GetSDLPtr(), width, height);
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
		//m_fps = (1.0f / deltaTime);
		//m_fps += m_fps / (1.0f / deltaTime);
		if (m_fps < 1.0f / deltaTime)
			m_fps += 0.1f;
		else m_fps -= 0.1f;
		SDL_SetWindowTitle(m_window.GetSDLPtr(), to_string(m_fps).c_str());
		Update(deltaTime);
		m_eventHandler.PostUpdate();
		Render();	
		lastTime = currentTime;
	}
}

void Game::InitGameWorld()
{
	m_map = {
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
		{32, 0, 32, 98, 32, 0, 0, 32},
		{32, 0, 98, 0, 0, 0, 0, 32},
		{32, 0, 32, 0, 0, 0, 0, 32},
		{74, 0, 0, 32, 0, 0, 0, 74},
		{32, 0, 0, 0, 0, 74, 0, 32},
		{32, 32,32,32, 74, 32, 32, 32}
	};
	auto plane = vec2();
	plane.y = (m_renderer.GetWidth() / 304.0f) * 0.99f;
	plane.y /= m_renderer.GetHeight() / 152.0f;
	m_player = Player(vec2(5, 5), 0.0f, plane);
	m_raycaster = Raycaster();
	m_wallTexture = Texture("textures/walls.png", 6, 19);
	for (auto x = 0; x < m_map.size(); x++)
	{
		for (auto y = 0; y < m_map[x].size(); y++)
		{
			if (m_map[x][y] == 99 || m_map[x][y] == 98)
				m_doors[x + y * m_map[0].size()] = 1.0f;
		}
	}

	auto atlas = std::make_shared<Texture>("textures/atlas.png", 5, 10);
	auto ss_sprite = Sprite();
	ss_sprite.Atlas = std::make_shared<Texture>("textures/ss.bmp", 8, 7);

	auto trex_sprite = Sprite();
	trex_sprite.Atlas = std::make_shared<Texture>("textures/trex.bmp", 8, 7);

	auto sprite = Sprite();
	sprite.Id = 11;
	sprite.Atlas = atlas;

	auto lampSprite = Sprite();
	lampSprite.Id = 17;
	lampSprite.Atlas = atlas;
	m_objects.push_back(new Object(sprite, vec2(1.5, 1.5), true));
	m_objects.push_back(new Object(sprite, vec2(3.5, 3.5), true));
	m_objects.push_back(new Enemy(ss_sprite, vec2(4.5, 4.5), 0));
	m_objects.push_back(new Enemy(trex_sprite, vec2(4.5, 5.5), 0));
	m_objects.push_back(new Object(lampSprite, vec2(5.5, 5.5), false));
	m_objects.push_back(new Object(sprite, vec2(6.5, 6.5), true));

	m_miniFont = Font("mini");
	m_font = Font("font");
}

void Game::Update(const float deltaTime)
{
	static float timer = 0;
	timer += deltaTime;
	for (auto &door : m_doors)
	{
		door.second = fabs(sinf(timer));
	}
	if (Input::IsKeyDown(SDL_SCANCODE_ESCAPE))
		m_running = false;
	m_player.Update(deltaTime);

	auto GetTile = [&](const int x, const int y)
	{
		if (x >= 0 && x < m_map.size() && y >= 0 && y < m_map[x].size())
			return m_map[x][y];
	};

	auto rayStep = MathUtils::Rotate(m_player.GetDirection(), m_player.GetAngle() + 45);
	auto ray = m_player.GetPosition();
	for (auto step = 0.0f; step < 20.0f; step++)
	{
		ray += rayStep;
		auto rounded = vec2::floor(ray);
		if(rounded.x > 0 && rounded.y > 0 && rounded.x < m_map[0].size() && rounded.y < m_map.size())

		if (m_map[(int)rounded.x][(int)rounded.y] != 0)
			break;
	}
	auto rayDistance = vec2::distance(m_player.GetPosition(), ray);
	cout << rayDistance << endl;


	for (auto obj : m_objects)
	{
		if (auto enemy = dynamic_cast<Enemy*>(obj))
			enemy->Update(deltaTime, m_player);
	}

	DoPhysics();
}

void Game::Render()
{
	m_renderer.Clear();
	DrawWorld();
	DrawObjects();
	DrawUI();
	///DrawMap();
	m_renderer.Draw();
}

void Game::DrawWorld()
{
	/*auto t1 = std::thread(&Game::CastRays, this, 0, m_renderer.GetWidth() / 2);
	auto t2 = std::thread(&Game::CastRays, this, m_renderer.GetWidth() / 2, m_renderer.GetWidth());
	auto render_thread = std::thread([=]() {
		while (m_rays.size() > 0)
		{
			m.lock();
			auto ray = m_rays.back();
			m_renderer.DrawStrip(ray.second, m_wallTexture, ray.first);
			m_rays.pop();
			m.unlock();
		}
	});
	t1.join();
	t2.join();*/
	//render_thread.join();
	//for (auto i = 0; i < 4; i++)
	//{
	//	auto th1 = std::thread(&Game::CastRays, this, i * (m_renderer.GetWidth() / 4), i * (m_renderer.GetWidth() / 4) + m_renderer.GetWidth() / 4);
	//	th1.detach();
	//	//CastRays(i * (m_renderer.GetWidth() / 4), i * (m_renderer.GetWidth() / 4) + m_renderer.GetWidth() / 4);

	//}
	/*CastRays(0, m_renderer.GetWidth() / 4);
	CastRays(m_renderer.GetWidth() / 4, m_renderer.GetWidth() / 2);
	CastRays(m_renderer.GetWidth() / 2, m_renderer.GetWidth() / 2 + m_renderer.GetWidth() / 4);
	CastRays(m_renderer.GetWidth() / 2 + m_renderer.GetWidth() / 4, m_renderer.GetWidth());*/
	/*for (auto strip = 0; strip < m_renderer.GetWidth(); strip++)
	{
		auto ray = m_raycaster.CastRay(strip, m_renderer.GetWidth(), m_map, m_player, m_doors);
		m_renderer.DrawStrip(ray, m_wallTexture, strip);
	}*/
	CastRays(0, m_renderer.GetWidth());
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
			m_renderer.DrawRect(m_renderer, vec2((m_map.size() - x) * 4, y * 4), vec2(4, 4), MathUtils::PackRGBA(color), m_renderer.GetWidth());
		}
	}
	m_renderer.DrawRect(m_renderer, 
		vec2(m_map.size() - m_player.GetPosition().x + 1, m_player.GetPosition().y) * 4,
		vec2(m_player.GetRadius(), m_player.GetRadius() * 4),
		MathUtils::PackRGBA(
		SDL_Color{ 50, 255, 100, 255 }), m_renderer.GetWidth());
}

void Game::DrawObjects()
{
	m_renderer.SortObjects(m_objects, m_player);
	for (auto obj : m_objects)
	{
		m_renderer.DrawSprite(obj->GetSprite(), obj->GetPosition(), m_player);
	}
}

void Game::DrawUI()
{
	auto pos = m_player.GetPosition();
	auto x = (int)pos.x;
	auto y = (int)pos.y;
	auto text = to_string(x) + ", " + to_string(y);
	m_renderer.DrawText(text, m_miniFont, vec2(10, 10), 0xFFFFFFFF);

	text = to_string((int)m_fps);
	m_renderer.DrawText(text, m_miniFont, vec2(10, 30), 0xFFFFFFFF);
}

void Game::DoPhysics()
{
	for (auto i = -1; i <= 1; i++)
	{
		for (auto j = -1; j <= 1; j++)
		{
			if (i == 0 && j == 0)
				continue;
			auto x = (int)(m_player.GetPosition().x + i);
			auto y = (int)(m_player.GetPosition().y + j);
			if (m_map[x][y] == 0)
				continue;
			auto delta = Physics::Intersection(m_player.GetRadius(), m_player.GetPosition(), vec2(x, y), vec2(1, 1));
			m_player.SetPosition(m_player.GetPosition() - delta);
		}
	}
	for (auto obj : m_objects)
	{
		if (!obj->IsCollidable())
			continue;
		auto delta = Physics::Intersection(m_player.GetRadius(), m_player.GetPosition(), 0.3f, obj->GetPosition());
		m_player.SetPosition(m_player.GetPosition() - delta);
	}
}

void Game::CastRays(const int start, const int end)
{
	for (auto strip = start; strip < end; strip++)
	{
		auto ray = m_raycaster.CastRay(strip, m_renderer.GetWidth(), m_map, m_player, m_doors);


		//m_rays.push(std::pair<int, Ray>(strip, ray));
		m.lock();
		m_renderer.DrawStrip(ray, m_wallTexture, strip);
		m.unlock();
	}
}
