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

#include <chrono>
#include <numeric>
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
	if (m_IntSampleSize < 1)
	{
		m_IntSampleSize = 1;
	}

	if (ImGui::Button("Trash cache with GameObject3D"))
	{
		m_bIntTrashFinished = false;
		std::vector<GameObject3D> g(1000000);
		ImGui::Text("Trashing the cache");
		TrashCacheWithData(g, m_IntTrashCacheData, m_IntTrashCacheDataAverage, m_IntSampleSize, "Int");

		for (const auto& num : m_IntTrashCacheDataAverage)
		{
			std::wstringstream ws;
			ws << num;
			ws << "\n";
			OutputDebugStringW(ws.str().c_str());
		}
		OutputDebugStringW(L"\n");
		m_bIntTrashFinished = true;
	}


	if (m_bIntTrashFinished)
	{
		ImGui::PlotConfig conf;
		conf.values.ys = m_IntTrashCacheDataAverage.data();
		conf.values.count = static_cast<int>(m_IntTrashCacheDataAverage.size());
		conf.scale.min = 0;
		conf.scale.max = *std::max_element(m_IntTrashCacheDataAverage.begin(), m_IntTrashCacheDataAverage.end());
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
	ImGui::SetNextWindowPos(ImVec2(500, 50));
	ImGui::Begin("Exercise 2", NULL, ImGuiWindowFlags_None);

	ImGui::InputInt("Sample size GameObject", &m_GameObject3DSampleSize);
	if (m_GameObject3DSampleSize < 1)
	{
		m_GameObject3DSampleSize = 1;
	}

	if (ImGui::Button("Trash cache with GameObject3D"))
	{
		m_bGameObjectTrashFinished = false;
		std::vector<GameObject3D> g(1000000);
		ImGui::Text("Trashing the cache");
		
		TrashCacheWithData(g, m_GameObject3DTrashCacheData, m_GameObject3DTrashCacheDataAverage, m_GameObject3DSampleSize, "GameObject");


		for (const auto& num : m_GameObject3DTrashCacheDataAverage)
		{
			std::wstringstream ws;
			ws << num;
			ws << "\n";
			OutputDebugStringW(ws.str().c_str());
		}
		OutputDebugStringW(L"\n");
		m_bGameObjectTrashFinished = true;

	}

	if (m_bGameObjectTrashFinished)
	{
		ImGui::PlotConfig conf;
		conf.values.ys = m_GameObject3DTrashCacheDataAverage.data();
		conf.values.count = static_cast<int>(m_GameObject3DTrashCacheDataAverage.size());
		conf.scale.min = *std::min_element(m_GameObject3DTrashCacheDataAverage.begin(), m_GameObject3DTrashCacheDataAverage.end());
		conf.scale.max = *std::max_element(m_GameObject3DTrashCacheDataAverage.begin(), m_GameObject3DTrashCacheDataAverage.end());
		conf.tooltip.show = true;
		conf.tooltip.format = "x=%.2f, y=%.2f";
		conf.grid_x.show = true;
		conf.grid_y.show = true;
		conf.frame_size = ImVec2(200, 100);
		conf.line_thickness = 2.f;
		conf.values.color = ImColor(0, 255, 0);

		ImGui::Plot("Gameobject3DTrashCache Data", conf);
	}


	ImGui::InputInt("Sample size GameObject3DAlt", &m_GameObject3DAltSampleSize);
	if (m_GameObject3DAltSampleSize < 1)
	{
		m_GameObject3DAltSampleSize = 1;
	}


	if (ImGui::Button("Trash cache with GameObject3DAlt"))
	{
		m_bGameObjectAltTrashFinished = false;
		std::vector<GameObject3DAlt> g(1000000);
		ImGui::Text("Trashing the cache");
		TrashCacheWithData(g, m_GameObject3DAltTrashCacheData, m_GameObject3DAltTrashCacheDataAverage, m_GameObject3DAltSampleSize, "GameObjectAlt");

		for (const auto& num : m_GameObject3DAltTrashCacheDataAverage)
		{
			std::wstringstream ws;
			ws << num;
			ws << "\n";
			OutputDebugStringW(ws.str().c_str());
		}
		OutputDebugStringW(L"\n");
		m_bGameObjectAltTrashFinished = true;
	}

	if (m_bGameObjectAltTrashFinished)
	{
		ImGui::PlotConfig conf;
		conf.values.ys = m_GameObject3DAltTrashCacheDataAverage.data();
		conf.values.count = static_cast<int>(m_GameObject3DAltTrashCacheDataAverage.size());
		conf.scale.min = 0;
		conf.scale.max = *std::max_element(m_GameObject3DAltTrashCacheDataAverage.begin(), m_GameObject3DAltTrashCacheDataAverage.end());
		conf.tooltip.show = true;
		conf.tooltip.format = "x=%.2f, y=%.2f";
		conf.grid_x.show = true;
		conf.grid_y.show = true;
		conf.frame_size = ImVec2(200, 100);
		conf.line_thickness = 2.f;
		conf.values.color = ImColor(0, 0, 255);
		ImGui::Plot("Gameobject3DAltTrashCache Data", conf);
	}


	if (m_bGameObjectAltTrashFinished && m_bGameObjectTrashFinished)
	{
		ImGui::Text("Combined data");

		const float* CombinedData[2] = {
			m_GameObject3DAltTrashCacheDataAverage.data(),
			m_GameObject3DTrashCacheDataAverage.data()
		};
		ImU32 colors[2] = { ImColor(0, 0, 255), ImColor(0, 255, 0) };

		ImGui::PlotConfig conf;
		conf.values.count = std::max(static_cast<int>(m_GameObject3DAltTrashCacheDataAverage.size()) ,static_cast<int>(m_GameObject3DTrashCacheDataAverage.size()));
		conf.values.ys_list = CombinedData;
		conf.values.ys_count = 2;
		conf.scale.min = 0;
		conf.scale.max = std::max(
			*std::max_element(m_GameObject3DAltTrashCacheDataAverage.begin(), m_GameObject3DAltTrashCacheDataAverage.end()),
			*std::max_element(m_GameObject3DTrashCacheDataAverage.begin(), m_GameObject3DTrashCacheDataAverage.end())
		);
		conf.tooltip.show = true;
		conf.tooltip.format = "x=%.2f, y=%.2f";
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


template <typename T>
void dae::Minigin::TrashCacheWithData(std::vector<T>& dataset, std::vector<float>& cacheData, std::vector<float>& cacheDataAverage, int sampleSize, const char* datasetName)
{
	cacheData.clear();
	cacheDataAverage.clear();

	ImGui::Text("Trashing the cache with %s", datasetName);

	for (int StepSize{ 1 }; StepSize < 1024; StepSize *= 2)
	{
		for (size_t idx = 0; idx < sampleSize; ++idx)
		{
			const auto start = std::chrono::high_resolution_clock::now();

			for (size_t i = 0; i < dataset.size(); i += StepSize)
			{
				if constexpr (std::is_integral<T>::value)
				{
					dataset[i] *= 2;
				}
				else
				{
					dataset[i].ID *= 2;

				}
			}

			const auto end = std::chrono::high_resolution_clock::now();
			auto total = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
			cacheData.emplace_back(static_cast<float>(total));
		}
		if (sampleSize > 1)
		{
			cacheData.erase(std::max_element(cacheData.begin(), cacheData.end()));
			cacheData.erase(std::min_element(cacheData.begin(), cacheData.end()));
		}



		cacheDataAverage.emplace_back(static_cast<float>(
			std::accumulate(cacheData.begin(), cacheData.end(), 0.f) / cacheData.size()));
	}
}

template void dae::Minigin::TrashCacheWithData<int>(std::vector<int>&, std::vector<float>&, std::vector<float>&, int, const char*);
template void dae::Minigin::TrashCacheWithData<dae::GameObject3D>(std::vector<dae::GameObject3D>&, std::vector<float>&, std::vector<float>&, int, const char*);
template void dae::Minigin::TrashCacheWithData<dae::GameObject3DAlt>(std::vector<dae::GameObject3DAlt>&, std::vector<float>&, std::vector<float>&, int, const char*);