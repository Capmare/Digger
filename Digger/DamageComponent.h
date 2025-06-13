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
		
		const int m_CollisionOffset{0};
		std::vector<GameObject*> m_OtherActors{};
		glm::ivec4 m_DamageSquare{};

	};

}
