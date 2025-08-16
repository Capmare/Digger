#include "SpawnerComponent.h"
#include "AnimControllerComponent.h"
#include "AIMovementComponent.h"
#include "DamageComponent.h"
#include "HealthComponent.h"
#include "CombatInformationComponent.h"
#include <cassert>

using namespace dae;

void SpawnerComponent::Update(float dt)
{
	if (m_SpawnedMonsters >= m_MonstersToSpawn) return;

	m_TimeSinceLastSpawn += dt;
	if (m_TimeSinceLastSpawn >= m_NextSpawnDelay)
	{
		SpawnMonster();
		++m_SpawnedMonsters;

		m_TimeSinceLastSpawn = 0.f;
		m_NextSpawnDelay = ComputeNextDelay();
	}
}

void SpawnerComponent::SpawnMonster()
{
	assert(m_Scene && "Scene is null");
	assert(m_AIGridComp && m_Digger && m_MapComp && "AI movement dependency is null");

	GameObject* Monster = m_Scene->CreateNewGameObject();
	assert(Monster && "Failed to create monster GameObject");
	Monster->m_Name = "Monster";

	const int index = static_cast<int>(m_MonstersSpawned.size());
	const glm::ivec2 spawnPos = m_SpawnOrigin + glm::ivec2{ m_SpawnXSpacing * index, 0 };
	Monster->SetLocalPosition(spawnPos.x, spawnPos.y);

	Monster->CreateComponent<HealthComponent>(1);

	AnimControllerComponent* MonsterAnimController = Monster->CreateComponent<AnimControllerComponent>();
	assert(MonsterAnimController && "Failed to create AnimControllerComponent");

	FlipBookConfig NobbinFlipBookConfig{ "Nobbin.png", 3, 20, 20, 0.2f, true, 1 };
	FlipBookConfig HobbinFlipBookConfig{ "Hobbin.png", 3, 20, 20, 0.2f, true, 1 };

	MonsterAnimController->CreateState<NobbinState>(NobbinFlipBookConfig);
	MonsterAnimController->CreateState<HobbinState>(HobbinFlipBookConfig);
	MonsterAnimController->ChangeState("Nobbin");

	// AI movement
	auto* ai = Monster->CreateComponent<AIMovementComponent>(m_AIGridComp, MonsterAnimController, m_Digger, m_MapComp);
	assert(ai && "Failed to create AIMovementComponent");

	DamageComponent* dmgComp = Monster->CreateComponent<DamageComponent>(glm::ivec4{ 0, 0, 20, 20 });
	assert(dmgComp && "Failed to create DamageComponent");
	dmgComp->bDoesDamageWithoutCondition = true;
	dmgComp->AddNewDamageRecevingActor(m_Digger);

	for (GameObject* actor : m_DamagingMonsterObject)
	{
		if (!actor) continue;
		if (auto* otherDmg = actor->GetFirstComponentOfType<DamageComponent>())
		{
			otherDmg->AddNewDamageRecevingActor(Monster);
		}
	}

	m_MonstersSpawned.emplace_back(Monster);
}

void SpawnerComponent::ResetSpawns()
{
	for (GameObject* m : m_MonstersSpawned)
	{
		if (m) m->Destroy();
	}
	m_MonstersSpawned.clear();

	m_SpawnedMonsters = 0;
	m_TimeSinceLastSpawn = 0.f;
	m_NextSpawnDelay = ComputeNextDelay();

	m_SpawnLocation = m_SpawnOrigin;
}
