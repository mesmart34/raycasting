#pragma once

#include "imgui.h"
#include <string>
#include <vector>
#include <sstream>
#include "Input.h"
#include "Window.h"
#include "Base.h"
#include "RaycastEngine.h"

class RaycastEngine;

class Console
{
public:
	Console(const Ref<RaycastEngine>& engine);

	void Open();
	void Close();
	void AddLog(const std::string& data);

	void Draw(const Window& window);

	bool IsOpened() const;

private:
	void ProcessCommand(const std::string& command);
	std::vector<std::string> ParseCommand(const std::string& command, const char delim);

private:
	Ref<RaycastEngine> m_engine;
	std::stringstream m_stream;
	std::vector<std::string> m_consoleHistory;
	bool m_opened;
	
};

