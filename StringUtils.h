#pragma once

class __declspec(dllexport) StringUtils
{
public:

	static std::vector<std::string> ParseCommand(const std::string& s, const std::string& delimiter);

};

