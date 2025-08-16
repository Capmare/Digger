#include "DamageComponent.h"
#include "Renderer.h"
#include "AnimControllerComponent.h"
#include "MathUtils.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include <string>

namespace
{
	constexpr const char* kStateFalling = "Falling";
	constexpr const char* kStateDestroyed = "Destroyed";
	constexpr const char* kStateDead = "Dead";
}

void dae::DamageComponent::Update(const float)
{
	const auto ownerPos = GetOwner()->GetWorldPosition();
	const glm::ivec4 dmgRectI4 =
	{
		static_cast<int>(ownerPos.x + m_DamageSquare.x + 3),
		static_cast<int>(ownerPos.y + m_DamageSquare.y + 3),
		m_DamageSquare.z - 3,
		m_DamageSquare.w - 3
	};
	SDL_Rect dmgRectSDL{ dmgRectI4.x, dmgRectI4.y, dmgRectI4.z, dmgRectI4.w };

	bShouldDamage = bDoesDamageWithoutCondition;
	if (!bDoesDamageWithoutCondition)
	{
		if (auto* ownerAnim = GetOwner()->GetFirstComponentOfType<AnimControllerComponent>())
		{
			const auto* st = ownerAnim->GetCurrentState();
			const bool isFalling = (st && st->GetStateName() == kStateFalling);
			const bool isDestroyed = (st && st->GetStateName() == kStateDestroyed);
			bShouldDamage = isFalling && !isDestroyed;
		}
		else
		{
			bShouldDamage = false;
		}
	}

	if (!bShouldDamage) return;

	for (GameObject* actor : m_OtherActors)
	{
		if (!actor) continue;

		auto* anim = actor->GetFirstComponentOfType<AnimControllerComponent>();
		if (!anim) continue;

		glm::ivec2 res = anim->GetCurrentState()->GetFlipBook()->GetUsedTexture()->GetTextureResolution();
		res.x /= 4;

		const glm::ivec2 actorPos{
			static_cast<int>(actor->GetWorldPosition().x),
			static_cast<int>(actor->GetWorldPosition().y)
		};

		SDL_Rect actorRect{
			actorPos.x + m_CollisionOffset,
			actorPos.y + m_CollisionOffset,
			res.x - 2 * m_CollisionOffset,
			res.y - 2 * m_CollisionOffset
		};

		if (SDL_HasIntersection(&dmgRectSDL, &actorRect))
		{
			if (auto* health = actor->GetFirstComponentOfType<HealthComponent>())
			{
				if (actor->m_Name != "Digger")
				{
					if (auto* score = actor->GetFirstComponentOfType<ScoreComponent>())
					{
						score->IncreaseScore(500);
						GetOwner()->Destroy();
					}
				}

				const auto* cur = anim->GetCurrentState();
				if (!cur || cur->GetStateName() != kStateDead)
				{
					health->DecreaseHealth(1);
					anim->GetCurrentState()->GetFlipBook()->ResetFlipBook();
					anim->ChangeState(kStateDead);
				}
			}
		}
	}
}

void dae::DamageComponent::Render() const
{
	//auto* renderer = Renderer::GetInstance().GetSDLRenderer();
	//
	//// Draw the damage rectangle
	//SDL_Rect dmgRect = {
	//	static_cast<int>(GetOwner()->GetWorldPosition().x + m_DamageSquare.x),
	//	static_cast<int>(GetOwner()->GetWorldPosition().y + m_DamageSquare.y),
	//	m_DamageSquare.z,
	//	m_DamageSquare.w
	//};
	//
	//SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	//SDL_RenderDrawRect(renderer, &dmgRect);
	//
	//// Draw each actor's collision bounds
	//for (GameObject* Actors : m_OtherActors)
	//{
	//	const AnimControllerComponent* AnimComponent = Actors->GetFirstComponentOfType<AnimControllerComponent>();
	//	if (AnimComponent)
	//	{
	//		glm::ivec2 Resolution = AnimComponent->GetCurrentState()->GetFlipBook()->GetUsedTexture()->GetTextureResolution();
	//		Resolution.x /= 4;
	//
	//		SDL_Rect actorRect = {
	//			static_cast<int>(Actors->GetWorldPosition().x + m_CollisionOffset),
	//			static_cast<int>(Actors->GetWorldPosition().y + m_CollisionOffset),
	//			Resolution.x - 2 * m_CollisionOffset,
	//			Resolution.y - 2 * m_CollisionOffset
	//		};
	//
	//		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	//		SDL_RenderDrawRect(renderer, &actorRect);
	//	}
	//}
	//
	//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

}
