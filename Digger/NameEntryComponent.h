// NameEntryComponent.h
#pragma once
#include <string>
#include <functional>
#include <cstdint>         // for uint16_t
#include "BaseComponent.h"

namespace dae {
	class TextComponent;

	class NameEntryComponent final : public BaseComponent {
	public:
		NameEntryComponent(GameObject* owner,
			TextComponent* text,
			int padIndex = 0,
			std::function<void(const std::string&)> onDone = {});

		void Update(float dt) override;

	private:
		TextComponent* m_Text{};
		int m_PadIndex{ 0 };
		std::function<void(const std::string&)> m_OnDone;

		std::string m_Name;
		char m_Candidate{ 'A' };
		uint16_t m_PrevButtons{};       // <-- no WORD here

		static constexpr int MAX_LEN = 10;
		static constexpr const char* ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-";

		void RefreshText();
		void BumpCandidate(int delta);
	};
} // namespace dae
