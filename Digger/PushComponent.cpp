#include "PushComponent.h"
#include "GameObject.h"
#include "AnimControllerComponent.h"
#include "MathUtils.h"
#include "SDL_render.h"
#include "Renderer.h"
#include "GravityComponent.h"
#include "ScoreComponent.h"

void dae::PushComponent::Push(float PushAmmount)
{
	GravityComponent* GravityComp = GetOwner()->GetFirstComponentOfType<GravityComponent>();
	if (!GravityComp->GetIsFalling())
	{
		GetOwner()->SetLocalPosition(
			static_cast<int>(GetOwner()->GetWorldPosition().x + (m_Direction * PushAmmount)),
			static_cast<int>(GetOwner()->GetWorldPosition().y)
		);
	}
	
}

void dae::PushComponent::Update(const float )
{
	m_CollisionPoint.x = static_cast<int>(m_PushingPoint.x + GetOwner()->GetWorldPosition().x);
	m_CollisionPoint.y = static_cast<int>(m_PushingPoint.y + GetOwner()->GetWorldPosition().y);

	for (GameObject* Actor : m_OtherActors)
	{
		AnimControllerComponent* AnimController = Actor->GetFirstComponentOfType<AnimControllerComponent>();
		if (AnimController->GetCurrentState()->GetStateName() == "Dead") continue;

		glm::ivec2 ActorSize = AnimController->GetCurrentState()->GetFlipBook()->GetUsedTexture()->GetTextureResolution();
		ActorSize.x /= 4;
		glm::ivec4 CollisionSize
		{
			Actor->GetWorldPosition().x,
			Actor->GetWorldPosition().y,
			ActorSize.x,
			ActorSize.y

		};
		if (dae::MathUtils::CheckPointInSquare(CollisionSize, m_CollisionPoint))
		{
			
			AnimControllerComponent* AnimControlerBag = GetOwner()->GetFirstComponentOfType<AnimControllerComponent>();
			if (AnimControlerBag->GetCurrentState()->GetStateName() == "Destroyed")
			{
				ScoreComponent* ScoreComp = Actor->GetFirstComponentOfType<ScoreComponent>();
				if (ScoreComp)
				{
					ScoreComp->IncreaseScore(500);
					GetOwner()->Destroy();
				}
			}
			else
			{
				Push(10);
			}
		}
	}
}

void dae::PushComponent::Render() const
{
	// SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 0, 0, 255);
	// 
	// SDL_RenderDrawPoint(Renderer::GetInstance().GetSDLRenderer(), m_CollisionPoint.x, m_CollisionPoint.y);
	// SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 0, 0, 255);

}

