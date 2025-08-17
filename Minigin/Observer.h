#pragma once
#include <vector>
#include <iostream>
namespace dae
{
	class GameObject;
	class HealthComponent;

	enum class ObserverEvents
	{
		OnHealthChanged,
		OnDeath,
		OnScoreChanged,
		OnSavedScore
	};

	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void OnNotify(GameObject& GO,const ObserverEvents& Event) = 0; 
	};

	class Subject
	{
	public:
		Subject() = default;
		virtual ~Subject() = default;
		
		Subject(const Subject&) = delete;
		Subject(Subject&&) noexcept = delete;
		Subject& operator=(const Subject&) = delete;
		Subject& operator=(Subject&&) noexcept = delete;

		void AddObserver(Observer* observer)
		{
			if (observer) {
				m_Observers.emplace_back(observer);
			}
			
		}

		void RemoveObserver(Observer* observer)
		{
			m_Observers.erase(std::remove(m_Observers.begin(), m_Observers.end(), observer), m_Observers.end());
		}

		void NotifyObservers(GameObject& GO, const ObserverEvents& Event)
		{
			for (const auto& observer : m_Observers)
			{
				observer->OnNotify(GO, Event);
			}
		}
		

	private:
		std::vector<Observer*> m_Observers; 
	};



}