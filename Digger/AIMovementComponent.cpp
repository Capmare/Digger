#include "AIMovementComponent.h"
#include "AnimControllerComponent.h"
#include "AIGridComponent.h"
#include "GameObject.h"

void dae::AIMovementComponent::FixedUpdate(const float fixedDeltaTime)
{
	if (m_Destination.empty()) return;

	const float speed = 10.0f;
	glm::vec2 currentPos = GetOwner()->GetWorldTransform().m_position;
	glm::vec2 targetPos = glm::vec2(m_Destination.front());

	glm::vec2 direction = targetPos - currentPos;
	float distance = glm::length(direction);

	const float arrivalThreshold = 2.0f;

	if (distance < arrivalThreshold)
	{
		GetOwner()->GetWorldTransform().m_position = glm::vec3{targetPos,0};
		m_Destination.erase(m_Destination.begin());
	}
	else
	{
		glm::vec2 normalizedDir = glm::normalize(direction);
		glm::vec2 movement = normalizedDir * speed * fixedDeltaTime;

		if (glm::length(movement) > distance)
			movement = direction; // Don't overshoot

		GetOwner()->GetWorldTransform().m_position += glm::vec3(movement, 0.0f);
	}
}

void dae::AIMovementComponent::RecreatePath()
{
	if (m_Player)
	{
		if (m_AnimControllerComp->GetCurrentState()->GetStateName() == "Nobbin")
		{
			m_Destination = std::move(m_GridComponent->FindPathFreeTilesFromPixels(GetOwner()->GetWorldTransform().m_position, m_Player->GetWorldPosition()));
		}
		else if (m_AnimControllerComp->GetCurrentState()->GetStateName() == "Hobbin")
		{
			m_Destination = std::move(m_GridComponent->FindPathAllMapFromPixels(GetOwner()->GetWorldTransform().m_position, m_Player->GetWorldPosition()));
		}
	}

}
