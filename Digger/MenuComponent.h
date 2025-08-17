// MenuComponent.h
#pragma once
#include "BaseComponent.h"
#include <cstdint>
#include <vector>

namespace dae {
	class MenuComponent final : public BaseComponent {
	public:
		explicit MenuComponent(GameObject* owner);
		void Update(float dt) override;


		// builders
		void BuildScoreboard();
	private:

		void Trim(std::string& s);
		std::vector<std::pair<std::string, int>> LoadScores(const std::string& path);
		bool ParseLine(const std::string& line, std::string& nameOut, int& scoreOut);


		GameObject* m_ScoreRoot{};

		bool m_HasSelected{ false };

		// edge detection bootstrap
		bool m_firstUpdate{ true };
		uint16_t m_PrevButtons{ 0 }; // previous XInput buttons

		bool m_prevKeyA{ false };
		bool m_prevKeyX{ false };
		bool m_prevKeyY{ false };


		std::string m_ScoreFile{ "scoreboard.txt" };

		// layout
		float m_MaxRows{ 5 };
		float m_TitleX{ 380 }, m_TitleY{ 46 };
		float m_HeaderY{ 68 };
		float m_NameX{ 380 }, m_ScoreX{ 540 };
		float m_BaseY{ 88 }, m_LineStep{ 18 };


	};
} // namespace dae
