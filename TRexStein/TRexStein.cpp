#include "TRexStein.h"

using namespace std;

TrexStein::TrexStein(const int width, const int height, const std::string& title)
	: RaycastEngine(width, height, title)
{
	ResourceManager::LoadEverything();
	m_uiManager = CreateScope<UIManager>();
	m_raycaster = CreateScope<Raycaster>();
}


void TrexStein::AddNetworkPlayer(NetPlayer* object)
{
	auto found = false;
	if (object->GetClientID() == m_client->GetID())
		return;
	for (auto& obj : m_map->GetObjects())
	{
		if (obj->GetID() == object->GetID())
		{
			obj->SetPosition(object->GetPosition());
			obj->SetVelocity(object->GetVelocity());
			obj->SetAngle(object->GetAngle());
			//Console::AddLog(to_string(obj->GetAngle()));
			obj->SetEnable(object->IsEnabled());
			obj->SetColliable(object->IsCollidable());
			found = true;
			break;
		}
	}
	if (!found)
	{
		auto player = CreateRef<NetPlayer>(NetPlayer(*object));
		m_map->GetObjects().push_back(player);
		player->SetID(object->GetID());
	}
}

void TrexStein::AddObject(Object* object)
{
	auto found = false;
	for (auto obj : m_map->GetObjects())
	{
		if (obj->GetID() == object->GetID())
		{
			obj->SetPosition(object->GetPosition());
			obj->SetVelocity(object->GetVelocity());
			obj->SetAngle(object->GetAngle());
			obj->SetEnable(object->IsEnabled());
			obj->SetColliable(object->IsCollidable());
			found = true;
			break;
		}
	}
	if (!found)
	{
		m_map->GetObjects().push_back(CreateRef<Object>(Object(*object)));
	}
}

void TrexStein::AddEnemy(Enemy* object)
{
	auto found = false;
	for (auto obj : m_map->GetObjects())
	{
		if (obj->GetID() == object->GetID())
		{
			obj->SetPosition(object->GetPosition());
			obj->SetVelocity(object->GetVelocity());
			obj->SetAngle(object->GetAngle());
			obj->SetEnable(object->IsEnabled());
			obj->SetColliable(object->IsCollidable());
			found = true;
			break;
		}
	}
	if (!found)
	{
		m_map->GetObjects().push_back(CreateRef<Enemy>(Enemy(*object)));

	}
}

void TrexStein::LoadLevel(Map* map)
{
	m_map.reset(map);
	m_map->GetObjects().clear();
	m_camera->SetPosition(m_map->GetPlayerSpawnPoint());
}

void TrexStein::SetupClient()
{
	m_client = CreateScope<UDPClient>();
	m_client->SetOnConnectCallback([=](UDPClient* c) {

		Console::AddLog("Connected successfully with id " + to_string(c->GetID()));
		});
	m_client->SetOnDisconnectCallback([=](UDPClient*) {
		Console::AddLog("You are disconnected!");
		m_players.clear();
		});
	m_client->SetFailedToConnectCallback([=](UDPClient*) {
		Console::AddLog("Failed To Connect!");
		});
	m_client->SetOtherPlayerConnectCallback([=](UDPClient*, int id) {
		if (m_client->GetID() != id)
		{
			Console::AddLog("Player " + to_string(id) + " is connected");
		}
		});
	m_client->SetOtherPlayerDisconnectCallback([=](UDPClient*, int id) {
		if (m_client->GetID() != id)
		{
			Console::AddLog("Player " + to_string(id) + " is disconnected");
			//m_players.erase(id);
		}
		});
	m_client->SetOnMessage([=](UDPClient* cl, char* buffer, int len, ServerMessage type) {
		switch (type)
		{
		case ServerMessage::ObjectState:
		{
			auto bytes = 0;
			while (bytes < len)
			{
				auto size = 0;
				auto type = (ObjectType)buffer[bytes];
				bytes += sizeof(char);
				if (type == ObjectType::NET_PLAYER)
				{
					size = sizeof(NetPlayer);
					auto object = new NetPlayer();
					memcpy(&object[0], &buffer[bytes], sizeof(NetPlayer));
					AddNetworkPlayer(object);
				}else if(type == ObjectType::ENEMY)
				{
					size = sizeof(Enemy);
					auto object = new Enemy();
					memcpy(&object[0], &buffer[bytes], sizeof(Enemy));
					AddEnemy(object);
				}
				else
				if (type == ObjectType::STATIC)
				{
					size = sizeof(Object);
					auto object = new Object();
					memcpy(&object[0], &buffer[bytes], sizeof(Object));
					AddObject(object);
				}
				bytes += size;
			}

		} break;
		case ServerMessage::Door:
		{
			auto bytes = 0;
			while (bytes < len)
			{
				auto doorInfo = DoorInfo();
				memcpy(&doorInfo, &buffer[bytes], sizeof(DoorInfo));
				m_map->OpenDoorAt(doorInfo.x, doorInfo.y);
				Console::AddLog(to_string(doorInfo.x) + ", " + to_string(doorInfo.y));
				bytes += sizeof(DoorInfo);
			}
		} break;
		}
		});
}

void TrexStein::UpdateNetwork()
{
	m_client->Receive();
	if (m_client->IsConnected())
	{
		auto data = PlayerData();
		data.Angle = m_camera->GetAngle();
		data.Position = m_camera->GetPosition();
		data.Velocity = m_camera->GetVelocity();
		data.State = vec2::get_magnitude(data.Velocity) < 0.001 ? ObjectState::Idle : ObjectState::Walk;
		data.ClientID = m_client->GetID();
		char buffer[MAX_PACKET_SIZE];
		auto size = 0;
		buffer[0] = (char)ClientMessage::Input;
		size += sizeof(char);
		buffer[size] = m_client->GetID();
		size += sizeof(uint16_t);
		memcpy(&buffer[size], &data, sizeof(PlayerData));
		size += sizeof(PlayerData);
		m_client->Send((char*)&buffer, size);
	}
}

void TrexStein::OnStart()
{
	ResourceManager::LoadEverything();
	m_wallTexture = ResourceManager::GetTexture("walls");
	auto plane = vec2();
	plane.y = (m_renderer->GetWidth() / 304.0f) * 0.99f;
	plane.y /= m_renderer->GetHeight() / 152.0f;
	m_camera = CreateScope<Camera>(vec2(), 0.0f, plane);
	LoadLevel(Utils::LoadMap("maps/Test.xml"));
	m_camera->SetPosition(m_map->GetPlayerSpawnPoint());
	m_client = CreateScope<UDPClient>();
	SetupClient();

	for (const auto p : m_map->GetObjects())
		Console::AddLog(to_string(p->GetID()));
}

void TrexStein::OnUpdate(const float deltaTime)
{
	UpdateNetwork();
	if (!Console::IsOpened())
	{
		ControlCamera(m_camera, deltaTime);
		if (Input::IsKeyDown(SDL_SCANCODE_E))
			Use();
		if (Input::IsKeyDown(SDL_SCANCODE_SPACE))
			Attack();
	}
	m_camera->Physics(deltaTime);

	for (const auto& obj : m_map->GetObjects())
	{
		if (!obj->IsEnabled())
			continue;
		obj->Update(deltaTime);
	}
	m_map->UpdateDoors(deltaTime);
	DoPhysics();
	m_uiManager->Update(deltaTime);
}

void TrexStein::OnRender()
{
	DrawWorld();
	DrawObjects();


}

void TrexStein::OnConsoleCommand(const std::vector<std::string>& command)
{
	if (command[0] == "quit")
	{
		Shutdown();
	}
	else if (command[0] == "clear")
	{
		Console::Clear();
	}
	else if (command[0] == "connect")
	{
		auto clientInfo = StringUtils::ParseCommand(command[1], ":");
		auto ip = clientInfo[0];
		auto port = atoi(clientInfo[1].c_str());
		ConnectToServer(ip, port);
	}
	else if (command[0] == "disconnect")
	{
		DisconnectFromServer();
	}
	else if (command[0] == "load")
	{
		if (command[1] == "level")
		{
			auto level = Utils::LoadMap("maps/" + command[2] + ".xml");

			if (level == nullptr)
				Console::AddLog("Failed");
			else
			{
				LoadLevel(level);
				Console::AddLog("The level " + command[2] + " is successfully loaded!");
			}

		}
	}
	else {
		Console::AddLog("Unknown command \'" + command[0] + "\'");
	}
}

void TrexStein::DrawWorld()
{
	auto max = 0;
	for (auto strip = 0; strip < m_renderer->GetWidth(); strip++)
	{
		auto ray = m_raycaster->CastRay(strip, m_renderer->GetWidth(), m_camera, m_map);
		if (ray.Distance > max)
			max = ray.Distance;
		auto inside = ray.Position == vec2::floor(m_camera->GetPosition());
		m_renderer->DrawStrip(ray, m_wallTexture, strip, inside);
	}
	m_renderer->SetMaxDistance(max);
}

void TrexStein::DrawObjects()
{
	/*auto objectToRender = vector<Ref<Object>>();
	copy(m_map->GetObjects().begin(), m_map->GetObjects().end(), back_inserter(objectToRender));
	for (auto player : m_players)
		objectToRender.push_back(player.second);*/
	m_renderer->SortObjects(m_map->GetObjects(), m_camera->GetPosition());
	for (auto obj : m_map->GetObjects())
	{
		if (!obj->IsEnabled())
			continue;
		/*if (auto d = dynamic_cast<NetPlayer*>(obj.get()))
		{
			Console::AddLog(to_string(d->GetID()));

		}*/

		//Console::AddLog(to_string(obj->GetAngle()));
		obj->CalculateSprite(m_camera->GetPosition());
		//Console::AddLog(to_string(obj->GetSprite().Id));
		m_renderer->DrawSprite(obj->GetSprite(), obj->GetPosition(), m_camera);
	}
}

void TrexStein::DoPhysics()
{
	for (auto i = -1; i <= 1; i++)
	{
		for (auto j = -1; j <= 1; j++)
		{
			if (i == 0 && j == 0)
				continue;
			auto x = (int)(m_camera->GetPosition().x + i);
			auto y = (int)(m_camera->GetPosition().y + j);
			if (m_map->GetIndexAt(x, y) == 0)
				continue;
			if (m_map->GetIndexAt(x, y) == 98)
			{
				if (m_map->IsDoorOpened(x, y))
					continue;
			}
			auto delta = Physics::Intersection(m_camera->GetRadius(), m_camera->GetPosition(), vec2(x, y), vec2(1, 1));
			m_camera->SetPosition(m_camera->GetPosition() - delta);
		}
	}
	for (auto i = 0; i < m_map->GetObjects().size(); i++)
	{
		auto a = m_map->GetObjects()[i];
		if (a->IsCollidable())
		{
			auto delta = Physics::Intersection(m_camera->GetRadius(), m_camera->GetPosition(), 0.3f, a->GetPosition());
			m_camera->SetPosition(m_camera->GetPosition() - delta);
		}
	}
}

void TrexStein::SolveWallCollision(vec2& position, const float radius)
{
	for (auto i = -1; i <= 1; i++)
	{
		for (auto j = -1; j <= 1; j++)
		{
			if (i == 0 && j == 0)
				continue;
			auto x = (int)(position.x + i);
			auto y = (int)(position.y + j);
			if (m_map->GetIndexAt(x, y) == 0)
				continue;
			if (m_map->GetIndexAt(x, y) == 98)
			{
				if (m_map->IsDoorOpened(x, y))
					continue;
			}
			auto delta = Physics::Intersection(radius, position, vec2(x, y), vec2(1, 1));
			position -= delta;
		}
	}
}

void TrexStein::Use()
{
	auto ray = m_camera->GetPosition();
	for (auto step = 0.0f; step < 20; step += 0.5f)
	{
		ray += vec2(
			step * cosf(MathUtils::DegToRad(m_camera->GetAngle() + 180)),
			step * sinf(MathUtils::DegToRad(m_camera->GetAngle() + 180))
		);
		auto tile = m_map->GetIndexAt(ray.x, ray.y);
		if (tile == -1)
			break;

		if (tile == 98 && step < 1.25f)
		{
			//m_map->OpenDoorAt(ray.x, ray.y);
			if (m_client->IsConnected())
			{
				char buffer[MAX_PACKET_SIZE];
				auto size = 0;
				buffer[0] = (char)ClientMessage::Door;
				size += sizeof(char);
				auto x = (int)ray.x;
				auto y = (int)ray.y;
				memcpy(&buffer[size], &x, sizeof(int));
				size += sizeof(int);
				memcpy(&buffer[size], &y, sizeof(int));
				size += sizeof(int);
				m_client->Send(buffer, size);
			}
		}

		if (tile > 0)
			break;

	}
}

void TrexStein::Attack()
{
	auto ray = m_camera->GetPosition();
	auto stop = false;
	for (auto step = 0.0f; step < 40; step += 0.25f)
	{
		ray += vec2(
			step * cosf(MathUtils::DegToRad(m_camera->GetAngle() + 180)),
			step * sinf(MathUtils::DegToRad(m_camera->GetAngle() + 180))
		);
		auto tile = m_map->GetIndexAt(ray.x, ray.y);
		if (tile == -1)
			break;
		for (auto& obj : m_map->GetObjects())
		{
			if (obj->IsCollidable())
			{
				auto distance = vec2::distance(obj->GetPosition(), ray);
				if (vec2::distance(obj->GetPosition(), ray) < 0.3f)
				{
					obj->OnRaycastHit(10);
					stop = true;
					break;
				}
			}
		}

		if (tile > 0 || stop)
			break;

	}
}

void TrexStein::ControlCamera(const Scope<Camera>& camera, const float deltaTime)
{
	static float mouseX = 0;

	mouseX += -Input::GetMouseAxis().x * deltaTime * 30.0f;
	mouseX *= 0.75f;

	camera->Rotate(mouseX);
	auto velocity = m_camera->GetVelocity();
	auto direction = m_camera->GetDirection();
	if (Input::IsKeyPressed(SDL_SCANCODE_W))
		velocity += direction * deltaTime * 0.2;
	else if (Input::IsKeyPressed(SDL_SCANCODE_S))
		velocity += direction * deltaTime * -0.2;
	if (Input::IsKeyPressed(SDL_SCANCODE_A))
		velocity += vec2(-direction.y, direction.x) * deltaTime * 0.2;
	else if (Input::IsKeyPressed(SDL_SCANCODE_D))
		velocity += vec2(direction.y, -direction.x) * deltaTime * 0.2;

	camera->SetVelocity(velocity);
	velocity.x = MathUtils::Clamp(velocity.x, -0.05, 0.05);
	velocity.y = MathUtils::Clamp(velocity.y, -0.05, 0.05);
	velocity *= 0.95;
	if (fabs(velocity.x) < 0.001f)
		velocity.x = 0;
	if (fabs(velocity.y) < 0.001f)
		velocity.y = 0;

}

int main(int argc, char** argv)
{
	auto game = new TrexStein(1360, 768, "TRexStein");
	game->Run();
	return 0;
}