#pragma once

#include "imgui.h"
#include <string>
#include <vector>
#include <sstream>
#include "Input.h"
#include "Window.h"
#include "Base.h"
#include "RaycastEngine.h"
#include "StringUtils.h"

class RaycastEngine;

class Console
{
public:
	Console() = delete;
	static void Init(const Ref<RaycastEngine>& engine);

	static void Open();
	static void Close();
	static void AddLog(const std::string& data);

	static void Draw(const Scope<Window>& window);

	static bool IsOpened();

private:
	static void ProcessCommand(const std::string& command);
	

private:
	static Ref<RaycastEngine> m_engine;
	static std::stringstream m_stream;
	static std::vector<std::string> m_consoleHistory;
	static bool m_opened;
	
};

