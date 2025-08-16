#pragma once
#include "BaseComponent.h"

namespace dae
{
	class MenuComponent final : public BaseComponent
	{
	public:
		explicit MenuComponent(GameObject* owner);
		void Update(float deltaTime) override;
	private:
		unsigned short m_PrevButtons{};
		bool m_HasSelected{ false };
	};
}