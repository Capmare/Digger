#pragma once
#include "BaseComponent.h"
#include "GameObject.h"

namespace dae
{
	class MovementComponent : public BaseComponent

	{
	public:
		MovementComponent(GameObject* Owner) : BaseComponent(Owner) {};
		virtual ~MovementComponent() = default;
		
		MovementComponent(const MovementComponent&) = delete;
		MovementComponent(MovementComponent&&) noexcept = delete;
		MovementComponent& operator=(const MovementComponent&) = delete;
		MovementComponent& operator=(MovementComponent&&) noexcept = delete;

		void AddMovementInput(const glm::vec3& Value);
		void AddMovementInput(const int x, const int y);
		void AddMovementInput(const glm::vec2& Value);



		void FixedUpdate(const float fixedDeltaTime) override;

		float m_LerpSpeed{70};
	private:

		bool IsNearlyEqual(float a, float b, float tolerance = 0.001f) {
			return std::fabs(a - b) <= tolerance;
		}
		constexpr glm::vec3 Lerp(const glm::vec3& a, const glm::vec3& b, float t) {
			return a + (b - a) * t;
		}

		bool bCanMoveAgain{ true };
		glm::vec3 m_DesiredPosition{};
		
	};
}


