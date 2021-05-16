#include "pch.h"
#include "RaycastEngine.h"

using namespace std;


RaycastEngine::RaycastEngine(const int width, const int height) :
	m_window(WindowProps(width, height)),
	m_renderer(Renderer(m_window, width * 0.7, height * 0.7))
{
	m_console = CreateRef<Console>(Ref<RaycastEngine>(this));
	Input::SetScale(m_renderer.GetScale());
	InitGameWorld();
	InitNetworking();
	SDL_AddEventWatch(Quit, this);
}

int SDLCALL RaycastEngine::Quit(void* data, SDL_Event* ev)
{
	if (ev->type == SDL_QUIT)
	{
		auto engine = (RaycastEngine*)data;
		engine->Shutdown();
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
	m_console->AddLog("Trying to connect to " + ip + ":" + to_string(port));
	m_client->Connect(ip, port);
}

void RaycastEngine::InitGameWorld()
{
	m_map = Map(vec2(5, 5));
	auto plane = vec2();
	plane.y = (m_renderer.GetWidth() / 304.0f) * 0.99f;
	plane.y /= m_renderer.GetHeight() / 152.0f;
	m_player = Player(m_map.GetPlayerSpawnPoint(), 0.0f, plane);
	m_raycaster = Raycaster();
	m_wallTexture = Texture("textures/walls.png", 6, 19);

	auto atlas = CreateRef<Texture>("textures/atlas.png", 5, 10);
	auto ss_sprite = Sprite();
	ss_sprite.Atlas = CreateRef<Texture>("textures/ss.bmp", 8, 7);

	auto trex_sprite = Sprite();
	trex_sprite.Atlas = CreateRef<Texture>("textures/trex.bmp", 8, 7);

	m_secondPlayer = CreateRef<NetPlayer>(ss_sprite, vec2(3, 3), 0);
	m_map.AddObject(m_secondPlayer);
	auto sprite = Sprite();
	sprite.Id = 11;
	sprite.Atlas = atlas;

	auto lampSprite = Sprite();
	lampSprite.Id = 17;
	lampSprite.Atlas = atlas;
	m_map.AddObject(CreateRef<Object>(sprite, vec2(1.5, 1.5), true));
	m_map.AddObject(CreateRef<Object>(sprite, vec2(3.5, 3.5), true));
	m_map.AddObject(CreateRef<Enemy>(ss_sprite, vec2(4.5, 4.5), 0));
	m_map.AddObject(CreateRef<Enemy>(trex_sprite, vec2(4.5, 5.5), 0));
	m_map.AddObject(CreateRef<Object>(lampSprite, vec2(5.5, 5.5), false));
	m_map.AddObject(CreateRef<Object>(sprite, vec2(6.5, 6.5), true));
	m_map.AddObject(CreateRef<Enemy>(ss_sprite, vec2(13.5, 6.5), 90));
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
	//m_uiElements.push_back(panel);
	//m_uiElements.push_back(m_fpsLabel);
	//m_uiElements.push_back(playerPosText);
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

void RaycastEngine::InitNetworking()
{
	m_client = CreateScope<UDPClient>();
	m_client->SetOnConnectCallback([=](UDPClient*) {
		m_console->AddLog("Connected successfully!");
		});
	m_client->SetOnDisconnectCallback([=](UDPClient*) {
		m_console->AddLog("You are disconnected!");
		});
	m_client->SetFailedToConnectCallback([=](UDPClient*) {
		m_console->AddLog("Failed To Connect!");
	});
}

void RaycastEngine::Update(const float deltaTime)
{

	if (m_client->IsConnected())
	{
		static float timer = 0;
		timer += deltaTime * 100;
		
		//int id = ((ClientMessage*)&msg)->Id;
		//m_console->AddLog(to_string(id));
		if (timer > 2)
		{
			auto player = PlayerInfo();
			player.Angle = m_player.GetAngle();
			player.Position = m_player.GetPosition();
			auto msg = m_client->BuildMessage((char*)&player);
			m_client->Send((char*)&msg, sizeof(ClientMessage));
			timer = 0;
		}
		while (auto msg = m_client->PollMessage())
		{
			auto player = *(PlayerInfo*)&(msg->Message);
			//std::cout << clientMessage->Id << std::endl;
			if (msg->Id != m_client->GetID())
			{
				
				m_secondPlayer->SetPosition(player.Position);
				m_secondPlayer->SetAngle(player.Angle);
				
				m_console->AddLog(to_string(msg->Id) + ": " + to_string(player.Position.x) + ", " + to_string(player.Position.y));
			}
		}
	}
	if (m_console->IsOpened())
		return;
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
	if (!cursor)
		m_player.Update(deltaTime);


	//static float mouseY = 0.0f;
	//mouseY -= Input::GetMouseAxis().y * deltaTime * 500;
	/*static float headMove = 0.0f;
	headMove += deltaTime * 10;
	m_renderer.SetVerticalOffset(64 + cosf(headMove) *400 * vec2::get_magnitude(m_player.GetVelocity()));*/

	for (auto obj : m_map.GetObjects())
	{
		if (auto enemy = dynamic_cast<Enemy*>(obj.get()))
			enemy->Update(deltaTime, m_player);
	}
	if (Input::IsKeyDown(SDL_SCANCODE_E))
		Use();
	if (Input::IsKeyDown(SDL_SCANCODE_SPACE))
		Attack();
	m_map.UpdateDoors(deltaTime);
	DoPhysics();


	m_uiManager.Update(deltaTime);
}

void RaycastEngine::Render()
{
	glViewport(0, 0, m_window.GetWidth(), m_window.GetHeight());
	m_renderer.Clear();

	DrawWorld();
	DrawObjects();

	//ImGui::NewFrame();
	//m_renderer.Draw(m_window.GetWidth(), m_window.GetHeight());
	//ImGui::ShowDemoWindow();

	//ImGui::Begin("Image");
	//ImGui::Image(texture, ImVec2(100, 100));
	//ImGui::End();
	//ImGui::Render();

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
	m_renderer.SortObjects(m_map.GetObjects(), m_player);
	for (auto obj : m_map.GetObjects())
	{
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
			if (m_map.GetIndexAt(x, y) == 0)
				continue;
			if (m_map.GetIndexAt(x, y) == 98)
			{
				if (m_map.IsDoorOpened(x, y))
					continue;
			}
			auto delta = Physics::Intersection(m_player.GetRadius(), m_player.GetPosition(), vec2(x, y), vec2(1, 1));
			m_player.SetPosition(m_player.GetPosition() - delta);
		}
	}
	for (auto obj : m_map.GetObjects())
	{
		if (!obj->IsCollidable())
			continue;
		auto delta = Physics::Intersection(m_player.GetRadius(), m_player.GetPosition(), 0.3f, obj->GetPosition());
		m_player.SetPosition(m_player.GetPosition() - delta);
	}
}

void RaycastEngine::Use()
{
	auto ray = m_player.GetPosition();
	for (auto step = 0.0f; step < 20; step += 0.5f)
	{
		ray += vec2(
			step * cosf(MathUtils::DegToRad(m_player.GetAngle())),
			step * sinf(MathUtils::DegToRad(m_player.GetAngle()))
		);
		auto tile = m_map.GetIndexAt(ray.x, ray.y);
		if (tile == -1)
			break;

		if (tile == 98 && step < 1.25f)
			m_map.OpenDoorAt(ray.x, ray.y);

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
			step * cosf(MathUtils::DegToRad(m_player.GetAngle())),
			step * sinf(MathUtils::DegToRad(m_player.GetAngle()))
		);
		auto tile = m_map.GetIndexAt(ray.x, ray.y);
		if (tile == -1)
			break;
		for (auto& obj : m_map.GetObjects())
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
