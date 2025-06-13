#include "GravityComponent.h"
#include "MapComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "AnimControllerComponent.h"
#include "DamageComponent.h"

dae::GravityComponent::GravityComponent(GameObject* Owner, class MapComponent* GridComp) : BaseComponent(Owner), m_GridComponent(GridComp)
{
	AnimComponent = GetOwner()->GetFirstComponentOfType<AnimControllerComponent>();
}

void dae::GravityComponent::FixedUpdate(const float fixedDeltaTime)
{
	if (m_GridComponent)
	{
			
		int emptyCount = 0;
		const int totalChecks = 20;

		for (int idx = 0; idx < totalChecks; ++idx)
		{
			SDL_Color color = Renderer::GetInstance().ReadPixelColor(
				m_GridComponent->GetMapTexture(),
				static_cast<int>(GetOwner()->GetWorldPosition().x + idx),
				static_cast<int>(GetOwner()->GetWorldPosition().y + 20)
			);

			if (color.r < 10 && color.g < 10 && color.b < 10)
			{
				++emptyCount;
			}
		}

		bool bCanFall = (emptyCount >= totalChecks * 0.6);

		if (bCanFall)
		{

			if (!bIsFalling) m_StartFallingYPosition = GetOwner()->GetLocalTransform().m_position.x;

			if (!bIsBroken)
			{
				if (AnimComponent)
				{
					AnimComponent->ChangeState("Falling");
				}
			}
			
			bIsFalling = true;
			GetOwner()->SetLocalPosition(static_cast<int>(GetOwner()->GetLocalTransform().m_position.x), static_cast<int>(GetOwner()->GetLocalTransform().m_position.y + 80 * fixedDeltaTime));
			return;
		}
		else
		{
			if (bIsBroken) return;
			if (bIsFalling)
			{
				std::cout << GetOwner()->GetLocalTransform().m_position.y - m_StartFallingYPosition << std::endl;
				if ((abs(GetOwner()->GetLocalTransform().m_position.y - m_StartFallingYPosition)) > 30)
				{
					if (AnimComponent)
					{
						AnimComponent->ChangeState("Destroyed");
						bIsBroken = true;

					}
				}
				else
				{
					if (AnimComponent)
					{
						AnimComponent->ChangeState("Idle");
					}
				}
			}
		}
		bIsFalling = false;
	}
}

void dae::GravityComponent::Render() const
{
	// for (int idx{}; idx < 20; ++idx)
	// {
	// 	Renderer::GetInstance().DrawPoint(static_cast<int>(GetOwner()->GetWorldPosition().x + idx), static_cast<int>(GetOwner()->GetWorldPosition().y + 20), 2);
	// }
}
