#include "GameObservers.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "SoundSystem.h"
#include "ServiceLocator.h"



void dae::HealthObserver::OnNotify(GameObject& GO, const ObserverEvents& Event)
{
	HealthComponent* HealthComp = GO.GetAllComponentsOfType<dae::HealthComponent>().at(0);
	std::string HealthText{ "Current health: " };
	if (HealthComp)
	{
		switch (Event)
		{
		case ObserverEvents::OnHealthChanged:

			//dae::ServiceLocator::GetSoundSystem().PlaySound("../Data/piano2.wav");
			HealthText += std::to_string(HealthComp->GetHealth());
			HealthComp->GetHealthText()->SetText(HealthText);
			break;
		case ObserverEvents::OnDeath:
			GO.Destroy();
			std::cout << GO.m_Name << " has died";
			// player dead
			break;
		default:
			break;
		}
	}
}

void dae::ScoreObserver::OnNotify(GameObject& GO, const ObserverEvents& Event)
{
	dae::ScoreComponent* ScoreComp = GO.GetAllComponentsOfType<dae::ScoreComponent>().at(0);

	std::string ScoreText{ "Current score:" };
	if (ScoreComp)
	{
		switch (Event)
		{
		case ObserverEvents::OnScoreChanged:
			ScoreText += std::to_string(ScoreComp->GetScore());
			ScoreComp->GetScoreText()->SetText(ScoreText);
			break;
		default:
			break;
		}
	}
}
