#pragma once
#include "vec2.h"
#include "Label.h"
#include "Input.h"
#include <functional>

class Button : public Label
{
public:
	Button() = default;
	Button(const std::string& text, const vec2& position, const vec2& size, std::function<void(Button*)> func);

	virtual void Update(const float deltaTime) override;
private:
	std::function<void(Button*)> m_callback;
};