#pragma once
#include "BaseComponent.h"
#include "GameObject.h"


namespace dae
{
	class DamageComponent
		: public BaseComponent
	{
	public:
		DamageComponent(GameObject* Owner, glm::ivec4 DamageZone) : BaseComponent(Owner), m_DamageSquare(DamageZone) {};
		virtual ~DamageComponent() = default;
		
		DamageComponent(const DamageComponent&) = delete;
		DamageComponent(DamageComponent&&) noexcept = delete;
		DamageComponent& operator=(const DamageComponent&) = delete;
		DamageComponent& operator=(DamageComponent&&) noexcept = delete;

		

		void Update(const float deltaTime) override;
		void Render() const override;

		void AddNewDamageRecevingActor(GameObject* NewActor) { m_OtherActors.emplace_back(NewActor); }

		bool bShouldDamage{};

	private:
		constexpr bool CheckPointInSquare(const glm::ivec4& rect, const glm::ivec2& point) {
			return point.x >= rect.x &&
				point.y >= rect.y &&
				point.x < rect.x + rect.z &&
				point.y < rect.y + rect.w;
		}

		std::vector<GameObject*> m_OtherActors{};
		glm::ivec4 m_DamageSquare{};

	};

}
