#include "DiggingComponent.h"
#include "glm.hpp"
#include "GridComponent.h"
#include "AnimationState.h"

void dae::DiggingComponent::DigRight(const bool bInvert)
{
	if (m_AnimControllerComp)
	{
		bInvert ? m_AnimControllerComp->RotateAnimation(180) : m_AnimControllerComp->RotateAnimation(0);
	}
	
	

	if (m_Grid)
	{
	

		glm::ivec2 CurrentTile = m_Grid->GetTileAtPixel
		(
			static_cast<int>((GetOwner()->GetLocalTransform().m_position).x + (bInvert ? 0 : m_TextureResolution.x)),
			static_cast<int>((GetOwner()->GetLocalTransform().m_position).y + m_TextureResolution.y)
		);

		m_Grid->UpdateTileType(
			CurrentTile,
			dae::TileType::Tunnel
		);
	}
}

void dae::DiggingComponent::DigUp(const bool bInvert)
{
	if (m_AnimControllerComp)
	{
		bInvert ? m_AnimControllerComp->RotateAnimation(90) : m_AnimControllerComp->RotateAnimation(270);
	}
	

	if (m_Grid)
	{
		glm::ivec2 CurrentTile = m_Grid->GetTileAtPixel
		(
			static_cast<int>((GetOwner()->GetLocalTransform().m_position).x + m_TextureResolution.x),
			static_cast<int>((GetOwner()->GetLocalTransform().m_position).y + (bInvert ? m_TextureResolution.y : 0))
		);

		m_Grid->UpdateTileType(
			CurrentTile,
			dae::TileType::Tunnel
		);
	}
}
