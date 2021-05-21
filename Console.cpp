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
	if (m_consoleHistory.size() > 100)
		m_consoleHistory.erase(m_consoleHistory.begin());
	m_consoleHistory.push_back(data);
	//m_stream << data << (char)0xa;
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

bool Console::IsOpened() const
{
	return m_opened;
}

void Console::ProcessCommand(const std::string& line)
{
	auto command = StringUtils::ParseCommand(line, " ");
	if (command[0] == "quit")
	{
		m_engine->Shutdown();
	}
	else if (command[0] == "clear")
	{
		m_consoleHistory.clear();
	}
	else if (command[0] == "connect")
	{
		auto clientInfo = StringUtils::ParseCommand(command[1], ":");
		auto ip = clientInfo[0];
		auto port = atoi(clientInfo[1].c_str());
		AddLog(line);
		m_engine->ConnectToServer(ip, port);
	}
	else if (command[0] == "disconnect")
	{
		AddLog(line);
		m_engine->DisconnectFromServer();
	}
	else if (command[0] == "load")
	{
		if (command[1] == "level")
		{
			AddLog(line);
			auto level = Map::LoadMap("maps/" + command[2] + ".xml");

			if (level == nullptr)
				AddLog("Failed");
			else
			{
				m_engine->LoadLevel(level);
				AddLog("The level " + command[2] + " is successfully loaded!");
			}

		}
	}
	else {
		AddLog("Unknown command \'" + line + "\'");
	}
}
