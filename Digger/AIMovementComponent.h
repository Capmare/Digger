#pragma once
#include <BaseComponent.h>
#include <vector>
#include "glm.hpp"

namespace dae
{
	

	class AIGridComponent;
	class AnimControllerComponent;
	class MapComponent;

	class AIMovementComponent :
		public BaseComponent
	{
	public:
		AIMovementComponent(GameObject* Owner,AIGridComponent* GridComp, AnimControllerComponent* AnimController,GameObject* Player, MapComponent* MapComp ) 
			: BaseComponent(Owner), m_GridComponent(GridComp), m_AnimControllerComp(AnimController), m_Player(Player), m_MapComponent(MapComp) {

			RecreatePath();
			m_NextStateChangeTime = 10.0f + static_cast<float>(rand() % 11);
		};
		virtual ~AIMovementComponent() = default;
		
		AIMovementComponent(const AIMovementComponent&) = delete;
		AIMovementComponent(AIMovementComponent&&) noexcept = delete;
		AIMovementComponent& operator=(const AIMovementComponent&) = delete;
		AIMovementComponent& operator=(AIMovementComponent&&) noexcept = delete;
		void RecreatePath(int idx = 1);


		void FixedUpdate(const float fixedDeltaTime) override;
		void Render() const override;

		bool bStopPathFinding{};
	private:

		std::string m_DefaultState = "Nobbin";
		std::string m_AlternateState = "Hobbin";
		float m_StateChangeTimer = 0.0f;
		float m_NextStateChangeTime = 0.0f;
		bool m_IsInAlternateState = false;
		float m_AlternateStateTimer = 0.0f;

		std::vector<glm::ivec2> m_Destination{};
		GameObject* m_Player{};
		AnimControllerComponent* m_AnimControllerComp{};
		AIGridComponent* m_GridComponent{};
		MapComponent* m_MapComponent{};

		glm::vec2 m_StartPos{};
		glm::vec2 m_TargetPos{};

		float m_SegmentDuration{ 0.0f };
		float m_MoveLerpTime{ 0.0f };
		const float speed{ 30.f };
		bool m_MovingToTarget{ false };

		float m_MaxTime{2.f};
		float m_PassedTime{};


	};

}
