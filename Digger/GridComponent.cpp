#include "GridComponent.h"
#include "TextureComponent.h"
#include <iostream>

void dae::GridComponent::UpdateTileType(const glm::ivec2 Tile, const TileType NewType)
{
	int TileIdx{ Tile.y * m_Width + Tile.x };
	m_Tiles[TileIdx] = NewType;
	switch (NewType)
	{
	default:
		break;
	case TileType::Dirt:
		m_TileTextures[TileIdx]->SetTexture("cback1.png");
		break;
	case TileType::Tunnel:
		m_TileTextures[TileIdx]->SetTexture("Black.png");
		break;
	}

	
}

void dae::GridComponent::Update(const float deltaTime)
{
	for (size_t idx{}; idx < m_TileTextures.size(); ++idx)
	{
		if (!m_TileTextures[idx]) return;
		m_TileTextures[idx]->Update(deltaTime);
	}
}


glm::ivec2 dae::GridComponent::GetTileAtPixel(int pixelX, int pixelY) const
{
	const int TileWidth = 10;
	const int TileHeight = 10;

	int tileX = pixelX / TileWidth;
	int tileY = pixelY / TileHeight;

	if (tileX < 0 || tileX >= m_Width || tileY < 0 || tileY >= m_Height)
		return { -1, -1 }; 

	return { tileX, tileY };
}

void dae::GridComponent::Render() const
{
	for (size_t idx{}; idx < m_TileTextures.size(); ++idx)
	{
		if (!m_TileTextures[idx]) return;
		m_TileTextures[idx]->Render();
	}
}

