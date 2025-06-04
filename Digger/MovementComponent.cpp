#include "MovementComponent.h"
#include "GridComponent.h"

void dae::MovementComponent::AddMovementInput(const glm::vec3& Value) const
{
	if (GetOwner())
	{
		GetOwner()->SetLocalPosition(GetOwner()->GetLocalTransform().m_position + Value);
		if (m_Grid)
		{
			glm::ivec2 CurrentTile = m_Grid->GetTileAtPixel
			(
				static_cast<int>((GetOwner()->GetLocalTransform().m_position + Value).x + 40),
				static_cast<int>((GetOwner()->GetLocalTransform().m_position + Value).y)
			);

			m_Grid->UpdateTileType(
				CurrentTile,
				dae::TileType::Tunnel
			);
			
			std::cout << CurrentTile.x << " " << CurrentTile.y << std::endl;
		}
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
