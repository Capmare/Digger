#pragma once
#include <BaseComponent.h>
#include <vector>
#include <unordered_set>
#include "SDL_pixels.h"
#include "glm.hpp"

namespace dae { struct LockedTexture; }

namespace dae
{
	// Node struct for pathfinding data
	struct Node
	{
		glm::ivec2 position;
		int gCost = INT_MAX; 
		int hCost = 0;       
		glm::ivec2 parent = glm::ivec2(-1, -1);

		int fCost() const { return gCost + hCost; }
	};

	struct IVec2Hash {
		size_t operator()(const glm::ivec2& v) const;
	};

	struct ComparePair
	{
		bool operator()(const std::pair<int, glm::ivec2>& a, const std::pair<int, glm::ivec2>& b) const
		{
			return a.first > b.first; 
		}
	};

	class AIGridComponent :
		public BaseComponent
	{
	public:
		AIGridComponent(GameObject* Owner, const glm::ivec2 ScreenResolution, const glm::ivec2 TileSize,class MapComponent* MapComp) 
			: BaseComponent(Owner), m_ScreenResolution(ScreenResolution), m_TileSize(TileSize), m_MapComponent(MapComp) {};
		virtual ~AIGridComponent() = default;
		
		AIGridComponent(const AIGridComponent&) = delete;
		AIGridComponent(AIGridComponent&&) noexcept = delete;
		AIGridComponent& operator=(const AIGridComponent&) = delete;
		AIGridComponent& operator=(AIGridComponent&&) noexcept = delete;


		void Update(const float deltaTime) override;

		void Render() const override;


		std::vector<glm::ivec2> FindPathAllMap(const glm::ivec2& start, const glm::ivec2& goal) const;
		std::vector<glm::ivec2> FindPathFreeTilesFromPixels(const glm::ivec2& pixelStart, const glm::ivec2& pixelGoal) const;
		std::vector<glm::ivec2> FindPathAllMapFromPixels(const glm::ivec2& pixelStart, const glm::ivec2& pixelGoal) const;
		std::vector<glm::ivec2> FindPathFreeTiles(const glm::ivec2& start, const glm::ivec2& goal) const;
		std::vector<glm::ivec2> FindPathToRandomFreeTile(const glm::ivec2& start, int maxAttempts) const;

		glm::ivec2 TileToPixels(const glm::ivec2& tileCoord) const;
		void RebuildFreeTiles();
	private:

		bool CheckIfTileEmpty(int Tile, const LockedTexture& locked, SDL_PixelFormat* fmt) const;

		int Heuristic(const glm::ivec2& a, const glm::ivec2& b) const;

		std::vector<glm::ivec2> GetNeighbors(const glm::ivec2& pos) const;

		class MapComponent* m_MapComponent{};

		std::unordered_set<glm::ivec2, IVec2Hash> m_FreeTiles;

		const glm::ivec2 m_ScreenResolution{};
		const glm::ivec2 m_TileSize{};
		float m_PassedTime{};
		float m_MaxWaitingTime{ .5 };

		mutable bool bCanUpdatePath{};
	

	};
}

