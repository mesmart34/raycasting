#include "pch.h"
#include "UIManager.h"


UIManager::UIManager()
{
	m_root = CreateRef<UIElement>();
	m_root->SetEnable(true);
	SDL_StartTextInput();
}

void UIManager::Update(const float deltaTime)
{
	if (Input::IsMouseDown(SDL_BUTTON_LEFT))
		Input::SetTextInputMode(nullptr);
	UpdateElement(deltaTime, m_root);
}

void UIManager::AddElementToRoot(const Ref<UIElement>& ui)
{
	m_root->AddChild(ui);
}

Ref<UIElement> UIManager::GetRootElement() const
{
	return m_root;
}

void UIManager::UpdateElement(const float deltaTime, const Ref<UIElement>& root)
{
	if (!root->IsEnabled())
		return;
	root->Update(deltaTime);
	auto mouse = Input::GetMousePosition();
	if (mouse.x > root->GetPosition().x && mouse.x < root->GetPosition().x + root->GetSize().x &&
		mouse.y > root->GetPosition().y && mouse.y < root->GetPosition().y + root->GetSize().y)
	{
		if(!root->IsFocused())
			root->OnMouseEnter(m_focusElement);

	}
	for (auto &ui : root->GetChildren())
	{
		UpdateElement(deltaTime, ui);
	}
}
