#include "pch.h"
#include "RaycastEngine.h"

using namespace std;


RaycastEngine::RaycastEngine(const int width, const int height) :
	m_window(WindowProps(width, height)),
	m_renderer(Renderer(m_window, width, height))
{
	ResourceManager::LoadEverything();
	m_console = CreateRef<Console>(Ref<RaycastEngine>(this));
	Input::SetScale(m_renderer.GetScale());
	
	InitGameWorld();
	InitNetworking();
	//InitUI();
	/*Map::SaveMap("maps/Test.xml", m_map);*/
	auto mp1 = Map::LoadMap("maps/out.xml");
	SDL_AddEventWatch(&RaycastEngine::WindowEventListener, this);
}

int SDLCALL RaycastEngine::WindowEventListener(void* data, SDL_Event* ev)
{
	auto engine = (RaycastEngine*)data;
	if (ev->type == SDL_QUIT)
	{
		engine->Shutdown();
	}
	else if (ev->type == SDL_WINDOWEVENT)
	{
		if (ev->window.event == SDL_WINDOWEVENT_RESIZED)
		{
			engine->m_window.Resize(ev->window.data1, ev->window.data2);
			//engine->m_renderer.Init(ev->window.data1, ev->window.data2);
			//engine->m_console->AddLog(to_string(engine->m_window.GetWidth()) + ", " + to_string(engine->m_window.GetHeight()));
		}
	}
	return 0;
}

void RaycastEngine::Run()
{
	m_running = true;
	auto lastTime = SDL_GetPerformanceCounter();
	while (m_running)
	{
		m_eventHandler.Handle();
		auto currentTime = SDL_GetPerformanceCounter();
		auto timeSlice = currentTime - lastTime;
		auto deltaTime = timeSlice / (float)SDL_GetPerformanceFrequency();
		if (m_fps < 1.0f / deltaTime)
			m_fps += 0.1f;
		else m_fps -= 0.1f;
		Update(deltaTime);
		m_eventHandler.PostUpdate();
		Render();
		lastTime = currentTime;
	}
}

void RaycastEngine::ConnectToServer(const std::string& ip, const int port)
{
	if (m_client->IsConnected())
		return;
	m_console->AddLog("Trying to connect to " + ip + ":" + to_string(port));
	m_client->Connect(ip, port);
}

void RaycastEngine::DisconnectFromServer()
{
	m_client->Disconnect();
}

void RaycastEngine::InitGameWorld()
{
	m_map = Map::LoadMap("maps/Test.xml");
	auto plane = vec2();
	plane.y = (m_renderer.GetWidth() / 304.0f) * 0.99f;
	plane.y /= m_renderer.GetHeight() / 152.0f;
	m_player = Player(m_map->GetPlayerSpawnPoint(), 0.0f, plane);
	m_raycaster = Raycaster();
	m_wallTexture = ResourceManager::GetTexture("walls");
}

void RaycastEngine::InitNetworking()
{
	m_client = CreateScope<UDPClient>();
	m_client->SetOnConnectCallback([=](UDPClient* c) {

		m_console->AddLog("Connected successfully with id " + to_string(c->GetID()));
	});
	m_client->SetOnDisconnectCallback([=](UDPClient*) {
		m_console->AddLog("You are disconnected!");
		for (auto player = m_map->GetObjects().begin(); player != m_map->GetObjects().end(); player++)
		{
			if ((*player)->GetType() == ObjectType::NET_PLAYER)
				m_map->GetObjects().erase(player);
		}
		m_players.clear();
	});
	m_client->SetFailedToConnectCallback([=](UDPClient*) {
		m_console->AddLog("Failed To Connect!");
	});
	m_client->SetOtherPlayerConnectCallback([=](UDPClient*, int id) {
		if (m_client->GetID() != id)
		{
			m_console->AddLog("Player " + to_string(id) + " is connected");
			auto spr = Sprite();
			spr.Atlas = ResourceManager::GetTexture("officer");
			m_players[id] = CreateRef<NetPlayer>(spr, m_map->GetPlayerSpawnPoint(), 0.0f);
			m_players.at(id)->SetEnable(true);
			m_map->AddObject(m_players[id]);
		}
	});
	m_client->SetOtherPlayerDisconnectCallback([=](UDPClient*, int id) {
		if (m_client->GetID() != id)
		{
			m_console->AddLog("Player " + to_string(id) + " is disconnected");
			m_players.erase(id);
		}
	});
	m_client->SetOnMessage([=](UDPClient* cl, char* buffer, int len, ServerMessage type) {
		switch (type)
		{
			case ServerMessage::PlayersState:
			{
				auto data = *(PlayerData*)buffer;
				if (data.Id == cl->GetID())
					return;
				if (m_players.find(data.Id) == m_players.end())
				{
					auto spr = Sprite(0, ResourceManager::GetTexture("officer"));
					m_players[data.Id] = CreateRef<NetPlayer>(spr, m_map->GetPlayerSpawnPoint(), 0.0f);
					m_map->AddObject(m_players[data.Id]);
				}
				auto player = m_players[data.Id];
				player->SetPosition(data.Position);
				player->SetVelocity(data.Velocity);
				player->SetAngle(data.Angle);
				player->SetState(data.State);
				player->SetEnable(true);
				
			} break;
			case ServerMessage::Door:
			{
				auto doorInfo = *(DoorInfo*)buffer;
				m_console->AddLog(to_string(doorInfo.x) + ", " + to_string(doorInfo.y));
				m_map->OpenDoorAt(doorInfo.x, doorInfo.y);
			} break;
		}
	});

}

void RaycastEngine::InitUI()
{
	auto fpslabelPos = vec2(m_renderer.GetWidth() - 200, 10);
	m_fpsLabel = CreateRef<Label>("fps: ", fpslabelPos, vec2(200, 50), 20, 0, 0xFF00FF00);
	m_fpsLabel->SetBackgroundColor(0x00111111);
	m_fpsLabel->SetTextAlignment(Right);
	playerPosText = CreateRef<Label>("position: ", vec2(10, 10), vec2(200, 50), 20, 0, 0xFF00FF00);
	playerPosText->SetBackgroundColor(0);
	playerPosText->SetTextAlignment(Left);
	auto btn = CreateRef<Button>("Exit", vec2(10, 10), vec2(200, 50),
		[=](Button* btn) {
			Shutdown();
		}
	);
	btn->SetFontSize(30);
	btn->SetBackgroundColor(0xFF551155);
	panel = CreateRef<Panel>(vec2(), vec2(), 0xAA111111);
	panel->SetSize(vec2(220, 180));
	panel->SetLocalPosition(vec2(m_renderer.GetWidth() / 2 - panel->GetSize().x / 2, m_renderer.GetHeight() / 2 - panel->GetSize().y / 2));
	panel->AddChild(btn);

	auto textBox = CreateRef<TextBox>(vec2(10, 60), vec2(200, 50));
	textBox->SetBackgroundColor(0xAA111111);
	textBox->SetFontSize(25);

	auto textBox1 = CreateRef<TextBox>(vec2(10, 120), vec2(200, 50));
	textBox1->SetBackgroundColor(0xAA111111);
	textBox1->SetFontSize(25);

	panel->AddChild(textBox);
	panel->AddChild(textBox1);
	panel->SetEnable(false);

	auto connectPanel = CreateRef<Panel>(vec2(1, 1), vec2(130, 150), 0xAA0000AA);
	connectPanel->SetSize(vec2(330, 260));
	connectPanel->SetLocalPosition(vec2(m_renderer.GetWidth(), m_renderer.GetHeight()) * 0.5f - connectPanel->GetSize() * 0.5f);
	connectPanel->SetBackgroundColor(0xAA111111);



	auto titleLabel = CreateRef<Label>("Connect to server", vec2(0, 0), vec2(330, 50), 20, 0, 0xFFFFFFFF);
	titleLabel->SetTextAlignment(Middle);

	auto ipLabel = CreateRef<Label>("IP: ", vec2(10, 60), vec2(50, 50));
	ipLabel->SetFontSize(20);
	auto textIp = CreateRef<TextBox>(vec2(70, 60), vec2(250, 50));
	textIp->SetFontSize(20);

	auto portLabel = CreateRef<Label>("Port: ", vec2(10, 130), vec2(80, 50));
	portLabel->SetFontSize(20);
	auto textPort = CreateRef<TextBox>(vec2(100, 130), vec2(220, 50));
	textPort->SetFontSize(20);

	auto connectBtn = CreateRef<Button>("Connect", vec2(10, 200), vec2(310, 50), [=](Button* btn) {
		auto ip = textIp->GetText();
		auto port = textPort->GetText();
		cout << "Connected to " << ip << ":" << port << endl;
		});
	connectBtn->SetBackgroundColor(0xFFCCCCFF);
	connectBtn->SetFontSize(20);
	connectPanel->AddChild(titleLabel);
	connectPanel->AddChild(connectBtn);
	connectPanel->AddChild(ipLabel);
	connectPanel->AddChild(textIp);
	connectPanel->AddChild(portLabel);
	connectPanel->AddChild(textPort);
	connectPanel->SetEnable(false);

	m_uiManager.AddElementToRoot(connectPanel);
	m_uiManager.AddElementToRoot(m_fpsLabel);
	m_uiManager.AddElementToRoot(playerPosText);
}

void RaycastEngine::Update(const float deltaTime)
{
	static bool cursor = false;
	if (Input::IsKeyDown(SDL_SCANCODE_ESCAPE))
	{
		if (!cursor)
		{
			Input::SetCursorMode(CursorMode::Show);
			panel->SetEnable(true);
		}
		else
		{
			panel->SetEnable(false);
			Input::SetCursorMode(CursorMode::Hidden);
		}
		cursor = !cursor;
	}

	if (!m_console->IsOpened())
	{
		m_player.Update(deltaTime);
		if (Input::IsKeyDown(SDL_SCANCODE_E))
			Use();
		if (Input::IsKeyDown(SDL_SCANCODE_SPACE))
			Attack();
	}

	for (auto obj : m_map->GetObjects())
	{
		if(!obj->IsEnabled())
			continue;
		if (auto enemy = dynamic_cast<Enemy*>(obj.get()))
			enemy->Update(deltaTime, m_player);
	}
	m_map->UpdateDoors(deltaTime);
	DoPhysics();
	UpdateNetwork(deltaTime);

	m_uiManager.Update(deltaTime);
}

void RaycastEngine::UpdateNetwork(const float deltaTime)
{
	m_client->Receive();
	if(m_client->IsConnected())
	{
		auto data = PlayerData();
		data.Angle = m_player.GetAngle();
		data.Position = m_player.GetPosition();
		data.Velocity = m_player.GetVelocity();
		data.State = vec2::get_magnitude(data.Velocity) < 0.001 ? EnemyState::Idle : EnemyState::Walk;
		data.Id = m_client->GetID();
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

void RaycastEngine::Render()
{
	glViewport(0, 0, m_window.GetWidth(), m_window.GetHeight());
	m_renderer.Clear();

	DrawWorld();
	DrawObjects();

	m_renderer.Draw();
	DrawUI();

	SDL_GL_SwapWindow(m_window.GetSDLPtr());
}

void RaycastEngine::DrawWorld()
{
	auto max = 0;
	for (auto strip = 0; strip < m_renderer.GetWidth(); strip++)
	{
		auto ray = m_raycaster.CastRay(strip, m_renderer.GetWidth(), m_player, m_map);
		if (ray.Distance > max)
			max = ray.Distance;
		auto inside = ray.Position == vec2::floor(m_player.GetPosition());
		m_renderer.DrawStrip(ray, m_wallTexture, strip, inside);
	}
	m_renderer.SetMaxDistance(max);
}

void RaycastEngine::DrawObjects()
{
	m_renderer.SortObjects(m_map->GetObjects(), m_player);
	for (auto obj : m_map->GetObjects())
	{
		if (!obj->IsEnabled())
			continue;
		m_renderer.DrawSprite(obj->GetSprite(), obj->GetPosition(), m_player);
	}
}

void RaycastEngine::DrawUI()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_window.GetSDLPtr());
	ImGui::NewFrame();

	const float PAD = 10.0f;
	static int corner = 0;
	static bool open = false;
	ImGuiIO& io = ImGui::GetIO();
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
	if (corner != -1)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 work_pos = viewport->WorkPos; 
		ImVec2 work_size = viewport->WorkSize;
		ImVec2 window_pos, window_pos_pivot;
		window_pos.x = (corner & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
		window_pos.y = (corner & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
		window_pos_pivot.x = (corner & 1) ? 1.0f : 0.0f;
		window_pos_pivot.y = (corner & 2) ? 1.0f : 0.0f;
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		window_flags |= ImGuiWindowFlags_NoMove;
	}
	ImGui::SetNextWindowBgAlpha(0.35f); 
	if (ImGui::Begin("Example: Simple overlay", &open, window_flags))
	{
		auto fps = "FPS: " + to_string(m_fps);
		auto playerPos = "Position: " + to_string(m_player.GetPosition().x) + ", " + to_string(m_player.GetPosition().y);
		auto angle = "Angle: " + to_string(m_player.GetAngle());
		ImGui::Text(fps.c_str());
		ImGui::Text(playerPos.c_str());
		ImGui::Text(angle.c_str());
	}
	ImGui::End();




	static bool menu_opened = false;
	static bool network_open = false;
	static bool client_window = false;

	if (Input::IsKeyDown(SDL_SCANCODE_ESCAPE))
	{
		menu_opened = !menu_opened;
	}

	if (menu_opened)
	{
		ImGui::Begin("Menu");
		ImGui::SetWindowSize(ImVec2(250, 350));
		//ImGui::SetWindowPos(ImVec2(m_window.GetWidth() / 2 - 250, m_window.GetHeight() / 2 - 350));
		if (ImGui::Button("Connect", ImVec2(ImGui::GetWindowWidth(), 25)))
		{
			network_open = true;
			client_window = true;
		}
		if (ImGui::Button("Start server", ImVec2(ImGui::GetWindowWidth(), 25)))
		{

			network_open = true;
			client_window = false;
		}

		if (ImGui::Button("Exit", ImVec2(ImGui::GetWindowWidth(), 25)))
		{
			Shutdown();
		}
		ImGui::End();
	}




	if (network_open && menu_opened)
	{


		ImGui::Begin("Networking", &network_open, ImGuiWindowFlags_NoCollapse);
		if (client_window)
		{
			ImGui::SetNextWindowSize(ImVec2(250, 250));
			static char ip[256] = { 0 };

			ImGui::InputText("Ip", ip, 256);
			ImGui::SameLine();
			static auto port = 0;

			ImGui::InputInt("Port", &port);
			if (ImGui::Button("Connect"))
			{
				ConnectToServer(ip, port);
				std::cout << ip << ", " << port << std::endl;
			}
		}
		else {
			if (ImGui::Button("Start server"))
			{

			}
		}
		ImGui::End();
	}

	if (Input::IsKeyDown(SDL_SCANCODE_GRAVE))
	{
		if(m_console->IsOpened())
			m_console->Close();
		else 
			m_console->Open();
		Input::SetCursorMode(m_console->IsOpened() ? CursorMode::Show : CursorMode::Hidden);


	}
	m_console->Draw(m_window);
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void RaycastEngine::DoPhysics()
{
	for (auto i = -1; i <= 1; i++)
	{
		for (auto j = -1; j <= 1; j++)
		{
			if (i == 0 && j == 0)
				continue;
			auto x = (int)(m_player.GetPosition().x + i);
			auto y = (int)(m_player.GetPosition().y + j);
			if (m_map->GetIndexAt(x, y) == 0)
				continue;
			if (m_map->GetIndexAt(x, y) == 98)
			{
				if (m_map->IsDoorOpened(x, y))
					continue;
			}
			auto delta = Physics::Intersection(m_player.GetRadius(), m_player.GetPosition(), vec2(x, y), vec2(1, 1));
			m_player.SetPosition(m_player.GetPosition() - delta);
		}
	}
	for (auto obj : m_map->GetObjects())
	{
		if (!obj->IsCollidable())
			continue;
		auto delta = Physics::Intersection(m_player.GetRadius(), m_player.GetPosition(), 0.3f, obj->GetPosition());
		m_player.SetPosition(m_player.GetPosition() - delta);
	}
}

void RaycastEngine::Use()
{
	//m_console->AddLog("working");
	auto ray = m_player.GetPosition();
	for (auto step = 0.0f; step < 20; step += 0.5f)
	{
		ray += vec2(
			step * cosf(MathUtils::DegToRad(m_player.GetAngle() + 180)),
			step * sinf(MathUtils::DegToRad(m_player.GetAngle() + 180))
		);
		auto tile = m_map->GetIndexAt(ray.x, ray.y);
		if (tile == -1)
			break;

		if (tile == 98 && step < 1.25f)	
		{
			m_map->OpenDoorAt(ray.x, ray.y);
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

void RaycastEngine::Attack()
{
	auto ray = m_player.GetPosition();
	auto stop = false;
	for (auto step = 0.0f; step < 40; step += 0.25f)
	{
		ray += vec2(
			step * cosf(MathUtils::DegToRad(m_player.GetAngle() + 180)),
			step * sinf(MathUtils::DegToRad(m_player.GetAngle() + 180))
		);
		auto tile = m_map->GetIndexAt(ray.x, ray.y);
		if (tile == -1)
			break;
		for (auto& obj : m_map->GetObjects())
		{
			if (obj->IsCollidable())
			{
				auto distance = vec2::distance(obj->GetPosition(), ray);
				//cout << distance << endl;
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

void RaycastEngine::Shutdown()
{
	m_running = false;
}

void RaycastEngine::LoadLevel(Map* map)
{
	m_map = map;
	m_player.SetPosition(map->GetPlayerSpawnPoint());
}
