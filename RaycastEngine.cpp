#include "RaycastEngine.h"

using namespace std;


RaycastEngine::RaycastEngine(const int width, const int height) : m_renderer(Renderer(m_window, width, height))
{
	Input::SetScale(m_renderer.GetScale());
	m_eventHandler = EventHandler();
	InitGameWorld();
}

void RaycastEngine::Run()
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

void RaycastEngine::InitGameWorld()
{
	m_map = Map(vec2(5, 5));

	auto plane = vec2();
	plane.y = (m_renderer.GetWidth() / 304.0f) * 0.99f;
	plane.y /= m_renderer.GetHeight() / 152.0f;
	m_player = Player(m_map.GetPlayerSpawnPoint(), 0.0f, plane);
	m_raycaster = Raycaster();
	m_wallTexture = Texture("textures/walls.png", 6, 19);

	auto atlas = CreateRef<Texture>("textures/atlas.png", 5, 10);
	auto ss_sprite = Sprite();
	ss_sprite.Atlas = CreateRef<Texture>("textures/ss.bmp", 8, 7);

	auto trex_sprite = Sprite();
	trex_sprite.Atlas = CreateRef<Texture>("textures/trex.bmp", 8, 7);

	auto sprite = Sprite();
	sprite.Id = 11;
	sprite.Atlas = atlas;

	auto lampSprite = Sprite();
	lampSprite.Id = 17;
	lampSprite.Atlas = atlas;
	m_map.AddObject(CreateRef<Object>(sprite, vec2(1.5, 1.5), true));
	m_map.AddObject(CreateRef<Object>(sprite, vec2(3.5, 3.5), true));
	m_map.AddObject(CreateRef<Enemy>(ss_sprite, vec2(4.5, 4.5), 0));
	m_map.AddObject(CreateRef<Enemy>(trex_sprite, vec2(4.5, 5.5), 0));
	m_map.AddObject(CreateRef<Object>(lampSprite, vec2(5.5, 5.5), false));
	m_map.AddObject(CreateRef<Object>(sprite, vec2(6.5, 6.5), true));
	
	m_fpsLabel = CreateRef<Label>("fps: ", vec2(0, 0), vec2(100, 100), 0xFFFFFFFF);
	m_uiElements.push_back(m_fpsLabel);
}

void RaycastEngine::Update(const float deltaTime)
{
	if (Input::IsKeyDown(SDL_SCANCODE_ESCAPE))
		Shutdown();
	m_player.Update(deltaTime);


	/*static float mouseY = 0.0f;
	mouseY -= Input::GetMouseAxis().y * deltaTime * 500;
	m_renderer.SetVerticalOffset(mouseY + 100);*/

	for (auto obj : m_map.GetObjects())
	{
		if (auto enemy = dynamic_cast<Enemy*>(obj.get()))
			enemy->Update(deltaTime, m_player);
	}
	if (Input::IsKeyDown(SDL_SCANCODE_SPACE))
		CastRay();
	m_map.UpdateDoors(deltaTime);
	DoPhysics();


	for (auto& ui : m_uiElements)
		ui->Update();
}

void RaycastEngine::Render()
{
	m_renderer.Clear();
	DrawWorld();
	DrawObjects();
	DrawUI();
	///DrawMap();
	m_renderer.Draw(m_window.GetWidth(), m_window.GetHeight());
}

void RaycastEngine::DrawWorld()
{
	for (auto strip = 0; strip < m_renderer.GetWidth(); strip++)
	{
		auto ray = m_raycaster.CastRay(strip, m_renderer.GetWidth(), m_player, m_map);
		m_renderer.DrawStrip(ray, m_wallTexture, strip);
	}
}

void RaycastEngine::DrawMap()
{
	
	/*for (auto x = 0; x < m_map.size(); x++)
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
		SDL_Color{ 50, 255, 100, 255 }), m_renderer.GetWidth());*/
}

void RaycastEngine::DrawObjects()
{
	m_renderer.SortObjects(m_map.GetObjects(), m_player);
	for (auto obj : m_map.GetObjects())
	{
		m_renderer.DrawSprite(obj->GetSprite(), obj->GetPosition(), m_player);
	}
}

void RaycastEngine::DrawUI()
{
	/*auto pos = m_player.GetPosition();
	auto x = (int)pos.x;
	auto y = (int)pos.y;
	auto text = to_string(x) + ", " + to_string(y);
	m_renderer.DrawText(text, m_miniFont, vec2(10, 10), 0.25f, 0xFFFFFFFF);

	text = to_string((int)m_fps);
	m_renderer.DrawText(text, m_miniFont, vec2(10, 30), 0.25f, 0xFFFFFFFF);


	text = to_string((int)tile);
	m_renderer.DrawText(text, m_miniFont, vec2(10, 50), 0.25f, 0xFFFFFFFF);*/
	m_fpsLabel->SetText(to_string((int)m_fps));
	for (auto& ui : m_uiElements)
		m_renderer.DrawUIElement(ui);
}

void RaycastEngine::DoPhysics()
{
	for (auto i = -1; i <= 1; i++)
	{
		for (auto j = -1; j <= 1; j++)
		{
			if (i == 0 && j == 0)
				continue;
			auto x = (int)(m_player.GetPosition().x + i);
			auto y = (int)(m_player.GetPosition().y + j);
			if (m_map.GetIndexAt(x, y) == 0)
				continue;
			if (m_map.GetIndexAt(x, y) == 98)
			{
				if(m_map.IsDoorOpened(x, y))
					continue;
			}
			auto delta = Physics::Intersection(m_player.GetRadius(), m_player.GetPosition(), vec2(x, y), vec2(1, 1));
			m_player.SetPosition(m_player.GetPosition() - delta);
		}
	}
	for (auto obj : m_map.GetObjects())
	{
		if (!obj->IsCollidable())
			continue;
		auto delta = Physics::Intersection(m_player.GetRadius(), m_player.GetPosition(), 0.3f, obj->GetPosition());
		m_player.SetPosition(m_player.GetPosition() - delta);
	}
}

void RaycastEngine::CastRay()
{
	auto ray = m_player.GetPosition();

	for (auto step = 0.0f; step < 20; step += 0.5f)
	{
		ray += vec2(
			step * cosf(MathUtils::DegToRad(m_player.GetAngle())),
			step * sinf(MathUtils::DegToRad(m_player.GetAngle()))
			);
		auto tile = m_map.GetIndexAt(ray.x, ray.y);
		if (tile == -1)
			break;
		
		if (tile == 98 && step < 1.25f)
			m_map.OpenDoorAt(ray.x, ray.y);

		if (tile > 0)
		{
			
			break;
		}

	}

}

void RaycastEngine::Shutdown()
{
	m_running = false;
}

