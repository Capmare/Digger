#include "AIGridComponent.h"
#include "Renderer.h"
#include "MathUtils.h"
#include <unordered_map>
#include <queue>
#include <unordered_set>
#include "MapComponent.h"
#include <random>
#include <thread>
#include <algorithm>

void dae::AIGridComponent::Update(const float deltaTime)
{
	m_PassedTime += deltaTime;
	if (m_PassedTime > m_MaxWaitingTime)
	{
		bCanUpdatePath = true;
		m_PassedTime = 0;
		std::jthread(&dae::AIGridComponent::RebuildFreeTiles, this).join();
	}
}

void dae::AIGridComponent::Render() const
{
	//auto Renderer = Renderer::GetInstance().GetSDLRenderer();
	//SDL_SetRenderDrawColor(Renderer, 150, 150, 150, 255); // R, G, B, A
	//
	//const int gridWidth = m_ScreenResolution.x;
	//const int gridHeight = m_ScreenResolution.y;
	//const int tileWidth = m_TileSize.x;
	//const int tileHeight = m_TileSize.y;
	//
	//for (int x = 0; x <= gridWidth; x += tileWidth)
	//{
	//	SDL_RenderDrawLine(Renderer, x, 0, x, gridHeight);
	//}
	//
	//for (int y = 0; y <= gridHeight; y += tileHeight)
	//{
	//	SDL_RenderDrawLine(Renderer, 0, y, gridWidth, y);
	//}
	//
	//auto path = FindPathAllMap({ 0,11 }, { 0,12 });
	//
	//SDL_SetRenderDrawColor(Renderer, 255, 0, 0, 255);
	//for (const auto& p : path)
	//{
	//	SDL_Rect rect{};
	//	rect.x = p.x * m_TileSize.x;
	//	rect.y = p.y * m_TileSize.y;
	//	rect.w = m_TileSize.x;
	//	rect.h = m_TileSize.y;
	//	SDL_RenderFillRect(Renderer, &rect);
	//}
	//
	//auto freePath = FindPathFreeTiles({ 0,11 }, { 0,12 });
	//SDL_SetRenderDrawColor(Renderer, 0, 255, 0, 100);
	//for (const auto& tile : freePath)
	//{
	//	SDL_Rect rect{};
	//	rect.x = tile.x * m_TileSize.x;
	//	rect.y = tile.y * m_TileSize.y;
	//	rect.w = m_TileSize.x;
	//	rect.h = m_TileSize.y;
	//	SDL_RenderFillRect(Renderer, &rect);
	//}
	
}

// A* star is good here since it does not care about dirt
std::vector<glm::ivec2> dae::AIGridComponent::FindPathAllMap(const glm::ivec2& start, const glm::ivec2& goal) const
{

	using namespace glm;

	int numTilesX = (m_TileSize.x > 0) ? m_ScreenResolution.x / m_TileSize.x : 1;
	int numTilesY = (m_TileSize.y > 0) ? m_ScreenResolution.y / m_TileSize.y : 1;

	auto isValidGridPosition = [&](const ivec2& pos) {
		return pos.x >= 0 && pos.y >= 0 &&
			pos.x < numTilesX && pos.y < numTilesY;
		};

	if (!isValidGridPosition(start) || !isValidGridPosition(goal))
	{
		std::cerr << "FindPath Error: Start or goal position is outside grid bounds.\n";
		return {};
	}

	std::unordered_map<ivec2, Node, IVec2Hash> allNodes;
	std::priority_queue<
		std::pair<int, ivec2>,
		std::vector<std::pair<int, ivec2>>,
		ComparePair> openSet;

	std::unordered_set<ivec2, IVec2Hash> closedSet;

	Node startNode{ start };
	startNode.gCost = 0;
	startNode.hCost = Heuristic(start, goal);
	allNodes[start] = startNode;
	openSet.emplace(startNode.fCost(), start);

	while (!openSet.empty())
	{
		ivec2 current = openSet.top().second;
		openSet.pop();

		if (closedSet.contains(current))
			continue;

		if (current == goal)
		{
			std::vector<ivec2> path;
			ivec2 step = goal;
			while (step != start)
			{
				auto it = allNodes.find(step);
				if (it == allNodes.end()) {
					std::cerr << "FindPath Error: Missing parent node during path reconstruction.\n";
					return {};
				}
				path.push_back(step);
				step = it->second.parent;
			}
			path.push_back(start);
			std::reverse(path.begin(), path.end());
			return path;
		}

		closedSet.insert(current);

		for (const auto& neighbor : GetNeighbors(current))
		{
			if (!isValidGridPosition(neighbor))
				continue;

			if (closedSet.contains(neighbor))
				continue;

			int tentativeG = allNodes[current].gCost + 1;

			auto it = allNodes.find(neighbor);
			if (it == allNodes.end() || tentativeG < it->second.gCost)
			{
				Node neighborNode;
				neighborNode.position = neighbor;
				neighborNode.gCost = tentativeG;
				neighborNode.hCost = Heuristic(neighbor, goal);
				neighborNode.parent = current;
				allNodes[neighbor] = neighborNode;

				openSet.emplace(neighborNode.fCost(), neighbor);
			}
		}
	}

	return {};
}

std::vector<glm::ivec2> dae::AIGridComponent::FindPathFreeTilesFromPixels(const glm::ivec2& pixelStart, const glm::ivec2& pixelGoal) const
{
	glm::ivec2 tileStart = glm::ivec2(pixelStart.x / m_TileSize.x, pixelStart.y / m_TileSize.y);
	glm::ivec2 tileGoal = glm::ivec2(pixelGoal.x / m_TileSize.x, pixelGoal.y / m_TileSize.y);
	std::vector<glm::ivec2> Path{};
	for (auto& tile : FindPathFreeTiles(tileStart, tileGoal))
	{
		Path.emplace_back(TileToPixels(tile));
	}

	return Path;
}

std::vector<glm::ivec2> dae::AIGridComponent::FindPathAllMapFromPixels(const glm::ivec2& pixelStart, const glm::ivec2& pixelGoal) const
{
	glm::ivec2 tileStart = glm::ivec2(pixelStart.x / m_TileSize.x, pixelStart.y / m_TileSize.y);
	glm::ivec2 tileGoal = glm::ivec2(pixelGoal.x / m_TileSize.x, pixelGoal.y / m_TileSize.y);

	std::vector<glm::ivec2> Path{};
	for (auto& tile : FindPathAllMap(tileStart, tileGoal))
	{
		Path.emplace_back(TileToPixels(tile));
	}

	return Path;
}

// use BFS for nobbin instead of A* because AStar fails when getting stuck
std::vector<glm::ivec2> dae::AIGridComponent::FindPathFreeTiles(const glm::ivec2& start, const glm::ivec2& goal) const
{

	if (!m_FreeTiles.contains(start) || !m_FreeTiles.contains(goal))
		return {};

	std::unordered_map<glm::ivec2, glm::ivec2, IVec2Hash> cameFrom;
	std::queue<glm::ivec2> frontier;
	std::unordered_set<glm::ivec2, IVec2Hash> visited;

	frontier.push(start);
	visited.insert(start);

	while (!frontier.empty())
	{
		glm::ivec2 current = frontier.front();
		frontier.pop();

		if (current == goal)
		{
			// Reconstruct path
			std::vector<glm::ivec2> path;
			glm::ivec2 step = goal;
			while (step != start)
			{
				path.push_back(step);
				step = cameFrom[step];
			}
			path.push_back(start);
			std::reverse(path.begin(), path.end());
			return path;
		}

		for (const auto& neighbor : GetNeighbors(current))
		{
			if (!m_FreeTiles.contains(neighbor) || visited.contains(neighbor))
				continue;

			frontier.push(neighbor);
			visited.insert(neighbor);
			cameFrom[neighbor] = current;
		}
	}

	// No path found
	return {};
}


std::vector<glm::ivec2> dae::AIGridComponent::FindPathToRandomFreeTile(const glm::ivec2& start, int maxAttempts) const
{
	
	if (m_FreeTiles.empty())
		return {};

	std::vector<glm::ivec2> freeTiles(m_FreeTiles.begin(), m_FreeTiles.end());

	for (int attempt = 0; attempt < maxAttempts; ++attempt)
	{
		int index = rand() % freeTiles.size();
		const glm::ivec2& randomGoal = freeTiles[index];

		if (randomGoal == start)
			continue;

		auto path = FindPathFreeTilesFromPixels(start, randomGoal);
		if (!path.empty())
			return path;
	}

	return {}; 
}

glm::ivec2 dae::AIGridComponent::TileToPixels(const glm::ivec2& tileCoord) const
{
	return {tileCoord.x * m_TileSize.x, tileCoord.y * m_TileSize.y};
}

void dae::AIGridComponent::RebuildFreeTiles()
{
	m_FreeTiles.clear();

	int numTilesX = (m_TileSize.x > 0) ? m_ScreenResolution.x / m_TileSize.x : 1;
	int numTilesY = (m_TileSize.y > 0) ? m_ScreenResolution.y / m_TileSize.y : 1;

	LockedTexture locked;
	if (!Renderer::GetInstance().LockTexture(m_MapComponent->GetMapTexture(), locked))
		return;

	SDL_PixelFormat* fmt = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);

	for (int y = 0; y < numTilesY; ++y)
	{
		for (int x = 0; x < numTilesX; ++x)
		{
			int tileIndex = x + y * numTilesX;
			if (CheckIfTileEmpty(tileIndex, locked, fmt))
			{
				m_FreeTiles.insert({ x, y });
			}
		}
	}

	SDL_FreeFormat(fmt);
	Renderer::GetInstance().UnlockTexture(locked);
}




bool dae::AIGridComponent::CheckIfTileEmpty(int Tile, const LockedTexture& locked, SDL_PixelFormat* fmt) const
{

	int numTilesX = (m_TileSize.x > 0) ? m_ScreenResolution.x / m_TileSize.x : 1;
	int numTilesY = (m_TileSize.y > 0) ? m_ScreenResolution.y / m_TileSize.y : 1;

	if (Tile < 0 || Tile >= (numTilesX * numTilesY))
	{
		std::cerr << "Invalid tile index " << Tile
			<< ". Grid has " << (numTilesX * numTilesY) << " tiles." << std::endl;
		return false;
	}

	int tileCol = Tile % numTilesX;
	int tileRow = Tile / numTilesX;

	const int tileLeft = tileCol * m_TileSize.x;
	const int tileTop = tileRow * m_TileSize.y;
	const int tileRight = tileLeft + m_TileSize.x;
	const int tileBottom = tileTop + m_TileSize.y;

	const int stepX{ std::max(1, m_TileSize.x / 4) };
	const int stepY{std::max(1, m_TileSize.y / 4)};

	int emptyCount = 0;
	int totalChecks = 0;

	const Uint32* pixelData = static_cast<const Uint32*>(locked.pixels);

	for (int y = tileTop; y < tileBottom; y += stepY)
	{
		for (int x = tileLeft; x < tileRight; x += stepX)
		{
			int clampedX = glm::clamp(x, 0, locked.width - 1);
			int clampedY = glm::clamp(y, 0, locked.height - 1);

			int index = clampedY * (locked.pitch / sizeof(Uint32)) + clampedX;
			Uint32 pixel = pixelData[index];

			SDL_Color color;
			SDL_GetRGBA(pixel, fmt, &color.r, &color.g, &color.b, &color.a);

			++totalChecks;
			if (color.r < 10 && color.g < 10 && color.b < 10)
			{
				++emptyCount;
			}
		}
	}

	return (emptyCount >= totalChecks * 0.9);


}

int dae::AIGridComponent::Heuristic(const glm::ivec2& a, const glm::ivec2& b) const
{
	return abs(a.x - b.x) + abs(a.y - b.y); // using Manhattan
}

std::vector<glm::ivec2> dae::AIGridComponent::GetNeighbors(const glm::ivec2& pos) const
{
	return {
		{pos.x + 1, pos.y},
		{pos.x - 1, pos.y},
		{pos.x, pos.y + 1},
		{pos.x, pos.y - 1}
	};
}


size_t dae::IVec2Hash::operator()(const glm::ivec2& v) const
{
	return std::hash<int>()(v.x) ^ (std::hash<int>()(v.y) << 1);
}