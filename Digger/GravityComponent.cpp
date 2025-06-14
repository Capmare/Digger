#include "GravityComponent.h"
#include "MapComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "AnimControllerComponent.h"
#include "DamageComponent.h"

dae::GravityComponent::GravityComponent(GameObject* Owner, class MapComponent* MapComp) : BaseComponent(Owner), m_MapComponent(MapComp)
{
	AnimComponent = GetOwner()->GetFirstComponentOfType<AnimControllerComponent>();
}

void dae::GravityComponent::FixedUpdate(const float fixedDeltaTime)
{
	if (!m_MapComponent) return;

	int emptyCount = 0;
	const int totalChecks = 20;

	for (int idx = 0; idx < totalChecks; ++idx)
	{
		SDL_Color color = Renderer::GetInstance().ReadPixelColor(
			m_MapComponent->GetMapTexture(),
			static_cast<int>(GetOwner()->GetWorldPosition().x + idx),
			static_cast<int>(GetOwner()->GetWorldPosition().y + 20)
		);

		if (color.r < 10 && color.g < 10 && color.b < 10)
			++emptyCount;
	}

	bool bCanFall = (emptyCount >= totalChecks * 0.6f);

	if (!bCanFall)
	{
		shakeTimer = 0.0f; // prevent shaking on solid ground
	}

	if (bCanFall)
	{
		if (!bIsFalling && !bIsBroken && shakeTimer < shakeDuration)
		{
			shakeTimer += fixedDeltaTime;
			timeSinceLastShake += fixedDeltaTime;

			if (timeSinceLastShake >= shakeInterval)
			{
				timeSinceLastShake = 0.0f;
				shakeDirection *= -1; // Flip shake direction
			}

			auto pos = GetOwner()->GetLocalTransform().m_position;
			pos.x += shakeDirection * 0.5f;
			GetOwner()->SetLocalPosition(pos);

			return;
		}

		if (!bIsFalling)
		{
			m_StartFallingYPosition = GetOwner()->GetLocalTransform().m_position.y;
			if (!bIsBroken && AnimComponent)
				AnimComponent->ChangeState("Falling");

			bIsFalling = true;
			shakeTimer = 0.0f;
			timeSinceLastShake = 0.0f;
		}

		// Apply fall
		auto pos = GetOwner()->GetWorldPosition();
		pos.y += 80.0f * fixedDeltaTime;

		const int screenBottom = 200;
		const int objectHeight = 20;
		if (pos.y > screenBottom - objectHeight)
		{
			pos.y = screenBottom - objectHeight;

			bIsFalling = false;

			if (!bIsBroken && AnimComponent)
				AnimComponent->ChangeState("Destroyed");

			bIsBroken = true;
		}

		GetOwner()->SetLocalPosition(static_cast<int>(pos.x), static_cast<int>(pos.y));
		return;
	}
	else
	{
		if (bIsBroken) return;

		if (bIsFalling)
		{
			if (abs(GetOwner()->GetLocalTransform().m_position.y - m_StartFallingYPosition) > 30)
			{
				if (AnimComponent)
					AnimComponent->ChangeState("Destroyed");

				bIsBroken = true;
			}
			else
			{
				if (AnimComponent)
					AnimComponent->ChangeState("Idle");
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
