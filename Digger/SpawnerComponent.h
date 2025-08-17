#pragma once
#include <BaseComponent.h>
#include "Scene.h"
#include "AIGridComponent.h"
#include "MapComponent.h"
#include <random>

namespace dae
{
	class SpawnerComponent : public BaseComponent
	{
	public:
		SpawnerComponent(GameObject* Owner,
			Scene* scene,
			AIGridComponent* AIGridComp,
			MapComponent* MapComp,
			GameObject* Digger,
			float spawnInterval = 5.f,
			int monstersToSpawn = 5,
			const glm::ivec2 spawnLocation = { 0, 0 },
			const std::vector<GameObject*>& damagingMonsterActors = {})
			: BaseComponent(Owner)
			, m_Scene(scene)
			, m_AIGridComp(AIGridComp)
			, m_MapComp(MapComp)
			, m_Digger(Digger)
			, m_SpawnOrigin(spawnLocation)
			, m_SpawnLocation(spawnLocation)
			, m_MonstersToSpawn(monstersToSpawn)
			, m_SpawnInterval(spawnInterval)
			, m_DamagingMonsterObject(damagingMonsterActors)
		{
			// start a staggered schedule; nothing spawns immediately
			m_TimeSinceLastSpawn = 0.f;
			m_NextSpawnDelay = ComputeNextDelay();
		}

		virtual ~SpawnerComponent() = default;

		SpawnerComponent(const SpawnerComponent&) = delete;
		SpawnerComponent(SpawnerComponent&&) noexcept = delete;
		SpawnerComponent& operator=(const SpawnerComponent&) = delete;
		SpawnerComponent& operator=(SpawnerComponent&&) noexcept = delete;

		void Update(float dt) override;
		void SpawnMonster();
		void ResetSpawns();

		std::vector<GameObject*> m_MonstersSpawned{};

	private:
		float ComputeNextDelay() const
		{
			// jitter spawns so they don't happen at the exact same cadence
			const float minDelay = (std::max)(0.1f, m_SpawnInterval * (1.f - m_SpawnJitter));
			const float maxDelay = m_SpawnInterval * (1.f + m_SpawnJitter);
			static thread_local std::mt19937 rng{ std::random_device{}() };
			std::uniform_real_distribution<float> dist(minDelay, maxDelay);
			return dist(rng);
		}


		// Dependencies
		std::vector<GameObject*> m_DamagingMonsterObject{};
		Scene* m_Scene{};
		AIGridComponent* m_AIGridComp{};
		MapComponent* m_MapComp{};
		GameObject* m_Digger{};

		// Spawn config/state
		glm::ivec2 m_SpawnOrigin{};
		glm::ivec2 m_SpawnLocation{};
		int        m_MonstersToSpawn{};
		int        m_SpawnedMonsters{ 0 };

		float m_SpawnInterval = 5.f;     
		float m_SpawnJitter = 0.4f;   
		float m_TimeSinceLastSpawn = 0.f;
		float m_NextSpawnDelay = 0.f;

		// Placement
		int   m_SpawnXSpacing = 150;     
	};
}
