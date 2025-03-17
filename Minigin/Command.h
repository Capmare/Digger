#pragma once
#include "GameObject.h"
#include "MovementComponent.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"


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
			MovementComponent* MovementComp = GO.GetAllComponentsOfType<MovementComponent>().at(0);
			MovementComp->AddMovementInput(m_Direction);
		}
		glm::vec2 m_Direction{};
	};


	class Command_DecreaseHealth final : public Command
	{
		void Exec(GameObject& GO) override
		{
			HealthComponent* HealthComp =  GO.GetAllComponentsOfType<HealthComponent>().at(0);
			HealthComp->DecreaseHealth();

		}
	};


	class Command_IncreaseScore final : public Command
	{
		void Exec(GameObject& GO) override
		{
			dae::ScoreComponent* ScoreComp = GO.GetAllComponentsOfType<ScoreComponent>().at(0);
			ScoreComp->IncreaseScore(100);
		}
	};

}

