#include "pch.h"
#include "TextBox.h"

int TextBox::m_focusedGlobalCount = 0;


TextBox::TextBox(const vec2& position, const vec2& size)
	: Label("", position, size)
{
	m_textAlignment = Left;
}

void TextBox::Update(const float deltaTime)
{
	
	if (Input::IsMouseDown(SDL_BUTTON_LEFT))
	{
		if (IsMouseInside(Input::GetMousePosition().x, Input::GetMousePosition().y))
		{
			m_focused = true;
			Input::SetTextInputMode(&m_text);
		}
		else {
			m_focused = false;
		}
	}

	m_timer += 1;
	if (m_timer > 20)
	{
		m_pointerVisibile = !m_pointerVisibile;
		m_timer -= 20;
	}
}

bool TextBox::IsPointerVisible() const
{
	return m_pointerVisibile;
}
