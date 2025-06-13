#include "DamageComponent.h"
#include "Renderer.h"
#include "AnimControllerComponent.h"
#include "MathUtils.h"
#include "HealthComponent.h"
#include "GravityComponent.h"

void dae::DamageComponent::Update(const float )
{
	glm::ivec4 DmgRect = {
		static_cast<int>(GetOwner()->GetWorldPosition().x + m_DamageSquare.x + 3),
		static_cast<int>(GetOwner()->GetWorldPosition().y + m_DamageSquare.y + 3),
		m_DamageSquare.z - 3,
		m_DamageSquare.w - 3
	};

	GravityComponent* GravityComp = GetOwner()->GetFirstComponentOfType<GravityComponent>();
	if (GravityComp)
	{
		bShouldDamage = GravityComp->GetIsFalling() && !GravityComp->GetIsBroken();
	}

	for (GameObject* Actors : m_OtherActors)
	{
		if (!bShouldDamage) continue;

		if (AnimControllerComponent* AnimComponent = Actors->GetFirstComponentOfType<AnimControllerComponent>())
		{
			glm::ivec2 Resolution = AnimComponent->GetCurrentState()->GetFlipBook()->GetUsedTexture()->GetTextureResolution();
			Resolution.x /= 4;  

			glm::ivec2 ActorPos = { static_cast<int>(Actors->GetWorldPosition().x), static_cast<int>(Actors->GetWorldPosition().y) };

			SDL_Rect ActorRect = {
				ActorPos.x + m_CollisionOffset,
				ActorPos.y + m_CollisionOffset,
				Resolution.x - 2 * m_CollisionOffset,
				Resolution.y - 2 * m_CollisionOffset
			};

			SDL_Rect DamageRect = {
				DmgRect.x, DmgRect.y, DmgRect.z, DmgRect.w
			};

			// Check for AABB overlap
			bool isOverlapping = SDL_HasIntersection(&DamageRect, &ActorRect);
			if (isOverlapping)
			{
				if (HealthComponent* HealthComp = Actors->GetFirstComponentOfType<HealthComponent>())
				{
					if (AnimComponent->GetCurrentState()->GetStateName() != "Dead")
					{
						HealthComp->DecreaseHealth(1);
						AnimComponent->GetCurrentState()->GetFlipBook()->ResetFlipBook();
						AnimComponent->ChangeState("Dead");
					}
				}
			}
		}
	}
	
}

void dae::DamageComponent::Render() const
{
	// auto* renderer = Renderer::GetInstance().GetSDLRenderer();
	// 
	// // Draw the damage rectangle
	// SDL_Rect dmgRect = {
	// 	static_cast<int>(GetOwner()->GetWorldPosition().x + m_DamageSquare.x),
	// 	static_cast<int>(GetOwner()->GetWorldPosition().y + m_DamageSquare.y),
	// 	m_DamageSquare.z,
	// 	m_DamageSquare.w
	// };
	// 
	// SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); 
	// SDL_RenderDrawRect(renderer, &dmgRect);
	// 
	// // Draw each actor's collision bounds
	// for (GameObject* Actors : m_OtherActors)
	// {
	// 	const AnimControllerComponent* AnimComponent = Actors->GetFirstComponentOfType<AnimControllerComponent>();
	// 	if (AnimComponent)
	// 	{
	// 		glm::ivec2 Resolution = AnimComponent->GetCurrentState()->GetFlipBook()->GetUsedTexture()->GetTextureResolution();
	// 		Resolution.x /= 4;
	// 
	// 		SDL_Rect actorRect = {
	// 			static_cast<int>(Actors->GetWorldPosition().x + m_CollisionOffset),
	// 			static_cast<int>(Actors->GetWorldPosition().y + m_CollisionOffset),
	// 			Resolution.x - 2 * m_CollisionOffset,
	// 			Resolution.y - 2 * m_CollisionOffset
	// 		};
	// 
	// 		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); 
	// 		SDL_RenderDrawRect(renderer, &actorRect);
	// 	}
	// }
	// 
	// SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

}
