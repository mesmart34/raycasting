#pragma once
#include "UIElement.h"
#include "Input.h"

enum TextAlignment
{
	Left, Middle, Right
};

class Label : public UIElement
{
public:

	Label() = default;
	Label(const std::string& text, const vec2& position, const vec2& size, const int fontSize = 10, const uint32_t backgroundColor = 0xAA111111, const uint32_t fontColor = 0xFFFFFFFF);

	std::string GetText() const;
	void SetText(const std::string& text);
	uint32_t GetFontColor() const;
	void SetAutoSize(const bool value);
	bool IsAutoSize() const;
	int GetFontSize() const;
	void SetFontSize(const int size);
	virtual void Update(const float deltaTime) override;
	void SetTextAlignment(TextAlignment ta);
	TextAlignment GetTextAlignment() const;

protected:
	std::string m_text;

private:
	uint32_t m_fontColor;
	int m_fontSize;
	bool m_autoSize;
	TextAlignment m_textAlignment;
};