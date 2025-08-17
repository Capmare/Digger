#pragma once
#include "BaseComponent.h"
#include "TextComponent.h"
#include "Observer.h"
#include "GameObject.h"
#include <fstream>
#include <algorithm>
#include <vector>
#include <sstream>

extern std::string g_PlayerName;

namespace dae
{
	class ScoreComponent : public BaseComponent
	{
	public:

		ScoreComponent(GameObject* Owner) : BaseComponent(Owner) {};
		
		virtual ~ScoreComponent() = default;
		
		ScoreComponent(const ScoreComponent&) = delete;
		ScoreComponent(ScoreComponent&&) noexcept = delete;
		ScoreComponent& operator=(const ScoreComponent&) = delete;
		ScoreComponent& operator=(ScoreComponent&&) noexcept = delete;

		int IncreaseScore(int ScoreIncreaseAmmount = 1)
		{
			m_Score = std::stoi(m_ScoreText->GetText());
			m_Score += ScoreIncreaseAmmount;
			m_Subject.NotifyObservers(*GetOwner(), ObserverEvents::OnScoreChanged);
			
			std::string ScoreString{};


			if (m_Score < 1000)
			{
				ScoreString += "00";

			}
			else if (m_Score < 10000)
			{
				ScoreString += "0";
			}

			ScoreString += std::to_string(m_Score);
			m_ScoreText->SetText(ScoreString);


			return m_Score;
		}

		void BindTextComponent(TextComponent* TextComp)
		{
			m_ScoreText = TextComp;
		}

		TextComponent* GetScoreText() const { return m_ScoreText; }
		int GetScore() const { return m_Score; }

		Subject m_Subject;

		void SaveScore(const std::string& playerName, const std::string& filePath = "../Data/scoreboard.txt")
		{
			std::vector<std::pair<std::string, int>> scores;

			{
				std::ifstream file(filePath);
				if (file.is_open())
				{
					std::string line;
					while (std::getline(file, line))
					{
						if (line.empty()) continue;

						std::istringstream ss(line);
						std::string name;
						std::string scoreStr;

						if (std::getline(ss, name, ',') && std::getline(ss, scoreStr))
						{
							try {
								int score = std::stoi(scoreStr);
								scores.emplace_back(name, score);
							}
							catch (...) {
								std::cerr << "[ScoreComponent] Invalid score in file: " << line << "\n";
							}
						}
					}
				}
			}

			auto it = std::find_if(scores.begin(), scores.end(),
				[&](const auto& entry) { return entry.first == playerName; });

			if (it != scores.end())
			{
				if (m_Score > it->second)
				{
					scores.erase(it);
					scores.emplace_back(playerName, m_Score);
				}
				else
				{
					std::cout << "[ScoreComponent] Not saving, existing score ("
						<< it->second << ") is higher or equal than " << m_Score << "\n";
				}
			}
			else
			{
				scores.emplace_back(playerName, m_Score);
			}

			std::sort(scores.begin(), scores.end(),
				[](const auto& a, const auto& b) { return a.second > b.second; });

			if (scores.size() > 5)
				scores.resize(5);

			{
				std::ofstream out(filePath, std::ios::trunc);
				if (!out.is_open())
				{
					std::cerr << "[ScoreComponent] Failed to write scoreboard file: " << filePath << "\n";
					return;
				}

				for (auto& [name, score] : scores)
				{
					out << name << "," << score << "\n";
				}
			}

			std::cout << "[ScoreComponent] Saved score: "
				<< playerName << " -> " << m_Score
				<< " into " << filePath << "\n";
		}



	private:
		TextComponent* m_ScoreText{};

		int m_Score{};
	};
	

}
