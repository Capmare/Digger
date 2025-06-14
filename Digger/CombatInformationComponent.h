#pragma once
#include <BaseComponent.h>
#include <vector>
#include "SpawnerComponent.h"

namespace dae
{
	class CombatInformationComponent :
		public BaseComponent
	{
	public:
		CombatInformationComponent(GameObject* Owner, SpawnerComponent* Spawner) : BaseComponent(Owner), m_Spawner(Spawner) {};
		virtual ~CombatInformationComponent() = default;
		
		CombatInformationComponent(const CombatInformationComponent&) = delete;
		CombatInformationComponent(CombatInformationComponent&&) noexcept = delete;
		CombatInformationComponent& operator=(const CombatInformationComponent&) = delete;
		CombatInformationComponent& operator=(CombatInformationComponent&&) noexcept = delete;

		SpawnerComponent* m_Spawner{};
	};
}

