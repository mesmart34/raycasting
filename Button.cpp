#include "Button.h"


Button::Button(const std::string& text, const vec2& position, const vec2& size, std::function<void(Button*)> func)
	: Label(text, position, size), m_callback(func)
{

}

void Button::Update(const float deltaTime)
{
	auto mouse = Input::GetMousePosition();
	if (Input::IsMouseDown(SDL_BUTTON_LEFT))
	{
		if (mouse.x > GetPosition().x && mouse.x < GetPosition().x + m_size.x &&
			mouse.y > GetPosition().y && mouse.y < GetPosition().y + m_size.y)
		{
			m_callback(this);
		}
	}
}
