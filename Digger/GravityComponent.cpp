#include "GravityComponent.h"
#include "MapComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "AnimControllerComponent.h"
#include "DamageComponent.h"
#include <cmath>
#include <string>

namespace
{
	constexpr int   kSamples = 20;
	constexpr float kEmptyRatio = 0.6f;
	constexpr int   kProbeOffsetY = 20;
	constexpr int   kDarkThreshold = 10;
	constexpr float kFallSpeed = 80.f;
	constexpr int   kScreenBottom = 200;
	constexpr int   kObjectHeight = 20;
	constexpr float kBreakFallDistance = 30.f;
	constexpr float kShakePixelOffset = 0.5f;

	constexpr const char* kStateIdle = "Idle";
	constexpr const char* kStateFalling = "Falling";
	constexpr const char* kStateDestroyed = "Destroyed";
}

dae::GravityComponent::GravityComponent(GameObject* Owner, class MapComponent* MapComp)
	: BaseComponent(Owner)
	, m_MapComponent(MapComp)
{
	AnimComponent = GetOwner()->GetFirstComponentOfType<AnimControllerComponent>();
}

void dae::GravityComponent::FixedUpdate(const float dt)
{
	if (!m_MapComponent) return;

	auto* owner = GetOwner();

	auto currentStateName = [&]() -> std::string {
		if (!AnimComponent) return kStateIdle;
		const auto* s = AnimComponent->GetCurrentState();
		return s ? s->GetStateName() : std::string{ kStateIdle };
		};

	auto setState = [&](const char* s)
		{
			if (!AnimComponent) return;
			const auto* cur = AnimComponent->GetCurrentState();
			if (cur && cur->GetStateName() == kStateDestroyed) return; // lock terminal state
			if (!cur || cur->GetStateName() != s)
				AnimComponent->ChangeState(s);
		};

	const std::string state = currentStateName();
	const auto wp = owner->GetWorldPosition();

	auto canFallHere = [&]() -> bool {
		int emptyCount = 0;
		dae::LockedTexture temp{};
		auto& R = Renderer::GetInstance();

		R.LockTexture(m_MapComponent->GetMapTexture(), temp);
		for (int i = 0; i < kSamples; ++i)
		{
			const int px = static_cast<int>(wp.x) + i;
			const int py = static_cast<int>(wp.y) + kProbeOffsetY;
			const SDL_Color c = R.FastReadPixel(temp, px, py);
			if (c.r < kDarkThreshold && c.g < kDarkThreshold && c.b < kDarkThreshold)
				++emptyCount;
		}
		R.UnlockTexture(temp);

		return emptyCount >= static_cast<int>(kSamples * kEmptyRatio);
		};

	const bool bCanFall = canFallHere();
	if (!bCanFall) shakeTimer = 0.0f;

	if (bCanFall)
	{
		if (state != kStateFalling && state != kStateDestroyed && shakeTimer < shakeDuration)
		{
			shakeTimer += dt;
			timeSinceLastShake += dt;
			if (timeSinceLastShake >= shakeInterval)
			{
				timeSinceLastShake = 0.0f;
				shakeDirection *= -1;
			}
			auto lp = owner->GetLocalTransform().m_position;
			lp.x += shakeDirection * kShakePixelOffset;
			owner->SetLocalPosition(lp);
			return;
		}

		if (state != kStateFalling)
		{
			m_StartFallingYPosition = owner->GetLocalTransform().m_position.y;
			setState(kStateFalling);
			shakeTimer = 0.0f;
			timeSinceLastShake = 0.0f;
		}

		auto newPos = wp;
		newPos.y += kFallSpeed * dt;

		const int floorY = kScreenBottom - kObjectHeight;
		if (newPos.y > floorY)
		{
			newPos.y = static_cast<float>(floorY);
			setState(kStateDestroyed);
		}

		owner->SetLocalPosition(static_cast<int>(newPos.x), static_cast<int>(newPos.y));
		return;
	}


	if (state == kStateFalling)
	{
		const float fallDistance =
			std::abs(owner->GetLocalTransform().m_position.y - m_StartFallingYPosition);

		if (fallDistance > kBreakFallDistance)
			setState(kStateDestroyed); 
		else
			setState(kStateIdle);
	}
}

void dae::GravityComponent::Render() const
{
}
