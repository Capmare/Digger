#include "SpawnerComponent.h"
#include "AnimControllerComponent.h"
#include "AIMovementComponent.h"
#include "DamageComponent.h"
#include "HealthComponent.h"
#include "CombatInformationComponent.h"

void dae::SpawnerComponent::SpawnMonster()
{
	GameObject* Monster = m_Scene->CreateNewGameObject();
	assert(Monster && "Failed to create monster GameObject");
	Monster->m_Name = "Monster";
	Monster->SetLocalPosition(m_SpawnLocation.x, m_SpawnLocation.y);

	Monster->CreateComponent<HealthComponent>(1);
	// Create AnimController and setup states
	AnimControllerComponent* MonsterAnimController = Monster->CreateComponent<AnimControllerComponent>();
	assert(MonsterAnimController && "Failed to create AnimControllerComponent");

	FlipBookConfig NobbinFlipBookConfig{ "Nobbin.png", 3, 20, 20, 0.2f, true, 1 };
	FlipBookConfig HobbinFlipBookConfig{ "Hobbin.png", 3, 20, 20, 0.2f, true, 1 };

	MonsterAnimController->CreateState<NobbinState>(NobbinFlipBookConfig);
	MonsterAnimController->CreateState<HobbinState>(HobbinFlipBookConfig);
	MonsterAnimController->ChangeState("Nobbin");

	// Create AI Movement component
	assert(m_AIGridComp && m_Digger && m_MapComp && "AI movement dependency is null");
	auto* ai = Monster->CreateComponent<AIMovementComponent>(m_AIGridComp, MonsterAnimController, m_Digger, m_MapComp);
	assert(ai && "Failed to create AIMovementComponent");

	// Create Damage component
	DamageComponent* dmgComp = Monster->CreateComponent<DamageComponent>(glm::ivec4{ 0, 0, 20, 20 });
	assert(dmgComp && "Failed to create DamageComponent");
	dmgComp->bDoesDamageWithoutCondition = true;
	dmgComp->AddNewDamageRecevingActor(m_Digger);

	m_MonstersSpawned.emplace_back(Monster);

	for (GameObject* Actor : m_DamagingMonsterObject)
	{
		DamageComponent* OtherDmgComp = Actor->GetFirstComponentOfType<DamageComponent>();
		OtherDmgComp->AddNewDamageRecevingActor(Monster);
	}

}

void dae::SpawnerComponent::ResetSpawns()
{

	m_SpawnLocation = { 270,5 };
	for (size_t idx{ 0 }; idx < m_MonstersSpawned.size(); ++idx)
	{
		AIMovementComponent* AIMovComp = m_MonstersSpawned[idx]->GetFirstComponentOfType<AIMovementComponent>();
		if (AIMovComp)
		{
			AIMovComp->bStopPathFinding = true;
			m_MonstersSpawned[idx]->SetLocalPosition(m_SpawnLocation.x, m_SpawnLocation.y);
			AIMovComp->RecreatePath(static_cast<int>(idx + 1));
			AIMovComp->bStopPathFinding = false;
			m_SpawnLocation += glm::ivec2{ 150 * (idx + 1),5 };
		}
		
	}

}


