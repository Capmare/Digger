#pragma once
#include <BaseComponent.h>
#include <vector>
#include "glm.hpp"
#include "Observer.h"

namespace dae
{
	class PushComponent :
		public BaseComponent
	{
	public:
		PushComponent(GameObject* Owner, glm::ivec2 PushingPoint, int Direction, int ScoreToIncrease) : BaseComponent(Owner), m_PushingPoint(PushingPoint),m_Direction(Direction), m_Score(ScoreToIncrease) {};
		virtual ~PushComponent() = default;
		
		PushComponent(const PushComponent&) = delete;
		PushComponent(PushComponent&&) noexcept = delete;
		PushComponent& operator=(const PushComponent&) = delete;
		PushComponent& operator=(PushComponent&&) noexcept = delete;

		void Push(float PushAmmount);

		void Update(const float deltaTime) override;

		glm::ivec2 CollisionPoint() const { return m_CollisionPoint; }

		void AddOtherActors(GameObject* Other) { m_OtherActors.emplace_back(Other); }

		void Render() const override;

		bool bIsInstantlyPickup{};
	private:
		
		std::vector<GameObject*> m_OtherActors{};


		glm::ivec2 m_PushingPoint{};
		glm::ivec2 m_CollisionPoint{};
		
		int m_Direction{};
		int m_Score{};
	};
}

