#pragma once
#include "BaseComponent.h"
#include "GameObject.h"
#include "MapComponent.h"

namespace dae
{
	class MovementComponent : public BaseComponent

	{
	public:
		MovementComponent(GameObject* Owner, MapComponent* GridComp) : BaseComponent(Owner), m_GridComponent(GridComp) {
		};
		virtual ~MovementComponent() = default;
		
		MovementComponent(const MovementComponent&) = delete;
		MovementComponent(MovementComponent&&) noexcept = delete;
		MovementComponent& operator=(const MovementComponent&) = delete;
		MovementComponent& operator=(MovementComponent&&) noexcept = delete;

		void AddMovementInput(const glm::vec3& Value);
		void AddMovementInput(const int x, const int y);
		void AddMovementInput(const glm::vec2& Value);

		void FixedUpdate(const float fixedDeltaTime) override;

		float m_LerpSpeed{20};

		bool bIsSpawning{true};

	private:

		float TimeOfDeath{};
		
		bool bCanMoveAgain{ true };
		glm::vec3 m_DesiredPosition{};
		MapComponent* m_GridComponent{};
	};
}


