#include "pch.h"
#include "ResourceManager.h"

std::map<std::string, Ref<Texture>> ResourceManager::m_textures;

void ResourceManager::LoadEverything()
{
	m_textures["ss"] = CreateRef<Texture>("ss", "textures/ss.bmp", 8, 7);
	m_textures["officer"] = CreateRef<Texture>("officer", "textures/officer.bmp", 8, 7);
	m_textures["atlas"] = CreateRef<Texture>("atlas", "textures/atlas.png", 5, 10);
	m_textures["walls"] = CreateRef<Texture>("walls", "textures/walls.png", 6, 19);
	m_textures["trex"] = CreateRef<Texture>("trex", "textures/trex.bmp", 8, 7);
	m_textures["fontMini"] = CreateRef<Texture>("fontMini", "fonts/mini.png", 8, 7);
	m_textures["font"] = CreateRef<Texture>("font", "fonts/font.png", 8, 7);
}

Ref<Texture> ResourceManager::GetTexture(const std::string& name)
{
	if (m_textures.find(name) == m_textures.end())
		return nullptr;
	return m_textures[name];
}
