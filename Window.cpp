#include "pch.h"
#include "Window.h"

Window::Window(const WindowProps& props) : m_props(props), m_window(nullptr, SDL_DestroyWindow)
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
	m_window = std::unique_ptr<SDL_Window, std::function<void(SDL_Window*)>>
	(
		SDL_CreateWindow("Ray casting", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, props.Width, props.Height, windowFlags), 
		SDL_DestroyWindow
	);
	IMGUI_CHECKVERSION();
	m_context = SDL_GL_CreateContext(m_window.get());
	SDL_GL_MakeCurrent(m_window.get(), m_context);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		throw std::runtime_error("glewInit failed");
	SDL_GL_SetSwapInterval(1);
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(m_window.get(), m_context);
	ImGui_ImplOpenGL3_Init(glsl_version.c_str());
	glClearColor(0, 0, 0, 0);
}

Window::~Window()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	SDL_GL_DeleteContext(m_context);
}

SDL_Window* Window::GetSDLPtr()
{
	return m_window.get();
}

void Window::Resize(const int width, const int height)
{
	m_props.Width = width;
	m_props.Height = height;
}

int Window::GetWidth() const
{
    return m_props.Width;
}

int Window::GetHeight() const
{
    return m_props.Height;
}
