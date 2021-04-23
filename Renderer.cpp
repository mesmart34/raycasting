#include "Renderer.h"

Renderer::Renderer(SDL_Window* window, const int width, const int height) : m_width(width), m_height(height)
{
	auto renderFlags = (Uint32)(SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	m_renderer = SDL_CreateRenderer(window, -1, renderFlags);
	m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, width, height);
	SDL_RenderSetLogicalSize(m_renderer, width, height);
	m_data = new uint32_t[width * height];
	m_zBuffer = std::vector<float>(m_width);
	m_spriteScale = height / 130.0f;
	m_transparentColor = MathUtils::PackRGBA({ 152, 0, 136, 255 });
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

inline void Renderer::PutPixel(const int x, const int y, const Uint32 color)
{
	if (x >= 0 && y >= 0 && x < m_width && y < m_height)
		m_data[x + y * m_width] = color;
	/*else
		std::cout << "Put ERR!!!!" << std::endl;*/
}

void Renderer::DrawStrip(const Ray& ray, const Texture& atlas, const int x)
{
	m_zBuffer[x] = ray.Distance;
	auto id = ray.Id + 1;
	if (ray.Horizontal)
		id++;
	if (ray.DoorBox)
		id += ray.Horizontal ? 1 : 2;
	auto height = (int)floorf(m_height / ray.Distance);
	auto start = m_height / 2 - height / 2;
	auto end = m_height / 2 + height / 2;
	auto textureOffset = vec2();
	if (ray.Door)
		textureOffset = vec2(ray.TextureOffset, 0);
	auto txt = atlas.GetScaledColumn(ray.WallX, m_height, height, textureOffset, id);
	if (height > m_height) {
		start = 0;
		end = m_height;
	}
	for (auto j = start; j < end; j++)
		PutPixel(x, j, txt[j - start]);
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

void Renderer::DrawColumn(const std::vector<uint32_t>& data, const int x)
{
	memcpy(m_data + x * m_height, data.data(), data.size());
}

void Renderer::DrawSprite(const Sprite& sprite, const vec2& position, const Player& player)
{
	auto width = 64;
	auto height = 64;
	auto spritePos = position - player.GetPosition();
	auto invDet = 1.0f / (player.GetPlane().x * player.GetDirection().y - player.GetDirection().x * player.GetPlane().y);
	auto transform = vec2(
		invDet * (player.GetDirection().y * spritePos.x  - player.GetDirection().x * spritePos.y),
		invDet * (-player.GetPlane().y * spritePos.x + player.GetPlane().x * spritePos.y)
	);
	auto spritePositionX = ((m_width / 2) * (1 + transform.x / transform.y));

	auto spriteSize = ((height / transform.y)) * m_spriteScale * 2;
	auto drawStartY = m_height / 2 - spriteSize / 2;
	auto drawEndY = m_height / 2 + spriteSize / 2;

	auto drawStartX = spritePositionX - spriteSize / 2;
	auto drawEndX = spritePositionX + spriteSize / 2;

	for (auto stripe = drawStartX; stripe < drawEndX; stripe++)
	{
			if (transform.y > 0 && stripe >= 0 && stripe < m_width && transform.y < m_zBuffer[stripe]) {
			auto texX = ((stripe - drawStartX) / spriteSize);
			auto txt = sprite.Atlas->GetScaledColumn(texX, m_height, spriteSize, vec2(0, 0), sprite.Id);
			if (drawEndY - drawStartY > m_height) {
				drawStartY = 0;
				drawEndY = m_height;
			}
			for (auto j = drawStartY; j < drawEndY - 1; j++)
			{
				auto color = txt[j - drawStartY];
				if (color != m_transparentColor)
					PutPixel(stripe, j, color);
			}
		}
	}

}

void Renderer::SortObjects(std::vector<Object*>& m_objects, const Player& m_player)
{
	std::sort(m_objects.begin(), m_objects.end(), [=](auto p1, auto p2)->bool {
		auto d1 = vec2::sqrDistance(m_player.GetPosition(), p1->GetPosition());
		auto d2 = vec2::sqrDistance(m_player.GetPosition(), p2->GetPosition());
		return d1 > d2;
	});
}

int Renderer::GetWidth() const
{
	return m_width;
}

int Renderer::GetHeight() const
{
	return m_height;
}

void Renderer::DrawText(const std::string& text, const Font& font, const vec2& position, const uint32_t color)
{
	auto letterPosition = 0;
	for (auto s : text)
	{
		if (s == (char)32)
		{
			letterPosition += 5;
			continue;
		}
		auto meta = font.GetMeta(s);
		auto start = meta.first;
		auto length = meta.second;
		auto txt = font.GetTexture();
		DrawTexture(txt, color, position + vec2(letterPosition, 0), start, length, font.GetTexture()->GetHeight());
		letterPosition += length + 5;
	}
}

void Renderer::DrawTexture(const std::shared_ptr<Texture>& texture, const uint32_t drawColor, const vec2& position, const int start, const int letterWidth, const int letterHeight)
{
	for (auto x = 0; x < letterWidth; x++)
	{
		for (auto y = 0; y < letterHeight; y++)
		{
			auto index = (x + start) + y * texture->GetWidth();
			if ((*texture)[index] == m_transparentColor)
				continue;
			PutPixel(x + position.x, y + position.y, drawColor);

		}
	}
}
