#include "pch.h"
#include "RaycastEngine.h"

using namespace std;


RaycastEngine::RaycastEngine(const int width, const int height, const std::string& title)
{
	m_window = CreateScope<Window>(WindowProps(width, height, title));
	SDL_AddEventWatch(&RaycastEngine::WindowEventListener, this);
	Console::Init(Ref<RaycastEngine>(this));
	m_renderer = CreateScope<Renderer>(width, height);
	m_eventHandler = CreateScope<EventHandler>();
	Input::SetScale(m_renderer->GetScale());
	m_client = CreateScope<UDPClient>();
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
			//engine->m_renderer = CreateScope<Renderer>(*engine->m_window, ev->window.data1, ev->window.data2);
			engine->m_window->Resize(ev->window.data1, ev->window.data2);
			glViewport(0, 0, ev->window.data1, ev->window.data2);
			//engine->m_renderer->Init(ev->window.data1, ev->window.data2);
		}
	}
	return 0;
}

void RaycastEngine::Run()
{
	m_running = true;
	auto lastTime = SDL_GetPerformanceCounter();
	OnStart();
	while (m_running)
	{
		m_eventHandler->Handle();
		auto currentTime = SDL_GetPerformanceCounter();
		auto timeSlice = currentTime - lastTime;
		auto deltaTime = timeSlice / (float)SDL_GetPerformanceFrequency();
		if (m_fps < 1.0f / deltaTime)
			m_fps += 0.1f;
		else m_fps -= 0.1f;
		Update(deltaTime);
		m_eventHandler->PostUpdate();
		Render();
		lastTime = currentTime;
	}
}

void RaycastEngine::ConnectToServer(const std::string& ip, const int port)
{
	if (m_client->IsConnected())
		return;
	Console::AddLog("Trying to connect to " + ip + ":" + to_string(port));
	m_client->Connect(ip, port);
}

void RaycastEngine::DisconnectFromServer()
{
	m_client->Disconnect();
}

void RaycastEngine::Update(const float deltaTime)
{

	OnUpdate(deltaTime);
	
}

void RaycastEngine::Render()
{
	//glViewport(0, 0, m_window->GetWidth(), m_window->GetHeight());
	m_renderer->Clear();

	OnRender();
	m_renderer->Draw();


	DrawUI();



	SDL_GL_SwapWindow(m_window->GetSDLPtr());
}


void RaycastEngine::DrawUI()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_window->GetSDLPtr());
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
		//auto playerPos = "Position: " + to_string(m_camera->GetPosition().x) + ", " + to_string(m_camera->GetPosition().y);
		//auto angle = "Angle: " + to_string(m_camera->GetAngle());
		ImGui::Text(fps.c_str());
		//ImGui::Text(playerPos.c_str());
		//ImGui::Text(angle.c_str());
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
		if(Console::IsOpened())
			Console::Close();
		else 
			Console::Open();
		Input::SetCursorMode(Console::IsOpened() ? CursorMode::Show : CursorMode::Hidden);


	}
	Console::Draw(m_window);
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void RaycastEngine::Shutdown()
{
	m_running = false;
}

