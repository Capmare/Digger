#include "MovementComponent.h"
#include "MapComponent.h"
#include <MathUtils.h>
#include "AnimControllerComponent.h"

void dae::MovementComponent::AddMovementInput(const glm::vec3& Value)
{
	if (!bCanMoveAgain) return;

	if (GetOwner())
	{
		double ValueLength = sqrt(pow(Value.x, 2) + pow(Value.y, 2) + pow(Value.z, 2));
		if (ValueLength > 0)
		{
			bCanMoveAgain = false;
		}
		m_DesiredPosition = GetOwner()->GetLocalTransform().m_position + Value;
		
	}
}


void dae::MovementComponent::FixedUpdate(const float fixedDeltaTime)
{
	AnimControllerComponent* AnimController = GetOwner()->GetFirstComponentOfType<AnimControllerComponent>();
	if (AnimController->GetCurrentState()->GetStateName() == "Dead") 
	{
		TimeOfDeath += fixedDeltaTime;
		if (TimeOfDeath > 2)
		{
			TimeOfDeath = 0;
			GetOwner()->SetLocalPosition(0, 140);
			AnimController->ChangeState("Idle");
			bIsSpawning = true;
		}
		return;
	}

	if (GetOwner() && bIsSpawning)
	{
		m_DesiredPosition = GetOwner()->GetLocalTransform().m_position;
		bIsSpawning = false;
		return;
	}

	if (GetOwner())
	{
		bool xLoc = dae::MathUtils::IsNearlyEqual(m_DesiredPosition.x, GetOwner()->GetLocalTransform().m_position.x);
		bool yLoc = dae::MathUtils::IsNearlyEqual(m_DesiredPosition.y, GetOwner()->GetLocalTransform().m_position.y);
		if (xLoc && yLoc)
		{
			bCanMoveAgain = true;
		}
		else
		{
			GetOwner()->SetLocalPosition(dae::MathUtils::Lerp(GetOwner()->GetLocalTransform().m_position, m_DesiredPosition, fixedDeltaTime * m_LerpSpeed));
			m_GridComponent->ClearTunnelArea({ GetOwner()->GetLocalTransform().m_position.x + 10,GetOwner()->GetLocalTransform().m_position.y+10 }, 10);
		}
		return;
	}
}



void dae::MovementComponent::AddMovementInput(const glm::vec2& Value)
{
	AddMovementInput(glm::vec3(Value.x, Value.y,0));
}

void dae::MovementComponent::AddMovementInput(const int x, const int y)
{
	AddMovementInput(glm::vec3(x, y, 0));
}
