// MenuComponent.h
#pragma once
#include "BaseComponent.h"
#include <cstdint>

namespace dae {
	class MenuComponent final : public BaseComponent {
	public:
		explicit MenuComponent(GameObject* owner);
		void Update(float dt) override;

	private:
		bool m_HasSelected{ false };

		// edge detection bootstrap
		bool m_firstUpdate{ true };
		uint16_t m_PrevButtons{ 0 }; // previous XInput buttons

		bool m_prevKeyA{ false };
		bool m_prevKeyX{ false };
		bool m_prevKeyY{ false };
	};
} // namespace dae
