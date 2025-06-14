#pragma once
#include <BaseComponent.h>
#include "Scene.h"
#include "AIGridComponent.h"
#include "MapComponent.h"

namespace dae
{
	class SpawnerComponent :
		public BaseComponent
	{
	public:
		SpawnerComponent(GameObject* Owner, Scene* scene, AIGridComponent* AIGridComp, MapComponent* MapComp, GameObject* Digger, float spawnInterval = 5.f, int m_MonstersToSpawn = 5, const glm::ivec2 SpawnLocation = { 0,0 }, const std::vector<GameObject*>& DamaginMonsterActor = {})
			: BaseComponent(Owner), m_Scene(scene), m_AIGridComp(AIGridComp), m_MapComp(MapComp), m_Digger(Digger), m_SpawnInterval(spawnInterval), m_MonstersToSpawn(m_MonstersToSpawn), m_SpawnLocation(SpawnLocation), m_DamagingMonsterObject(DamaginMonsterActor)
		{

			for (size_t idx{1}; idx < m_MonstersToSpawn+1; ++idx)
			{
				SpawnMonster();
				m_SpawnLocation += glm::ivec2{ 150*idx,0 };
			}
			


		};
		virtual ~SpawnerComponent() = default;
		
		SpawnerComponent(const SpawnerComponent&) = delete;
		SpawnerComponent(SpawnerComponent&&) noexcept = delete;
		SpawnerComponent& operator=(const SpawnerComponent&) = delete;
		SpawnerComponent& operator=(SpawnerComponent&&) noexcept = delete;


		void SpawnMonster();

		void ResetSpawns();

		std::vector<GameObject*> m_MonstersSpawned{};

	private:
		std::vector<GameObject*> m_DamagingMonsterObject{};
		Scene* m_Scene{};
		AIGridComponent* m_AIGridComp{};
		MapComponent* m_MapComp{};
		GameObject* m_Digger{};

		glm::ivec2 m_SpawnLocation{};
		int m_MonstersToSpawn{};
		int m_SpawnedMonsters{};
		float m_SpawnInterval = 5.f;
		float m_TimeSinceLastSpawn = 0.f;
	};
}

