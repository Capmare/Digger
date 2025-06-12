#include "DamageComponent.h"
#include "Renderer.h"
#include "AnimControllerComponent.h"
#include "MathUtils.h"
#include "HealthComponent.h"
#include "GravityComponent.h"

void dae::DamageComponent::Update(const float )
{
	glm::ivec4 DmgLocation = { static_cast<int>(GetOwner()->GetWorldPosition().x + m_DamageSquare.x+3), static_cast<int>(GetOwner()->GetWorldPosition().y + m_DamageSquare.y+3), m_DamageSquare.z-3, m_DamageSquare.w-3 };
	GravityComponent* GravityComp = GetOwner()->GetFirstComponentOfType<GravityComponent>();
	if (GravityComp)
	{
		bShouldDamage = GravityComp->GetIsFalling();
	}

	for (GameObject* Actors : m_OtherActors)
	{
		if (bShouldDamage)
		{
			if (AnimControllerComponent* AnimComponent = Actors->GetFirstComponentOfType<AnimControllerComponent>())
			{
				const glm::ivec2 Resolution = AnimComponent->GetCurrentState()->GetFlipBook()->GetUsedTexture()->GetTextureResolution();
				if (
					
					dae::MathUtils::CheckPointInSquare(DmgLocation, {Actors->GetWorldPosition().x + 5, Actors->GetWorldPosition().y + 5}) ||
					dae::MathUtils::CheckPointInSquare(DmgLocation, { Actors->GetWorldPosition().x + Resolution.x - 5,Actors->GetWorldPosition().y + 5 }) ||
					dae::MathUtils::CheckPointInSquare(DmgLocation, { Actors->GetWorldPosition().x + 5 ,Actors->GetWorldPosition().y + Resolution.y - 5 }) ||
					dae::MathUtils::CheckPointInSquare(DmgLocation, { Actors->GetWorldPosition().x + Resolution.x - 5 ,Actors->GetWorldPosition().y + Resolution.y-5 })
					)
				{
					HealthComponent* HealthComp = Actors->GetFirstComponentOfType<HealthComponent>();
					if (HealthComp)
					{
						HealthComp->DecreaseHealth(1);
						AnimComponent->ChangeState("Dead");
					}


				}
			}
		}

		

	}
	
}

void dae::DamageComponent::Render() const
{
	const SDL_Rect rect = { static_cast<int>(GetOwner()->GetWorldPosition().x + m_DamageSquare.x), static_cast<int>(GetOwner()->GetWorldPosition().y + m_DamageSquare.y), m_DamageSquare.z, m_DamageSquare.w };
	SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 0, 0, 255);

	SDL_RenderDrawRect(Renderer::GetInstance().GetSDLRenderer(), &rect);
	for (GameObject* Actors : m_OtherActors)
	{
		const AnimControllerComponent* AnimComponent = Actors->GetFirstComponentOfType<AnimControllerComponent>();
		if (AnimComponent)
		{
			glm::ivec2 Resolution = AnimComponent->GetCurrentState()->GetFlipBook()->GetUsedTexture()->GetTextureResolution();
			Resolution.x /= 4;

			SDL_RenderDrawPoint(Renderer::GetInstance().GetSDLRenderer(), static_cast<int>(Actors->GetWorldPosition().x), static_cast<int>(Actors->GetWorldPosition().y));
			SDL_RenderDrawPoint(Renderer::GetInstance().GetSDLRenderer(), static_cast<int>(Actors->GetWorldPosition().x + Resolution.x), static_cast<int>(Actors->GetWorldPosition().y));
			SDL_RenderDrawPoint(Renderer::GetInstance().GetSDLRenderer(), static_cast<int>(Actors->GetWorldPosition().x), static_cast<int>(Actors->GetWorldPosition().y + Resolution.y));
			SDL_RenderDrawPoint(Renderer::GetInstance().GetSDLRenderer(), static_cast<int>(Actors->GetWorldPosition().x + Resolution.x), static_cast<int>(Actors->GetWorldPosition().y + Resolution.y));
		}


	}


	SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 0, 0, 255);

}
