#include "pch.h"
#include "Console.h"

using namespace std;

Console::Console(const Ref<RaycastEngine>& engine) : m_opened(false), m_engine(engine)
{
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
	m_stream << data << (char)0xa;
}

void Console::Draw(const Window& window)
{
	static bool reclaim_focus = true;
	if (!m_opened)
		return;
	ImGui::Begin("Console", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(window.GetWidth(), 400));
	static const char* label = "";
	static char buffer[256];
	static bool AutoScroll = true;
	static bool ScrollToBottom = false;
	memset(buffer, 0, 256);
	static vector<string> consoleHistory;
	const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
	ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);
	ImGui::Text(m_stream.str().c_str());
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
			memset(buffer, 0, 256);
			ProcessCommand(command);
		}
		reclaim_focus = true;
	}

	ImGui::SetItemDefaultFocus();
	if (reclaim_focus)
		ImGui::SetKeyboardFocusHere(-1);
	ImGui::End();
}

bool Console::IsOpened() const
{
	return m_opened;
}

void Console::ProcessCommand(const std::string& line)
{
	auto command = ParseCommand(line, ' ');
	if (command[0] == "quit")
	{
		m_engine->Shutdown();
	}
	else if (command[0] == "connect")
	{
		auto clientInfo = ParseCommand(command[1], ':');
		auto ip = clientInfo[0];
		auto port = atoi(clientInfo[1].c_str());
		AddLog(line);
		m_engine->ConnectToServer(ip, port);
	}
	else {
		AddLog("Unknown command \'" + line + "\'");
	}
}

std::vector<std::string> Console::ParseCommand(const std::string& command, const char delim)
{
	auto splitted = std::vector<std::string>();
	auto char_cmd = command.c_str();
	auto spaceIndex = 0;
	for (auto i = 0; i <= command.size(); i++)
	{
		if (command[i] == delim || i == command.size())
		{
			auto element = std::string(&char_cmd[spaceIndex], &char_cmd[i]);
			splitted.push_back(element);
			spaceIndex = i + 1;
		}
	}
	return splitted;
}
