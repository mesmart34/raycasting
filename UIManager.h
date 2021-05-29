#pragma once
#include "UIElement.h"
#include "TextBox.h"
#include "Button.h"
#include "Label.h"
#include "Panel.h"
#include <vector>
#include "Base.h"
#include "Input.h"

class __declspec(dllexport) UIManager
{
public:
	UIManager();
	~UIManager() = default;
	void Update(const float deltaTime);
	void AddElementToRoot(const Ref<UIElement>& ui);
	Ref<UIElement> GetRootElement() const;

	friend class TextBox;
private:
	void UpdateElement(const float deltaTime, const Ref<UIElement>& root);


private:
	Ref<UIElement> m_root;
	Ref<UIElement> m_focusElement;
};

