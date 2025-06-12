#include "DamageComponent.h"
#include "Renderer.h"
#include "AnimControllerComponent.h"

void dae::DamageComponent::Update(const float )
{
	if (bShouldDamage)
	{
		for (GameObject* Actors : m_OtherActors)
		{
			if (const AnimControllerComponent* AnimComponent = Actors->GetFirstComponentOfType<AnimControllerComponent>())
			{
				const glm::ivec2 Resolution = AnimComponent->GetCurrentState()->GetFlipBook()->GetUsedTexture()->GetTextureResolution();
				if (
					CheckPointInSquare(m_DamageSquare, Actors->GetWorldPosition()) ||
					CheckPointInSquare(m_DamageSquare,{Actors->GetWorldPosition().x + Resolution.x,Actors->GetWorldPosition().y }) ||
					CheckPointInSquare(m_DamageSquare,{Actors->GetWorldPosition().x ,Actors->GetWorldPosition().y + Resolution.y }) ||
					CheckPointInSquare(m_DamageSquare,{Actors->GetWorldPosition().x + Resolution.x ,Actors->GetWorldPosition().y + Resolution.y })
					)
				{
					// damage
				}
			}
			
		}
	}
}

void dae::DamageComponent::Render() const
{
	SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 0, 0, 255); 
	const SDL_Rect rect = { static_cast<int>(GetOwner()->GetWorldPosition().x + m_DamageSquare.x), static_cast<int>(GetOwner()->GetWorldPosition().y + m_DamageSquare.y), m_DamageSquare.z, m_DamageSquare.w };

	SDL_RenderDrawRect(Renderer::GetInstance().GetSDLRenderer(),&rect);
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
