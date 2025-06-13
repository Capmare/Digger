#pragma once
#include <BaseComponent.h>

namespace dae
{

	class GravityComponent :
		public BaseComponent
	{
	public:
		GravityComponent(GameObject* Owner, class MapComponent* GridComp);
		virtual ~GravityComponent() = default;
		
		GravityComponent(const GravityComponent&) = delete;
		GravityComponent(GravityComponent&&) noexcept = delete;
		GravityComponent& operator=(const GravityComponent&) = delete;
		GravityComponent& operator=(GravityComponent&&) noexcept = delete;

		void FixedUpdate(const float fixedDeltaTime) override;


		void Render() const override;

		bool GetIsFalling() const { return bIsFalling; }
		bool GetIsBroken() const { return bIsBroken; }

	private:
		class MapComponent* m_MapComponent{};
		class AnimControllerComponent* AnimComponent{};

		float shakeTimer = 0.0f;
		float shakeDuration = 1.0f;
		float shakeInterval = 0.05f; 
		float timeSinceLastShake = 0.0f;
		int shakeDirection = 1;
		float m_StartFallingYPosition{};

		bool bIsFalling{ false };
		bool bIsBroken{ false };
	};
}

