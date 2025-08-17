#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <Xinput.h>
#include <SDL.h>
#include <fstream>
#include <algorithm>
#include "MenuComponent.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "Scene.h"

void LoadSinglePlayer();
void LoadCoOp();
void LoadVersus();

namespace dae {

	MenuComponent::MenuComponent(GameObject* owner)
		: BaseComponent(owner) {
	}

	void MenuComponent::BuildScoreboard()
	{
		m_TitleX = 8;   m_TitleY = 8;
		m_HeaderY = 20;
		m_NameX = 8;   m_ScoreX = 100;   
		m_BaseY = 32;  m_LineStep = 12;
		m_MaxRows = 12;

		auto& scene = SceneManager::GetInstance().GetLastScene();
		m_ScoreRoot = scene.CreateNewGameObject();

		auto font = ResourceManager::GetInstance().LoadFont("GameFont.ttf", 12);

		m_ScoreRoot->CreateComponent<TextComponent>("Scoreboard", font, SDL_Color{ 255,220,120,255 })
			->SetPosition(static_cast<float>(m_TitleX), static_cast<float>(m_TitleY));

		m_ScoreRoot->CreateComponent<TextComponent>("Name", font, SDL_Color{ 200,200,200,255 })
			->SetPosition(static_cast<float>(m_NameX), static_cast<float>(m_HeaderY));
		m_ScoreRoot->CreateComponent<TextComponent>("Score", font, SDL_Color{ 200,200,200,255 })
			->SetPosition(static_cast<float>(m_ScoreX), static_cast<float>(m_HeaderY));

		auto scores = LoadScores(m_ScoreFile);
		if (scores.empty()) {
			m_ScoreRoot->CreateComponent<TextComponent>("- no scores yet -", font, SDL_Color{ 180,180,180,255 })
				->SetPosition(static_cast<float>(m_NameX), static_cast<float>(m_BaseY));
			return;
		}

		for (int i = 0; i < static_cast<int>(scores.size()) && i < m_MaxRows; ++i) {
			const auto& [name, score] = scores[i];
			const int y = static_cast<int>(m_BaseY + i * m_LineStep);

			const float rankX = static_cast<float>(std::max(0, static_cast<int>(m_NameX - 8)));
			m_ScoreRoot->CreateComponent<TextComponent>(std::to_string(i + 1) + ".", font, SDL_Color{ 150,150,150,255 })
				->SetPosition(rankX, static_cast<float>(y));

			m_ScoreRoot->CreateComponent<TextComponent>(name, font, SDL_Color{ 255,255,255,255 })
				->SetPosition(static_cast<float>(m_NameX), static_cast<float>(y));

			m_ScoreRoot->CreateComponent<TextComponent>(std::to_string(score), font, SDL_Color{ 255,255,255,255 })
				->SetPosition(static_cast<float>(m_ScoreX), static_cast<float>(y));
		}
	}


	void MenuComponent::Update(float)
	{
		if (m_HasSelected)
			return;

		XINPUT_STATE state{};
		DWORD xiResult = XInputGetState(0, &state);

		const Uint8* keys = SDL_GetKeyboardState(nullptr);
		bool keyA = keys[SDL_SCANCODE_A] != 0;
		bool keyX = keys[SDL_SCANCODE_X] != 0;
		bool keyY = keys[SDL_SCANCODE_Y] != 0;

		// On first frame after scene loads, seed previous states and ignore input.
		if (m_firstUpdate) {
			if (xiResult == ERROR_SUCCESS) {
				m_PrevButtons = state.Gamepad.wButtons;
			}
			m_prevKeyA = keyA;
			m_prevKeyX = keyX;
			m_prevKeyY = keyY;
			m_firstUpdate = false;
			return; // consume carry-over input
		}

		//  Controller edge detection 
		if (xiResult == ERROR_SUCCESS) {
			uint16_t pressed = static_cast<uint16_t>(state.Gamepad.wButtons & ~m_PrevButtons);

			if (pressed & XINPUT_GAMEPAD_A) {
				LoadSinglePlayer();
				m_HasSelected = true;
			}
			else if (pressed & XINPUT_GAMEPAD_X) {
				LoadCoOp();
				m_HasSelected = true;
			}
			else if (pressed & XINPUT_GAMEPAD_Y) {
				LoadVersus();
				m_HasSelected = true;
			}

			m_PrevButtons = state.Gamepad.wButtons;
		}

		//  Keyboard edge detection 
		bool aPressed = (keyA && !m_prevKeyA);
		bool xPressed = (keyX && !m_prevKeyX);
		bool yPressed = (keyY && !m_prevKeyY);

		if (!m_HasSelected) {
			if (aPressed) {
				LoadSinglePlayer();
				m_HasSelected = true;
			}
			else if (xPressed) {
				LoadCoOp();
				m_HasSelected = true;
			}
			else if (yPressed) {
				LoadVersus();
				m_HasSelected = true;
			}
		}

		m_prevKeyA = keyA;
		m_prevKeyX = keyX;
		m_prevKeyY = keyY;
	}

	void MenuComponent::Trim(std::string& s)
	{
		auto notSpace = [](unsigned char c) { return !std::isspace(c); };
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), notSpace));
		s.erase(std::find_if(s.rbegin(), s.rend(), notSpace).base(), s.end());
	}

	bool MenuComponent::ParseLine(const std::string& line, std::string& nameOut, int& scoreOut)
	{
		auto pos = line.find(',');
		if (pos == std::string::npos) return false;
		std::string name = line.substr(0, pos);
		std::string scoreStr = line.substr(pos + 1);
		Trim(name);
		Trim(scoreStr);
		if (name.empty() || scoreStr.empty()) return false;
		try { scoreOut = std::stoi(scoreStr); }
		catch (...) { return false; }
		nameOut = std::move(name);
		return true;
	}


	std::vector<std::pair<std::string, int>> MenuComponent::LoadScores(const std::string& path)
	{
		auto log = [](const std::string& s) {
			std::cout << s << std::flush;
#ifdef _WIN32
			OutputDebugStringA(s.c_str());
#endif
			};

		auto trim = [](std::string& s) {
			auto notSpace = [](unsigned char c) { return !std::isspace(c); };
			s.erase(s.begin(), std::find_if(s.begin(), s.end(), notSpace));
			s.erase(std::find_if(s.rbegin(), s.rend(), notSpace).base(), s.end());
			};

		auto toLower = [](std::string s) {
			for (auto& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
			return s;
			};

		namespace fs = std::filesystem;

		std::vector<std::pair<std::string, int>> rows;

		// --- log cwd + target
		log("\n[Scoreboard] Loading scores from: " + path + "\n");
		try { log("[Scoreboard] CWD: " + fs::current_path().string() + "\n"); }
		catch (...) {}

		// --- build candidate paths
		std::vector<std::string> candidates;
		candidates.push_back(path);
		if (path.find('/') == std::string::npos && path.find('\\') == std::string::npos) {
			candidates.push_back("../Data/" + path);
			candidates.push_back("Data/" + path);
		}

		// --- try to open one
		std::ifstream f;
		std::string opened;
		for (const auto& p : candidates) {
			f.close();
			f.clear();
			f.open(p);
			log(std::string("[Scoreboard] try: ") + p + (f.is_open() ? "  [OK]\n" : "  [NO]\n"));
			if (f.is_open()) { opened = p; break; }
		}
		if (!f.is_open()) {
			log("[Scoreboard] FAILED to open any candidate.\n");
			return rows;
		}
		log("[Scoreboard] Using: " + opened + "\n");

		// --- parse lines: name,score  (skip header, blanks, comments)
		std::string line;
		bool firstLine = true;
		while (std::getline(f, line)) {
			if (firstLine && line.size() >= 3 &&
				(unsigned char)line[0] == 0xEF && (unsigned char)line[1] == 0xBB && (unsigned char)line[2] == 0xBF)
				line.erase(0, 3);
			firstLine = false;

			std::string raw = line;
			trim(line);
			if (line.empty() || line[0] == '#' || line[0] == ';') continue;

			auto comma = line.find(',');
			if (comma == std::string::npos) { log("[Scoreboard] skip (no comma): " + raw + "\n"); continue; }

			std::string name = line.substr(0, comma);
			std::string score = line.substr(comma + 1);
			trim(name); trim(score);
			if (name.empty() || score.empty()) { log("[Scoreboard] skip (empty field): " + raw + "\n"); continue; }
			if (toLower(name) == "name" && toLower(score) == "score") { log("[Scoreboard] skip header\n"); continue; }

			char* end = nullptr;
			long v = std::strtol(score.c_str(), &end, 10);
			if (end == score.c_str() || *end != '\0') { log("[Scoreboard] skip (non-numeric): " + raw + "\n"); continue; }

			rows.emplace_back(std::move(name), (int)v);
			log("[Scoreboard] parsed: " + rows.back().first + " -> " + std::to_string(rows.back().second) + "\n");
		}

		std::sort(rows.begin(), rows.end(), [](const auto& a, const auto& b) { return a.second > b.second; });

		log("[Scoreboard] total rows: " + std::to_string(rows.size()) + "\n");
		return rows;
	}


} // namespace dae
