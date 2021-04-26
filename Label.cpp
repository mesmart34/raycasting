#include "Label.h"

Label::Label(const std::string& text, const vec2& position, const vec2& size, const uint32_t color)
	: m_text(text), UIElement(position, size, color)
{

}

std::string Label::GetText() const
{
	return m_text;
}

void Label::SetText(const std::string& text)
{
	m_text = text;
}

void Label::Update()
{

}