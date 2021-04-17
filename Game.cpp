#include "Game.h"

using namespace std;

vector<vector<int>> Game::m_map = { 
	{1, 1, 2, 1, 1, 1}, 
	{1, 0, 0, 0, 0, 1}, 
	{1, 0, 0, 0, 0, 1}, 
	{1, 0, 0, 0, 0, 3}, 
	{1, 0, 0, 0, 0, 1}, 
	{1, 1, 1, 1, 1, 1} 
};

Game::Game(const int width, const int height) : m_width(width), m_height(height)
{
	auto windowFlags = (Uint32)(SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	m_window = SDL_CreateWindow("Wolf3D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, windowFlags);
	auto renderFlags = (Uint32)(SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	m_renderer = SDL_CreateRenderer(m_window, -1, renderFlags);
	m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
	SDL_RenderSetLogicalSize(m_renderer, m_width, m_height);
	fieldOfView = 90.0f;
	playerPosition = vec2(3, 3);
	wallHeight = m_height;
	viewDistance = 20.0f;
	direction = vec2(-1, 0);
	playerPlane = vec2(0, 0.96f);
}

void Game::Run()
{
	m_running = true;
	auto lastTime = SDL_GetPerformanceCounter();
	while (m_running)
	{
		HandleEvents();
		auto currentTime = SDL_GetPerformanceCounter();
		auto timeSlice = currentTime - lastTime;
		auto deltaTime = timeSlice / (float)SDL_GetPerformanceFrequency();
		Update(deltaTime);
		Render();
		lastTime = currentTime;
	}
}

void Game::HandleEvents()
{
	auto event = SDL_Event();
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
		{
			if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
				m_running = false;
		} break;
		default:
			break;
		}
	}
}

void Game::Update(const float deltaTime)
{
	playerAngle += deltaTime * 10;
	if (playerAngle > 360)
		playerAngle = 0;
	if (playerAngle < 0)
		playerAngle = 360;
	direction = RotateVector(direction, playerAngle);
	playerPlane = RotateVector(playerPlane, playerAngle);
	playerAngle = 0;
}

void Game::Render()
{
	auto data = new uint32_t[(int64_t)m_width * (int64_t)m_height];
	ClearScreen(data);
	
	DrawWorld(data);

	SDL_UpdateTexture(m_texture, nullptr, &data[0], sizeof(uint32_t) * m_width);
	auto dst = SDL_Rect{0, 0, m_width, m_height};
	SDL_RenderCopy(m_renderer, m_texture, nullptr, &dst);
	SDL_RenderPresent(m_renderer);
	delete[] data;
}

uint32_t Game::PackRGBA(const SDL_Color color) const
{
	return (uint32_t)(color.r << 24 | color.g << 16 | color.b << 8 | color.a << 0);
}

void Game::DrawWorld(uint32_t data[])
{
	for (auto strip = 0; strip < m_width; strip++)
	{
		auto ray = CastRay(strip);
		DrawWall(data, ray, strip);
	}
}

float Game::DegToRag(const float deg) const
{
	return deg * M_PI / 180.0f;
}

void Game::DrawWall(uint32_t data[], const Ray& ray, const int x)
{
	auto height = (int)floorf(wallHeight / ray.Distance);
	auto start = m_height / 2 - height / 2;
	if (start < 0)
		start = 0;
	auto end = start + height;
	if (end > m_height)
		end = m_height - 1;
	auto color = SDL_Color{ 100, 50, 50, 255 };
	if (ray.Id > 1)
		color = SDL_Color{50, 25, 25, 255};
	auto drawColor = PackRGBA(color);
	if (ray.Horizonal)
		drawColor = (drawColor) & 8355711;
	for (auto j = start; j < end; j++)
		data[x + j * m_width] = drawColor;
}

vec2 Game::RotateVector(const vec2& other, const float angle) const
{
	return vec2(
		other.x * cosf(DegToRag(angle)) - other.y * sinf(DegToRag(angle)),
		other.x * sinf(DegToRag(angle)) + other.y * cosf(DegToRag(angle))
	);
}

Ray Game::CastRay(const int strip) const
{
	auto camera_x = 2 * strip / (float)m_width - 1;
	auto rayDir = vec2(
		direction.x + playerPlane.x * camera_x,
		direction.y + playerPlane.y * camera_x
	);
	auto deltaDist = vec2(
		(rayDir.y == 0) ? 0 : ((rayDir.x == 0) ? 1 : fabs(1 / rayDir.x)),
		(rayDir.x == 0) ? 0 : ((rayDir.y == 0) ? 1 : fabs(1 / rayDir.y))
	);
	auto step = vec2(0, 0);
	auto sideDist = vec2(0, 0);
	auto horizontal = false;
	auto map = vec2((int)playerPosition.x, (int)playerPosition.y);
	if (rayDir.x < 0)
		sideDist.x = (playerPosition.x - map.x) * deltaDist.x;
	else 
		sideDist.x = (map.x + 1.0f - playerPosition.x) * deltaDist.x;
	step.x = rayDir.x < 0 ? -1 : 1;

	if (rayDir.y < 0)
		sideDist.y = (playerPosition.y - map.y) * deltaDist.y;
	else
		sideDist.y = (map.y + 1.0f - playerPosition.y) * deltaDist.y;
	step.y = rayDir.y < 0 ? -1 : 1;

	while (true)
	{
		if (sideDist.x < sideDist.y)
		{
			sideDist.x += deltaDist.x;
			map.x += step.x;
			horizontal = false;
		}
		else
		{
			sideDist.y += deltaDist.y;
			map.y += step.y;
			horizontal = true;
		}
		if (m_map[(int)map.x][(int)map.y] > 0) 
			break;
	}
	auto distance = 0.0f;
	if (horizontal == false) 
		distance = (map.x - playerPosition.x + (1 - step.x) / 2) / rayDir.x;
	else
		distance = (map.y - playerPosition.y + (1 - step.y) / 2) / rayDir.y;
	return Ray(distance, horizontal, m_map[(int)map.x][(int)map.y]);
}

void Game::ClearScreen(uint32_t data[])
{
	for (auto x = 0; x < m_width; x++)
	{
		for (auto y = 0; y < m_height; y++)
		{
			if(y <= m_height / 2)
				data[x + y * m_width] = PackRGBA(SDL_Color{56, 56, 56, 255});
			else
				data[x + y * m_width] = PackRGBA(SDL_Color{113, 113, 113, 255});
		}
	}
}
