#include "PushComponent.h"
#include "GameObject.h"
#include "AnimControllerComponent.h"
#include "MathUtils.h"
#include "SDL_render.h"
#include "Renderer.h"
// #include "GravityComponent.h" // removed
#include "ScoreComponent.h"
#include <string>

namespace {
	constexpr const char* kStateFalling = "Falling";
	constexpr const char* kStateDestroyed = "Destroyed";
	constexpr const char* kStateDead = "Dead";
}

void dae::PushComponent::Push(float PushAmmount)
{
	if (auto* ownerAnim = GetOwner()->GetFirstComponentOfType<AnimControllerComponent>())
	{
		const auto* st = ownerAnim->GetCurrentState();
		const std::string name = st ? st->GetStateName() : std::string{};
		if (name == kStateFalling || name == kStateDestroyed)
			return;
	}

	GetOwner()->SetLocalPosition(
		static_cast<int>(GetOwner()->GetWorldPosition().x + (m_Direction * PushAmmount)),
		static_cast<int>(GetOwner()->GetWorldPosition().y)
	);
}

void dae::PushComponent::Update(const float)
{
	m_CollisionPoint.x = static_cast<int>(m_PushingPoint.x + GetOwner()->GetWorldPosition().x);
	m_CollisionPoint.y = static_cast<int>(m_PushingPoint.y + GetOwner()->GetWorldPosition().y);

	for (GameObject* Actor : m_OtherActors)
	{
		if (!Actor) continue;

		if (!bIsInstantlyPickup)
		{
			auto* anim = Actor->GetFirstComponentOfType<AnimControllerComponent>();
			if (!anim) continue;

			const auto* st = anim->GetCurrentState();
			if (st && st->GetStateName() == kStateDead) continue;

			glm::ivec2 actorSize = st
				? st->GetFlipBook()->GetUsedTexture()->GetTextureResolution()
				: glm::ivec2{ 0,0 };
			actorSize.x /= 4;

			const glm::ivec4 actorRect{
				static_cast<int>(Actor->GetWorldPosition().x),
				static_cast<int>(Actor->GetWorldPosition().y),
				actorSize.x,
				actorSize.y
			};

			if (dae::MathUtils::CheckPointInSquare(actorRect, m_CollisionPoint))
			{
				auto* ownerAnim = GetOwner()->GetFirstComponentOfType<AnimControllerComponent>();
				const bool ownerDestroyed =
					ownerAnim && ownerAnim->GetCurrentState() &&
					ownerAnim->GetCurrentState()->GetStateName() == kStateDestroyed;

				if (ownerDestroyed)
				{
					if (auto* score = Actor->GetFirstComponentOfType<ScoreComponent>())
					{
						score->IncreaseScore(500);
						GetOwner()->Destroy();
					}
				}
				else
				{
					Push(10.f);
				}
			}
		}
		else
		{
			const glm::ivec4 actorRect{
				static_cast<int>(Actor->GetWorldPosition().x),
				static_cast<int>(Actor->GetWorldPosition().y),
				20, 20
			};

			if (dae::MathUtils::CheckPointInSquare(actorRect, m_CollisionPoint))
			{
				if (auto* score = Actor->GetFirstComponentOfType<ScoreComponent>())
				{
					score->IncreaseScore(m_Score);
					GetOwner()->Destroy();
				}
			}
		}
	}
}

void dae::PushComponent::Render() const
{
	// debug point draw (optional)
	// SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 0, 0, 255);
	// SDL_RenderDrawPoint(Renderer::GetInstance().GetSDLRenderer(), m_CollisionPoint.x, m_CollisionPoint.y);
	// SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 0, 0, 255);
}
