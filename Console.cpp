#include "pch.h"
#include "Console.h"

using namespace std;

Ref<RaycastEngine> Console::m_engine;
stringstream  Console::m_stream;
vector<std::string> Console::m_consoleHistory;
bool Console::m_opened;

void Console::Init(const Ref<RaycastEngine>& engine)
{
	m_engine = engine;
}

void Console::Open()
{
	m_opened = true;
}

void Console::Close()
{
	m_opened = false;
}

void Console::AddLog(const std::string& data)
{
	if (m_consoleHistory.size() > 100)
		m_consoleHistory.erase(m_consoleHistory.begin());
	m_consoleHistory.push_back(data);
}

void Console::Draw(const Scope<Window>& window)
{
	cout << "awdawd" << endl;
	static bool reclaim_focus = true;
	if (!m_opened)
		return;
	ImGui::Begin("Console", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(window->GetWidth(), window->GetHeight() / 2.5));
	static const char* label = "";
	static char buffer[256];
	static bool AutoScroll = true;
	static bool ScrollToBottom = false;
	memset(buffer, 0, 256);
	const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
	ImGui::BeginChild("Console", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);

	for (auto i = m_consoleHistory.begin(); i != m_consoleHistory.end(); i++)
	{
		ImGui::Text(i->c_str());
	}



	if (ScrollToBottom || (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
		ImGui::SetScrollHereY(1.0f);
	ScrollToBottom = false;

	ImGui::EndChild();
	ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
	ImGui::PushItemWidth(ImGui::GetWindowWidth());
	if (ImGui::InputText("Input", buffer, 256))
	{
		reclaim_focus = true;
	}
	ImGui::PopItemWidth();
	if (Input::IsKeyDown(SDL_SCANCODE_RETURN))
	{
		if (strlen(buffer) > 0)
		{
			auto command = string(buffer);
			ProcessCommand(command);
			memset(buffer, 0, 256);
		}
		reclaim_focus = true;
	}

	ImGui::SetItemDefaultFocus();
	if (reclaim_focus)
		ImGui::SetKeyboardFocusHere(-1);
	ImGui::End();
}

void Console::Clear()
{
	m_consoleHistory.clear();
}

bool Console::IsOpened()
{
	return m_opened;
}

void Console::ProcessCommand(const std::string& line)
{
	auto command = StringUtils::ParseCommand(line, " ");
	AddLog("> " + line);
	m_engine->OnConsoleCommand(command);
}
