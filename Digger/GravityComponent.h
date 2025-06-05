#pragma once
#include <BaseComponent.h>

namespace dae
{

	class GravityComponent :
		public BaseComponent
	{
	public:
		GravityComponent(GameObject* Owner, class GridComponent* GridComp);
		virtual ~GravityComponent() = default;
		
		GravityComponent(const GravityComponent&) = delete;
		GravityComponent(GravityComponent&&) noexcept = delete;
		GravityComponent& operator=(const GravityComponent&) = delete;
		GravityComponent& operator=(GravityComponent&&) noexcept = delete;

		void FixedUpdate(const float fixedDeltaTime) override;


		void Render() const override;

	private:
		class GridComponent* m_GridComponent{};
		class AnimControllerComponent* AnimComponent{};

		bool bIsFalling{ false };
	};
}

