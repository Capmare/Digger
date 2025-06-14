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

		int IncreaseScore(int ScoreIncreaseAmmount = 1)
		{
			m_Score = std::stoi(m_ScoreText->GetText());
			m_Score += ScoreIncreaseAmmount;
			m_Subject.NotifyObservers(*GetOwner(), ObserverEvents::OnScoreChanged);
			
			std::string ScoreString{};


			if (m_Score < 1000)
			{
				ScoreString += "00";

			}
			else if (m_Score < 10000)
			{
				ScoreString += "0";
			}

			ScoreString += std::to_string(m_Score);
			m_ScoreText->SetText(ScoreString);

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
