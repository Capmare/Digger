#include "MovementComponent.h"
#include "GridComponent.h"

void dae::MovementComponent::AddMovementInput(const glm::vec3& Value) const
{
	if (GetOwner())
	{
		GetOwner()->SetLocalPosition(GetOwner()->GetLocalTransform().m_position + Value);
		
	}
}

void dae::MovementComponent::AddMovementInput(const glm::vec2& Value) const
{
	AddMovementInput(glm::vec3(Value.x, Value.y,0));


}

void dae::MovementComponent::AddMovementInput(const int x, const int y) const
{
	AddMovementInput(glm::vec3(x, y, 0));
}
