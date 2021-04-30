#include "TextBox.h"


TextBox::TextBox(const vec2& position, const vec2& size)
	: Label("", position, size), m_focused(false)
{

}

void TextBox::Update(const float deltaTime)
{
	auto mouse = Input::GetMousePosition();
	if (Input::IsMouseDown(SDL_BUTTON_LEFT))
	{
		if (mouse.x > GetPosition().x && mouse.x < GetPosition().x + m_size.x &&
			mouse.y > GetPosition().y && mouse.y < GetPosition().y + m_size.y)
		{
			m_focused = true;
			Input::SetTextInputMode(true, &m_text);
		}else
		m_focused = false;
	}
	m_timer += 1;
		
	if (m_timer > 20)
	{
		m_pointerVisibile = !m_pointerVisibile;
		m_timer -= 20;
	}
}

bool TextBox::IsFocused() const
{
	return m_focused;
}

bool TextBox::IsPointerVisible() const
{
	return m_pointerVisibile;
}
