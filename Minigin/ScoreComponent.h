#pragma once
#include "BaseComponent.h"
#include "TextComponent.h"
#include "Observer.h"
#include "GameObject.h"


namespace dae
{
	class ScoreComponent : public BaseComponent
	{
	public:

		ScoreComponent(GameObject* Owner) : BaseComponent(Owner) {};
		virtual ~ScoreComponent() = default;
		
		ScoreComponent(const ScoreComponent&) = delete;
		ScoreComponent(ScoreComponent&&) noexcept = delete;
		ScoreComponent& operator=(const ScoreComponent&) = delete;
		ScoreComponent& operator=(ScoreComponent&&) noexcept = delete;

		int IncreaseScore(int HealthIncreaseAmmount = 1)
		{

			m_Score += HealthIncreaseAmmount;
			m_Subject.NotifyObservers(*GetOwner(), ObserverEvents::OnScoreChanged);
			if (m_Score >= 500)
			{
				m_Subject.NotifyObservers(*GetOwner(), ObserverEvents::Scored500);

			}

			return m_Score;
		}

		void BindTextComponent(TextComponent* TextComp)
		{
			m_ScoreText = TextComp;
		}

		TextComponent* GetScoreText() const { return m_ScoreText; }
		int GetScore() const { return m_Score; }

		Subject m_Subject;
	private:
		TextComponent* m_ScoreText{};

		int m_Score{};
	};

}
