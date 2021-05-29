#pragma once
#include "Raycasting.h"
#include "tinyxml2.h"
#include "Enemy.h"
#include <utility>
#include <map>
#include "NetPlayer.h"
#include "Utils.h"

class TrexStein : public RaycastEngine
{
public:
	TrexStein(const int width, const int height, const std::string& title);

protected:
	virtual void OnStart() override;
	virtual void OnUpdate(const float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnConsoleCommand(const std::vector<std::string>& commandTokens) override;


private:
	void AddNetworkPlayer(NetPlayer* net);
	void AddObject(Object* obj);
	void AddEnemy(Enemy* enemy);
	void LoadLevel(Map* map);
	void SetupClient();
	void UpdateNetwork();
	void DrawWorld();
	void DrawObjects();
	void DoPhysics();
	void SolveWallCollision(vec2& position, const float radius);
	void Use();
	void Attack();
	void ControlCamera(const Scope<Camera>& camera, const float deltaTime);

private:
	Scope<Map>						m_map;
	Scope<UIManager>				m_uiManager;
	Scope<Raycaster>				m_raycaster;
	Ref<Texture>					m_wallTexture;
	Scope<Camera>					m_camera;
	std::map<int, Ref<NetPlayer>>	m_players;

};

