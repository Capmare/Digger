#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <thread>
#include <chrono>
#include <iostream>
#include "Minigin.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"

#include "GameObject.h"

#include <Time.h>
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

	ResourceManager::GetInstance().Init(dataPath);
}

void dae::Minigin::ImGuiInterface()
{


	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_Once);
	ImGui::Begin("Exercise 1", NULL, ImGuiWindowFlags_None);

	ImGui::InputInt("Sample size Int", &m_IntSampleSize);

	if (ImGui::Button("Trash cache with Int"))
	{
		m_bIntTrashFinished = false;
		std::vector<int> g(m_IntSampleSize);
		ImGui::Text("Trashing the cache");
		m_IntTrashCacheData.clear();
		for (int StepSize{1}; StepSize < 1024; StepSize *= 2)
		{
			const auto start = std::chrono::high_resolution_clock::now();

			for (int i = 0; i < g.size(); i += StepSize)
			{
				g[i] *= 2;
			}
			auto end = std::chrono::high_resolution_clock::now();
			auto total = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() /10.f;
			m_IntTrashCacheData.emplace_back(static_cast<float>(total));
		}
		m_bIntTrashFinished = true;

		for (const auto& num : m_IntTrashCacheData)
		{
			std::wstringstream ws;
			ws << num;
			ws << "\n";
			OutputDebugStringW(ws.str().c_str());
		}
		OutputDebugStringW(L"\n");

	}

	if (m_bIntTrashFinished)
	{
		ImGui::PlotConfig conf;
		conf.values.ys = m_IntTrashCacheData.data();
		conf.values.count = static_cast<int>(m_IntTrashCacheData.size());
		conf.scale.min = 0;
		conf.scale.max = *std::max_element(m_IntTrashCacheData.begin(), m_IntTrashCacheData.end());
		conf.tooltip.show = true;
		conf.tooltip.format = "x=%1.f, y=%1.f";
		conf.grid_x.show = true;
		conf.grid_y.show = true;
		conf.frame_size = ImVec2(200, 100);
		conf.line_thickness = 2.f;
		conf.values.color = ImColor(255, 0, 0);

		ImGui::Plot("IntTrashCache Data", conf);
	}

	ImGui::End();


	// window 2


	ImGui::SetNextWindowSize(ImVec2(600, 500), ImGuiCond_Once);
	ImGui::Begin("Exercise 2", NULL, ImGuiWindowFlags_None);

	ImGui::InputInt("Sample size GameObject", &m_GameObject3DSampleSize);

	if (ImGui::Button("Trash cache with GameObject3D"))
	{
		m_bIntTrashFinished = false;
		std::vector<GameObject3D> g(m_GameObject3DSampleSize);
		m_GameObject3DTrashCacheData.clear();
		for (int StepSize{ 1 }; StepSize < 1024; StepSize *= 2)
		{
			const auto start = std::chrono::high_resolution_clock::now();

			for (int i = 0; i < g.size(); i += StepSize)
			{
				g[i].ID *= 2;
			}
			auto end = std::chrono::high_resolution_clock::now();
			auto total = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 10.f;
			m_GameObject3DTrashCacheData.emplace_back(static_cast<float>(total));
		}
		m_bGameObjectTrashFinished = true;

		for (const auto& num : m_GameObject3DTrashCacheData)
		{
			std::wstringstream ws;
			ws << num; 
			ws << "\n";
			OutputDebugStringW(ws.str().c_str());
		}
		OutputDebugStringW(L"\n");

	}

	if (m_bGameObjectTrashFinished)
	{
		ImGui::PlotConfig conf;
		conf.values.ys = m_GameObject3DTrashCacheData.data();
		conf.values.count = static_cast<int>(m_GameObject3DTrashCacheData.size());
		conf.scale.min = 0;
		conf.scale.max = *std::max_element(m_GameObject3DTrashCacheData.begin(), m_GameObject3DTrashCacheData.end());
		conf.tooltip.show = true;
		conf.tooltip.format = "x=%1.f, y=%1.f";
		conf.grid_x.show = true;
		conf.grid_y.show = true;
		conf.frame_size = ImVec2(200, 100);
		conf.line_thickness = 2.f;
		conf.values.color = ImColor(0, 255, 0);

		ImGui::Plot("Gameobject3DTrashCache Data", conf);
	}

	ImGui::InputInt("Sample size GameObject3DAlt", &m_GameObject3DAltSampleSize);

	if (ImGui::Button("Trash cache with GameObject3DAlt"))
	{

		m_bIntTrashFinished = false;
		std::vector<GameObject3DAlt> g(m_GameObject3DAltSampleSize);
		m_GameObject3DAltTrashCacheData.clear();
		for (int StepSize{ 1 }; StepSize < 1024; StepSize *= 2)
		{
			const auto start = std::chrono::high_resolution_clock::now();

			for (int i = 0; i < g.size(); i += StepSize)
			{
				g[i].ID *= 2;
			}
			auto end = std::chrono::high_resolution_clock::now();
			auto total = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 10.f;
			m_GameObject3DAltTrashCacheData.emplace_back(static_cast<float>(total));
		}
		m_bGameObjectAltTrashFinished = true;

		for (const auto& num : m_GameObject3DAltTrashCacheData)
		{
			std::wstringstream ws;
			ws << num;
			ws << "\n";
			OutputDebugStringW(ws.str().c_str());
		}
		OutputDebugStringW(L"\n");
	}

	if (m_bGameObjectAltTrashFinished)
	{
		ImGui::PlotConfig conf;
		conf.values.ys = m_GameObject3DAltTrashCacheData.data();
		conf.values.count = static_cast<int>(m_GameObject3DAltTrashCacheData.size());
		conf.scale.min = 0;
		conf.scale.max = *std::max_element(m_GameObject3DAltTrashCacheData.begin(), m_GameObject3DAltTrashCacheData.end());;
		conf.tooltip.show = true;
		conf.tooltip.format = "x=%1.f, y=%1.f";
		conf.grid_x.show = true;
		conf.grid_y.show = true;
		conf.frame_size = ImVec2(200, 100);
		conf.line_thickness = 2.f;
		conf.values.color = ImColor(0, 0, 255);
		ImGui::Plot("Gameobject3DAltTrashCache Data", conf);
	}

	if (m_bGameObjectAltTrashFinished && m_bGameObjectTrashFinished)
	{
		ImGui::Text("Combined graph");

		const float* ys_list[2];
		ys_list[0] = m_GameObject3DAltTrashCacheData.data();
		ys_list[1] = m_GameObject3DTrashCacheData.data();
		ImU32 colors[2] = { ImColor(0, 0, 255), ImColor(0, 255, 0) };

		ImGui::PlotConfig conf;
		conf.values.ys_list = ys_list;
		conf.values.ys_count = 2;
		conf.scale.min = 0;
		conf.scale.max = 10000;
		conf.tooltip.show = true;
		conf.tooltip.format = "x=%1.f, y=%1.f";
		conf.grid_x.show = true;
		conf.grid_y.show = true;
		conf.frame_size = ImVec2(200, 100);
		conf.line_thickness = 2.f;
		conf.values.colors = colors;
		ImGui::Plot("Combined Data", conf);
	}


	ImGui::End();
	ImGui::Render();
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

		if (AssigmentOne)
		{
			ImGuiInterface();

		}

		sceneManager.Update(deltaTime);
		renderer.Render();



		const auto sleepTime = newTime + milliseconds(MS_PER_FRAME) - high_resolution_clock::now();
		std::this_thread::sleep_for(sleepTime);


	}

	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	
}
