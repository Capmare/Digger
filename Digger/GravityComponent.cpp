#include "GravityComponent.h"
#include "MapComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "AnimControllerComponent.h"

dae::GravityComponent::GravityComponent(GameObject* Owner, class MapComponent* GridComp) : BaseComponent(Owner), m_GridComponent(GridComp)
{
	AnimComponent = GetOwner()->GetFirstComponentOfType<AnimControllerComponent>();
}

void dae::GravityComponent::FixedUpdate(const float fixedDeltaTime)
{
	if (m_GridComponent)
	{
		glm::ivec2 LeftSide = m_GridComponent->GetTileAtPixel(static_cast<int>(GetOwner()->GetWorldPosition().x ), static_cast<int>(GetOwner()->GetWorldPosition().y + 50));
		glm::ivec2 Middle = m_GridComponent->GetTileAtPixel(static_cast<int>(GetOwner()->GetWorldPosition().x + 25), static_cast<int>(GetOwner()->GetWorldPosition().y + 50));
		glm::ivec2 RightSide = m_GridComponent->GetTileAtPixel(static_cast<int>(GetOwner()->GetWorldPosition().x + 50), static_cast<int>(GetOwner()->GetWorldPosition().y + 50));
		
		// this becomes true 
		if (m_GridComponent->GetTileType(LeftSide) == TileType::Tunnel && m_GridComponent->GetTileType(Middle) == TileType::Tunnel && m_GridComponent->GetTileType(RightSide) == TileType::Tunnel)
		{
			if (AnimComponent)
			{
				AnimComponent->ChangeState("Falling");
			}
			bIsFalling = true;
			GetOwner()->SetLocalPosition(static_cast<int>(GetOwner()->GetLocalTransform().m_position.x), static_cast<int>(GetOwner()->GetLocalTransform().m_position.y + 150 * fixedDeltaTime));
			return;
		}
		if (AnimComponent)
		{
			AnimComponent->ChangeState("Idle");
		}
		bIsFalling = false;
	}
}

void dae::GravityComponent::Render() const
{

}
