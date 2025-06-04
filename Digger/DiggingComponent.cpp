#include "DiggingComponent.h"
#include "glm.hpp"
#include "GridComponent.h"
#include "GameObject.h"
#include "AnimControllerComponent.h"

void dae::DiggingComponent::DigRight(const bool bInvert)
{
	AnimControllerComponent* AnimControllerComp = GetOwner()->GetFirstComponentOfType<AnimControllerComponent>();
	if (AnimControllerComp)
	{
		bInvert ? AnimControllerComp->RotateAnimation(180) : AnimControllerComp->RotateAnimation(0);
	}
	
	

	if (m_Grid)
	{
		glm::ivec2 CurrentTile = m_Grid->GetTileAtPixel
		(
			static_cast<int>((GetOwner()->GetLocalTransform().m_position).x + (bInvert ? 0 : 100)),
			static_cast<int>((GetOwner()->GetLocalTransform().m_position).y + 50)
		);

		m_Grid->UpdateTileType(
			CurrentTile,
			dae::TileType::Tunnel
		);
	}
}

void dae::DiggingComponent::DigUp(const bool bInvert)
{
	AnimControllerComponent* AnimControllerComp = GetOwner()->GetFirstComponentOfType<AnimControllerComponent>();
	if (AnimControllerComp)
	{
		bInvert ? AnimControllerComp->RotateAnimation(90) : AnimControllerComp->RotateAnimation(270);
	}
	

	if (m_Grid)
	{
		glm::ivec2 CurrentTile = m_Grid->GetTileAtPixel
		(
			static_cast<int>((GetOwner()->GetLocalTransform().m_position).x + 50),
			static_cast<int>((GetOwner()->GetLocalTransform().m_position).y + (bInvert ? 100 : 0))
		);

		m_Grid->UpdateTileType(
			CurrentTile,
			dae::TileType::Tunnel
		);
	}
}
