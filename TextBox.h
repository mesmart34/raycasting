#pragma once

#include "UIElement.h"
#include "Label.h"
#include <string>
#include <iostream>
#include <sstream>


class TextBox : public Label
{
public:
	TextBox() = default;
	TextBox(const vec2& position, const vec2& size);


	virtual void Update(const float deltaTime) override;
	bool IsPointerVisible() const;

private:
	bool m_pointerVisibile;
	float m_timer;
	static int m_focusedGlobalCount;

};