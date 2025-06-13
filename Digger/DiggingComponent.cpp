#include "DiggingComponent.h"
#include "glm.hpp"
#include "MapComponent.h"
#include "AnimationState.h"

void dae::DiggingComponent::DigRight(const bool bInvert)
{
	if (m_AnimControllerComp->GetCurrentState()->GetStateName() == "Dead") return;
	
	if (m_AnimControllerComp)
	{
		bInvert ? m_AnimControllerComp->RotateAnimation(180) : m_AnimControllerComp->RotateAnimation(0);
	}
	
	

	if (m_Grid)
	{
		glm::ivec2 CurrentPosition = 
		{
			static_cast<int>((GetOwner()->GetLocalTransform().m_position).x + 10),
			static_cast<int>((GetOwner()->GetLocalTransform().m_position).y )
		};

	}
}

void dae::DiggingComponent::DigUp(const bool bInvert)
{
	if (m_AnimControllerComp->GetCurrentState()->GetStateName() == "Dead") return;

	if (m_AnimControllerComp)
	{
		bInvert ? m_AnimControllerComp->RotateAnimation(90) : m_AnimControllerComp->RotateAnimation(270);
	}
	

	if (m_Grid)
	{
		glm::ivec2 CurrentPosition =
		{
			static_cast<int>((GetOwner()->GetLocalTransform().m_position).x + 10),
			static_cast<int>((GetOwner()->GetLocalTransform().m_position).y)
		};

	}
}
