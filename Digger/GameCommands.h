#pragma once
#include "Command.h"
#include "ServiceLocator.h"


namespace dae
{
	class Command_Move final : public Command
	{
	public:
		Command_Move(const glm::vec2& Direction) : m_Direction{ Direction } {};

		void Exec(GameObject& GO) override
		{
			MovementComponent* MovementComp = GO.GetAllComponentsOfType<MovementComponent>().at(0);
			if (MovementComp)
			{
				MovementComp->AddMovementInput(m_Direction);
			}
		}
		glm::vec2 m_Direction{};
	};


	class Command_DecreaseHealth final : public Command
	{
		void Exec(GameObject& GO) override
		{
			HealthComponent* HealthComp = GO.GetAllComponentsOfType<HealthComponent>().at(0);
			if (HealthComp)
			{
				HealthComp->DecreaseHealth();
			}

		}
	};


	class Command_IncreaseScore final : public Command
	{
		void Exec(GameObject& GO) override
		{
			dae::ScoreComponent* ScoreComp = GO.GetAllComponentsOfType<ScoreComponent>().at(0);
			if (ScoreComp)
			{
				ScoreComp->IncreaseScore(10);
			}
		}
	};

	class Command_StopAllSound final : public Command
	{
		void Exec(GameObject& GO) override
		{
			(void)GO;
			dae::ServiceLocator::GetSoundSystem().StopAllSound();
		}
	};

	class Command_PlaySound final : public Command
	{
		void Exec(GameObject& GO) override
		{
			(void)GO;
			dae::ServiceLocator::GetSoundSystem().PlaySound("../Data/piano2.wav");

		}
	};

}