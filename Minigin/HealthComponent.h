#pragma once
#include "BaseComponent.h"
#include <vector>
#include <memory>
#include "TextComponent.h"
#include "Observer.h"

namespace dae
{
	class HealthComponent : public BaseComponent
	{
	public:
		HealthComponent(GameObject* Owner, int MaxStartHealth = 3) : BaseComponent(Owner), m_Health{ MaxStartHealth }, m_MaxHealth{MaxStartHealth} {};
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


			}
			if (m_Health <= 0)
			{
				m_Health = 0;
				m_Subject.NotifyObservers(*GetOwner(), ObserverEvents::OnDeath);
				// is dead
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
	private:


		TextComponent* m_HealthText{};

		int m_MaxHealth{};
		int m_Health{};
	};
}


