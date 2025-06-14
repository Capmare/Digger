#pragma once
#include "BaseComponent.h"
#include <vector>
#include <memory>
#include "TextComponent.h"
#include "Observer.h"
#include "TextureComponent.h"
#include <algorithm>
#include <ranges>
#include "FlipBookComponent.h"
#include "CombatInformationComponent.h"
#include "SpawnerComponent.h"

namespace dae
{
	class HealthComponent : public BaseComponent
	{
	public:
		HealthComponent(GameObject* Owner, int MaxStartHealth = 3) : BaseComponent(Owner), m_Health{ MaxStartHealth }, m_MaxHealth{MaxStartHealth} 
		{
			dae::FlipBookConfig DiggerFlipBookConfig{
				"Digger.png",
				3,
				20,
				20,
				.1f,
				false,
				1
			};

			for (size_t idx{}; idx < m_Health; ++idx)
			{
				const glm::ivec3 StartLocation = { 50 + 20 * idx, -3, 0 };
				m_DiggerHealthImage.emplace_back(std::make_unique<FlipBookComponent>(Owner,"diggerSingle.png",DiggerFlipBookConfig));
				m_DiggerHealthImage[idx]->GetUsedTexture()->bOverrideOwnerPosition = true;
				m_DiggerHealthImage[idx]->GetUsedTexture()->OverridenPosition = StartLocation;
			}

		};
		virtual ~HealthComponent() = default;

		HealthComponent(const HealthComponent&) = delete;
		HealthComponent(HealthComponent&&) noexcept = delete;
		HealthComponent& operator=(const HealthComponent&) = delete;
		HealthComponent& operator=(HealthComponent&&) noexcept = delete;

		int DecreaseHealth(int HealthDecreaseAmmount = 1)
		{
			if (m_Health > 0)
			{
				m_Health -= HealthDecreaseAmmount;
				m_Subject.NotifyObservers(*GetOwner(), ObserverEvents::OnHealthChanged);
				if (GetOwner()->m_Name == "Digger")
				{
					if (CombatInformationComponent* CIC = GetOwner()->GetFirstComponentOfType<CombatInformationComponent>())
					{
						CIC->m_Spawner->ResetSpawns();
					}
				}
			}
			else if (m_Health <= 0)
			{
				m_Health = 0;
				m_Subject.NotifyObservers(*GetOwner(), ObserverEvents::OnDeath);
				if (GetOwner()->m_Name == "Monster")
				{
					GetOwner()->Destroy();
				}
			}

			return m_Health;
		}

		int IncreaseHealth(int HealthIncreaseAmmount = 1)
		{

			if (m_Health < m_MaxHealth)
			{

				m_Health += HealthIncreaseAmmount;

			}

			return m_Health;
		}

		int ChangeMaxHealthValue(int NewMaxHealth)
		{
			m_MaxHealth = NewMaxHealth;
			return m_MaxHealth;
		}

		void BindTextComponent(TextComponent* TextComp)
		{
			m_HealthText = TextComp;
		}

		TextComponent* GetHealthText() const { return m_HealthText; }
		int GetHealth() const { return m_Health; }

		Subject m_Subject;

		void Render() const override
		{
			
			for (size_t idx{}; idx < m_Health; ++idx)
			{
				m_DiggerHealthImage[idx]->Render();
			}
		}

	private:


		TextComponent* m_HealthText{};

		std::vector<std::unique_ptr<FlipBookComponent>> m_DiggerHealthImage{};

		int m_MaxHealth{};
		int m_Health{};
	};
}


