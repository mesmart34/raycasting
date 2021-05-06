#include "pch.h"
#include "Label.h"


Label::Label(
	const std::string& text, 
	const vec2& position, 
	const vec2& size,
	const int fontSize, 
	const uint32_t backgroundColor,
	const uint32_t fontColor)
	: m_text(text), UIElement(position, size, backgroundColor), m_fontColor(fontColor), m_fontSize(fontSize), m_autoSize(false), m_textAlignment(Middle)
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

uint32_t Label::GetFontColor() const
{
	return m_fontColor;
}

void Label::SetAutoSize(const bool value)
{
	m_autoSize = value;
}

bool Label::IsAutoSize() const
{
	return m_autoSize;
}

int Label::GetFontSize() const
{
	return m_fontSize;
}

void Label::SetFontSize(const int size)
{
	m_fontSize = size;
}

void Label::Update(const float deltaTime)
{
	
}

void Label::SetTextAlignment(TextAlignment ta)
{
	m_textAlignment = ta;
}

TextAlignment Label::GetTextAlignment() const
{
	return m_textAlignment;
}
