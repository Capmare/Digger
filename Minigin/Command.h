#pragma once
#include "GameObject.h"
#include "MovementComponent.h"


namespace dae
{
	class Command
	{
	public:
		virtual ~Command() = default;

		virtual void Exec(GameObject& GO) = 0;


	};


	class Command_Move final : public Command
	{
	public:
		Command_Move(const glm::vec2& Direction) : m_Direction{ Direction } {};

		void Exec(GameObject& GO) override
		{
			dae::MovementComponent* MovementComp = GO.GetAllComponentsOfType<dae::MovementComponent>().at(0);
			MovementComp->AddMovementInput(m_Direction);
		}
		glm::vec2 m_Direction{};
	};






}

