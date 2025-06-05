#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <thread>
#include <iostream>
#include "Minigin.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"

#include "GameObject.h"

#pragma region ImGui
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl2.h"
#include "imgui_plot.h"
#pragma endregion ImGui


SDL_Window* g_window{};
SDL_GLContext g_context;



void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	printf("We compiled against SDL version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version);
	printf("We compiled against SDL_image version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
	printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
		version.major, version.minor, version.patch);


}




dae::Minigin::Minigin(const std::string &dataPath)
{
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	g_window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		1280,
		960,
		SDL_WINDOW_OPENGL
	);

	if (g_window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	g_context = SDL_GL_CreateContext(g_window);
	if (g_context == nullptr)
	{
		throw std::runtime_error(std::string("SDL_GL_CreateContext Error: ") + SDL_GetError());
	}

	int majorVersion, minorVersion;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &majorVersion);
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minorVersion);

	std::cout << "OpenGl Version: " << majorVersion << "." << minorVersion << std::endl;

	Renderer::GetInstance().Init(g_window);

	SDL_RenderSetLogicalSize(Renderer::GetInstance().GetSDLRenderer(), 320, 200);

	ResourceManager::GetInstance().Init(dataPath);
}


dae::Minigin::~Minigin()
{
	ResourceManager::GetInstance().UnloadUnusedResources();
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run(const std::function<void()>& load)
{
	using namespace std::chrono;

	load();

	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& input = InputManager::GetInstance();

	bool doContinue = true;
	auto currentTime = high_resolution_clock::now();
	float lag{};

	constexpr int MS_PER_FRAME{ 1000 / 144 };
	constexpr double FIXED_TIME_STEP{ 1.f / 60.f };

	// imgui 
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(g_window, g_context);
	ImGui_ImplOpenGL2_Init();


	while (doContinue)
	{
		const auto newTime = steady_clock::now();
		const float deltaTime = duration<float>(newTime - currentTime).count();

		currentTime = newTime;
		lag += deltaTime;


		doContinue = input.ProcessInput();
		while (lag >= FIXED_TIME_STEP)
		{
			sceneManager.FixedUpdate(FIXED_TIME_STEP);
			lag -= FIXED_TIME_STEP;
		}
		

		sceneManager.Update(deltaTime);
		sceneManager.RenderUI();
		renderer.Render();

		const auto sleepTime = newTime + milliseconds(MS_PER_FRAME) - high_resolution_clock::now();
		std::this_thread::sleep_for(sleepTime);

	}

	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	
}




