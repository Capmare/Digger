#pragma once
#include <BaseComponent.h>
#include <vector>
#include "glm.hpp"

namespace dae
{
	

	class AIGridComponent;
	class AnimControllerComponent;

	class AIMovementComponent :
		public BaseComponent
	{
	public:
		AIMovementComponent(GameObject* Owner,AIGridComponent* GridComp, AnimControllerComponent* AnimController,GameObject* Player ) 
			: BaseComponent(Owner), m_GridComponent(GridComp), m_AnimControllerComp(AnimController), m_Player(Player) {};
		virtual ~AIMovementComponent() = default;
		
		AIMovementComponent(const AIMovementComponent&) = delete;
		AIMovementComponent(AIMovementComponent&&) noexcept = delete;
		AIMovementComponent& operator=(const AIMovementComponent&) = delete;
		AIMovementComponent& operator=(AIMovementComponent&&) noexcept = delete;


		void FixedUpdate(const float fixedDeltaTime) override;

	private:
		void RecreatePath();
		std::vector<glm::ivec2> m_Destination{};
		GameObject* m_Player{};
		AnimControllerComponent* m_AnimControllerComp{};
		AIGridComponent* m_GridComponent{};
	};

}
