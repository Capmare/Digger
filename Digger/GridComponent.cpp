#include "GridComponent.h"
#include "TextureComponent.h"
#include <iostream>

void dae::GridComponent::UpdateTileType(const glm::ivec2 Tile, const TileType NewType)
{
	int TileIdx{ Tile.y * m_Width + Tile.x };
	m_Tiles.at(TileIdx) = NewType;
	std::cout << TileIdx << std::endl;
}

void dae::GridComponent::Update(const float deltaTime)
{
	for (size_t idx{}; idx < m_TileTextures.size(); ++idx)
	{
		if (!m_TileTextures[idx]) return;
		m_TileTextures[idx]->Update(deltaTime);
	}
}

void dae::GridComponent::Render() const
{
	for (size_t idx{}; idx < m_TileTextures.size(); ++idx)
	{
		if (!m_TileTextures[idx]) return;
		m_TileTextures[idx]->Render();
	}
}

void dae::GridComponent::CreateTileSize(int , TileType )
{

}
