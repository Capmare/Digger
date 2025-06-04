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

		void AddMovementInput(const glm::vec3& Value) const;
		void AddMovementInput(const int x, const int y) const;
		void AddMovementInput(const glm::vec2& Value) const;


	private:
	};
}


