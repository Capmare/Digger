#pragma once
#include "BaseComponent.h"
#include "GameObject.h"


namespace dae
{
	class DamageComponent : public BaseComponent
	{
	public:
		DamageComponent(GameObject* Owner, glm::ivec4 DamageZone)
			: BaseComponent(Owner), m_DamageSquare(DamageZone) {
		}

		void Update(float dt) override;
		void Render() const override;

		void AddNewDamageRecevingActor(GameObject* NewActor) { m_OtherActors.emplace_back(NewActor); }

		void SetDynamicActors(const std::vector<GameObject*>* actors) { m_DynamicActors = actors; }

		std::vector<GameObject*> GetDamageReceivingActors() const { return m_OtherActors; }

		GameObject* m_Player{};
		bool bShouldDamage{};
		bool bDoesDamageWithoutCondition{ false };

	private:
		const int m_CollisionOffset{ 0 };
		std::vector<GameObject*> m_OtherActors{};
		const std::vector<GameObject*>* m_DynamicActors{ nullptr }; 
		glm::ivec4 m_DamageSquare{};
	};



}
