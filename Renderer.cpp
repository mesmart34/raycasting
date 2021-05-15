#include "pch.h"
#include "Renderer.h"

Renderer::Renderer(Window& window, const int width, const int height) : m_width(width), m_height(height)
{
	m_data = new uint32_t[m_width * m_height]();
	m_zBuffer = std::vector<float>(m_width);
	m_scale = height / 130.0f;
	m_transparentColor = MathUtils::PackRGBA({ 152, 0, 136, 255});
	m_renderVerticalOffset = 0;
	m_miniFont = Font("mini");
	m_font = Font("font");

	glGenTextures(1, &m_quadTextureId);
	glBindTexture(GL_TEXTURE_2D, m_quadTextureId); 
	glEnable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)m_data);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Renderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
	//memset(m_data, 0, m_width * m_height);
	auto floor = MathUtils::PackRGBA(SDL_Color{ 56, 56, 56, 255 });
	auto ceil = MathUtils::PackRGBA(SDL_Color{ 113, 113, 113, 255 });
	auto middle = m_height / 2 + m_renderVerticalOffset / m_maxDistance;
	auto color = ceil;
	for (auto y = 0; y < m_height; y++)
	{
		if (y > middle)
			color = floor;
		memset(m_data + y * m_width, color, m_width * sizeof(uint32_t));
	}
}

inline void Renderer::PutPixel(const int x, const int y, const Uint32 color)
{
	if (x >= 0 && y >= 0 && x < m_width && y < m_height)
			m_data[x + (m_height - y - 1) * m_width] = color;
}

inline uint32_t Renderer::GetPixel(const int x, const int y)
{
	return m_data[x + (m_height - 1 - y) * m_width];
}

void Renderer::DrawStrip(const Ray& ray, const Texture& atlas, const int x, const bool isPlayerInside)
{
	m_zBuffer[x] = ray.Distance;
	auto id = ray.Id + 1;
	if (ray.DoorBox || (isPlayerInside))
		id = 101;
	if (ray.Horizontal)
		id++;   
	auto height = (int)floorf(m_height / ray.Distance);
	auto offset = m_renderVerticalOffset / m_zBuffer[x];
	auto start = m_height / 2 - height / 2;
	auto end = m_height / 2 + height / 2;
	auto textureOffset = vec2();
	if (ray.Door)
		textureOffset = vec2(ray.TextureOffset, 0);
	if (height > m_height) {
		start = 0;
		end = m_height;
	}
	static auto column = new uint32_t[m_height];
	atlas.GetScaledColumn(column, ray.WallX, m_height, height, textureOffset, id, offset);
	/*float density = 0.2;
	float gradient = 3.0;
	auto visibility = expf(-powf(ray.Distance * density, -gradient));
	visibility = MathUtils::Clamp(visibility, 0, 1);
	auto blendWithFog = ray.Distance > 5;*/
	for (auto j = start; j < end; j++)
	{
		//auto color = column[j - start];
		//color = GetBlendedColor(color, MathUtils::PackRGBA({100, 100, 100, (uint8_t)(visibility * 255)}));
		//color = GetBlendedColor(color, MathUtils::PackRGBA({100, 100, 100, (uint8_t)(visibility * 255)}));
		PutPixel(x, j, column[j - start]);
	}
}

void Renderer::Draw()
{
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, m_quadTextureId);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, m_data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	static float scale = 1.0f;
	glVertex2f(-1.0f * scale, -1.0f * scale);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(1.0f * scale, -1.0f * scale);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(1.0f * scale, 1.0f * scale);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(-1.0f * scale, 1.0f * scale);
	glTexCoord2f(0.0f, 0.0f);
	glEnd();
}

void Renderer::DrawRect(const vec2& start, const vec2& size, const uint32_t color)
{
	for (auto x = (int)start.x; x < (int)start.x + size.x; x++)
	{
		for (auto y = (int)start.y; y < (int)start.y + size.y; y++)
		{
			PutPixel(x, y, GetBlendedColor(GetPixel(x, y), color));
			//PutPixel(x, y, color);
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

	auto spriteSize = ((height / transform.y)) * m_scale * 2;
	auto drawStartY = (int)(m_height / 2 - spriteSize / 2);
	auto drawEndY = (int)(m_height / 2 + spriteSize / 2);

	auto drawStartX = (int)(spritePositionX - spriteSize / 2);
	auto drawEndX = (int)(spritePositionX + spriteSize / 2);

	for (auto stripe = drawStartX; stripe < drawEndX; stripe++)
	{
			if (transform.y > 0 && stripe >= 0 && stripe < m_width && transform.y < m_zBuffer[stripe]) {
			auto texX = ((stripe - drawStartX) / spriteSize);
			static auto column = new uint32_t[m_height];
			sprite.Atlas->GetScaledColumn(column, texX, m_height, spriteSize, vec2(0, 0), sprite.Id, m_renderVerticalOffset / transform.y);
			if (drawEndY - drawStartY > m_height) {
				drawStartY = 0;
				drawEndY = m_height;
			}
			for (auto j = drawStartY; j < drawEndY - 1; j++)
			{
				auto color = column[j - drawStartY];
				if (color != m_transparentColor)
					PutPixel(stripe, j, color);
			}
		}
	}

}

void Renderer::SortObjects(std::vector<Ref<Object>>& m_objects, const Player& m_player)
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

void Renderer::DrawText(const std::string& text, const Font& font, const vec2& position, const float size, const uint32_t color)
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
		DrawTexture(txt, color, position + vec2(letterPosition, 0), start, length, font.GetTexture()->GetHeight(), vec2(size, size));
		letterPosition += (length + 5) * m_scale * size;
	}
}

void Renderer::DrawTexture(const std::shared_ptr<Texture>& texture, const uint32_t drawColor, const vec2& position, const int start, const int letterWidth, const int letterHeight, const vec2& size)
{
	auto size_x = letterWidth * m_scale * size.x;
	auto size_y = letterHeight * m_scale * size.y;
	auto step_x = 1.0f * letterWidth / size_x;
	auto step_y = 1.0f * letterHeight / size_y;
	auto tex_x = 0.0f;
	for (auto x = 0; x < size_x; x++)
	{
		auto tex_y = 0.0f;
		for (auto y = 0; y < size_y; y++)
		{
			auto index = ((int)tex_x + start) + (int)tex_y * texture->GetWidth();
			auto color = (uint32_t)(*texture)[index];
			auto pos_x = (int)(x + position.x);
			auto pos_y = (int)(y + position.y);
			if (color != (uint32_t)m_transparentColor)
				PutPixel(pos_x, pos_y, drawColor);
			tex_y += step_y;

		}
		tex_x += step_x;
	}
}

float Renderer::GetScale() const
{
	return m_scale;
}

void Renderer::SetVerticalOffset(const float value)
{
	m_renderVerticalOffset = value;
}

inline uint32_t Renderer::GetBlendedColor(const uint32_t a, const uint32_t b) const
{
	auto temp_a = *(SDL_Color*)&a;
	auto temp_b = *(SDL_Color*)&b;
	auto alpha = temp_b.a / 255.0f;
	auto temp_res = SDL_Color();
	temp_res.r = MathUtils::Lerp(temp_a.r, temp_b.r, alpha);
	temp_res.g = MathUtils::Lerp(temp_a.g, temp_b.g, alpha);
	temp_res.b = MathUtils::Lerp(temp_a.b, temp_b.b, alpha);
	auto result = MathUtils::PackRGBA({ (uint8_t)temp_res.r, (uint8_t)temp_res.g, (uint8_t)temp_res.b, 255 });
	return result;
}

void Renderer::DrawUIElement(Ref<UIElement> element)
{
	if (!element->IsEnabled())
		return;
	DrawUIElementWithChildren(element);
	for (auto c : element->GetChildren())
		DrawUIElement(c);
}

void Renderer::DrawUIElementWithChildren(Ref<UIElement> element)
{
	if (auto textBox = dynamic_cast<TextBox*>(element.get()))
	{
		DrawTextBox(textBox);
	}
	else if (auto label = dynamic_cast<Label*>(element.get()))
	{
		DrawLabel(label);
	}
	else if (auto panel = dynamic_cast<Panel*>(element.get()))
	{
		DrawRect(panel->GetPosition(), panel->GetSize(), panel->GetBackgroundColor());
	}
}

void Renderer::SetMaxDistance(const float maxDistance)
{
	m_maxDistance = maxDistance;
}

SDL_Renderer* Renderer::GetRenderer()
{
	return m_renderer;
}

void Renderer::Present()
{
	SDL_RenderPresent(m_renderer);
}

void Renderer::DrawButton(const Button* const button)
{
}

void Renderer::DrawLabel(const Label* const label)
{
	DrawRect(label->GetPosition(), label->GetSize(), label->GetBackgroundColor());
	auto fontSize = label->GetFontSize() * 0.01f;
	auto textSize = m_miniFont.GetTextSize(label->GetText()) * m_scale * fontSize;
	auto coef = 1.0f;
	if (label->IsAutoSize())
		coef = textSize.x / label->GetSize().x;
	auto pos = label->GetPosition() + label->GetSize() * 0.5f - textSize * (0.5f / coef);
	if (label->GetTextAlignment() == Left)
		pos.x = label->GetPosition().x + 10;
	else if (label->GetTextAlignment() == Right)
		pos.x = label->GetPosition().x + label->GetSize().x - textSize.x - 10;

	DrawText(label->GetText(), m_miniFont, pos, fontSize / coef, label->GetFontColor());
}

void Renderer::DrawTextBox(const TextBox* const textBox)
{
	DrawRect(textBox->GetPosition(), textBox->GetSize(), textBox->GetBackgroundColor());
	auto fontSize = textBox->GetFontSize() * 0.01f;
	auto textSize = m_miniFont.GetTextSize(textBox->GetText()) * m_scale * fontSize;
	auto coef = 1.0f;
	if (textBox->IsAutoSize())
		coef = textSize.x / textBox->GetSize().x;
	auto pos = textBox->GetPosition() + textBox->GetSize() * 0.5f - textSize * (0.5f / coef);
	if (textBox->GetTextAlignment() == Left)
		pos.x = textBox->GetPosition().x + 10;
	else if (textBox->GetTextAlignment() == Right)
		pos.x = textBox->GetPosition().x + textBox->GetSize().x - textSize.x - 10;
	if(textBox->IsFocused() && textBox->IsPointerVisible())
		DrawRect(pos + textSize * vec2(1, 0), vec2(2, textSize.y * (1.0f / coef)), 0xFFAAAAAA);



	DrawText(textBox->GetText(), m_miniFont, pos, fontSize / coef, textBox->GetFontColor());
}
