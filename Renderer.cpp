#include "Renderer.h"

Renderer::Renderer(const Window& window, const int width, const int height) : m_width(width), m_height(height)
{
	auto renderFlags = (Uint32)(SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	m_renderer = SDL_CreateRenderer(window.GetSDLPtr(), -1, renderFlags);
	m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, width, height);
	SDL_RenderSetLogicalSize(m_renderer, width, height);
	m_data = new uint32_t[width * height];
}

void Renderer::Clear()
{
	for (auto x = 0; x < m_width; x++)
	{
		for (auto y = 0; y < m_height; y++)
		{
			if (y <= m_height / 2)
				m_data[x + y * m_width] = MathUtils::PackRGBA(SDL_Color{ 56, 56, 56, 255 });
			else
				m_data[x + y * m_width] = MathUtils::PackRGBA(SDL_Color{ 113, 113, 113, 255 });
		}
	}
}

void Renderer::PutPixel(const int x, const int y, const Uint32 color)
{
	if(x >= 0 && y >= 0 && x < m_width && y < m_height)
		m_data[x + y * m_width] = color;
}

void Renderer::DrawWallStrip(const Ray& ray, const Texture& texture, const int x)
{
	auto height = (int)floorf(m_height / ray.Distance);
	//std::cout << height << std::endl;
	auto start = m_height / 2 - height / 2;
	auto end = m_height / 2 + height / 2;
	auto offset = vec2((ray.Id - 1) % 6, (ray.Id - 1) / 6);
	if (ray.Horizontal)
		offset.x += 1;
	auto txt = texture.GetScaledColumn(m_height, 64, height, ray.WallX * texture.GetWidth(), m_height, offset);
	for (auto j = start; j < end; j++)
	{
		PutPixel(x, j, txt[(j - start) % height]);
	}
	/*for (auto j = start; j < end; j++)
	{
		PutPixel(x, j, txt[(j - start) % height]);
	}*/
}

void Renderer::Draw()
{
	SDL_UpdateTexture(m_texture, nullptr, &m_data[0], sizeof(uint32_t) * m_width);
	auto dst = SDL_Rect{ 0, 0, m_width, m_height };
	SDL_RenderCopy(m_renderer, m_texture, nullptr, &dst);
	SDL_RenderPresent(m_renderer);
}

void Renderer::DrawRect(Renderer& renderer, const vec2& start, const vec2& size, const uint32_t color, const int width)
{
	for (auto x = (int)start.x; x < (int)start.x + size.y; x++)
	{
		for (auto y = (int)start.y; y < (int)start.y + size.y; y++)
		{
			renderer.PutPixel(x, y, color);
		}
	}

}

int Renderer::GetWidth() const
{
	return m_width;
}

int Renderer::GetHeight() const
{
	return m_height;
}
