#include "pch.h"
#include "Window.h"

Window::Window(const WindowProps& props) : m_props(props)
{
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	std::string glsl_version = "";
	SDL_GL_SetAttribute(
		SDL_GL_CONTEXT_PROFILE_MASK,
		SDL_GL_CONTEXT_PROFILE_CORE
	);
	glsl_version = "#version 130";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	auto windowFlags = (Uint32)(
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	m_window = SDL_CreateWindow("Ray casting", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, props.Width, props.Height, windowFlags);
	IMGUI_CHECKVERSION();
	m_context = SDL_GL_CreateContext(m_window);
	SDL_GL_MakeCurrent(m_window, m_context);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		throw std::runtime_error("glewInit failed");
	SDL_GL_SetSwapInterval(1);
	SDL_AddEventWatch(Window::Resize, this);
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(m_window, m_context);
	ImGui_ImplOpenGL3_Init(glsl_version.c_str());
	glClearColor(0, 0, 0, 0);



	//float SCALE = 2.0f;
	//ImFontConfig cfg;
	//cfg.SizePixels = 13 * SCALE;
	////ImGui::GetIO().Fonts->AddFontDefault(&cfg)->

	//ImGui::GetIO().DisplaySize = ImVec2(props.Width * 2, props.Height * 2);
	//ImGui::GetIO().DisplayFramebufferScale = ImVec2(2.0f, 2.0f);
}

Window::~Window()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	SDL_GL_DeleteContext(m_context);
	SDL_DestroyWindow(m_window);
}

SDL_Window* Window::GetSDLPtr()
{
	return m_window;
}

int SDLCALL Window::Resize(void* data, SDL_Event* ev)
{
	if(ev->window.event == SDL_WINDOWEVENT_RESIZED)
	if (auto window = static_cast<Window*>(data))
	{
		window->m_props.Width = ev->window.data1;
		window->m_props.Height = ev->window.data2;
		//std::cout << window->m_props.Width << ", " << window->m_props.Height << std::endl;
		return 0;
	}
	return -1;
}

int Window::GetWidth() const
{
    return m_props.Width;
}

int Window::GetHeight() const
{
    return m_props.Height;
}
