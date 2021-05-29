#pragma once
#include <map>
#include "Base.h"
#include "Texture.h"

class __declspec(dllexport) ResourceManager
{
public:
	static void LoadEverything();
	static Ref<Texture> GetTexture(const std::string& name);

private:
	static std::map<std::string, Ref<Texture>> m_textures;
};

