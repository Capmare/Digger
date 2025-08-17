#pragma once
#include "Command.h"
#include "ServiceLocator.h"
#include "DiggingComponent.h"
#include <ProjectileComponent.h>


namespace dae
{
	class Command_Move final : public Command
	{
	public:
		Command_Move(const glm::vec2& Direction) : m_Direction{ Direction } {};

		void Exec(GameObject& GO) override
		{
			MovementComponent* MovementComp = GO.GetFirstComponentOfType<MovementComponent>();
			if (MovementComp)
			{
				MovementComp->AddMovementInput(m_Direction);
			}
			DiggingComponent* DigComponent = GO.GetFirstComponentOfType<DiggingComponent>();
			if (DigComponent)
			{
				if (m_Direction.x > 0)
				{
					DigComponent->DigRight(false);
				}
				if (m_Direction.x < 0)
				{
					DigComponent->DigRight(true);
				}

				if (m_Direction.y > 0)
				{
					DigComponent->DigUp(true);
				}
				if (m_Direction.y < 0)
				{
					DigComponent->DigUp(false);
				}
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

	class Command_Shoot final : public Command
	{
	public:
		Command_Shoot(const glm::vec2& Direction) : m_Direction{ Direction } {};

		void Exec(GameObject& GO) override
		{
			dae::FireballShooterComponent* ScoreComp = GO.GetAllComponentsOfType<FireballShooterComponent>().at(0);
			if (ScoreComp)
			{
				ScoreComp->TryFire(m_Direction);
			}
		}

	private:
		glm::vec2 m_Direction;
	};
}