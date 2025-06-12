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

	private:
		class MapComponent* m_GridComponent{};
		class AnimControllerComponent* AnimComponent{};

		bool bIsFalling{ false };
		bool bIsBroken{ false };
		float m_StartFallingYPosition{};
	};
}

