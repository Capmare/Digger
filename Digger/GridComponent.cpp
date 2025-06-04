#include "GridComponent.h"
#include "TextureComponent.h"
#include <iostream>

void dae::GridComponent::UpdateTileType(const glm::ivec2 Tile, const TileType NewType)
{
	int radius = 3;
	for (int y = -radius; y <= radius; ++y)
	{
		for (int x = -radius; x <= radius; ++x)
		{
			glm::ivec2 offset{ x, y };
			glm::ivec2 tile = Tile + offset;

			// Check if the tile is within the circle
			if (x * x + y * y <= radius * radius)
			{
				// Bounds check
				if (tile.x < 0 || tile.y < 0 || tile.x >= m_Width || tile.y >= m_Height)
					continue;

				int tileIdx = tile.y * m_Width + tile.x;
				m_Tiles[tileIdx] = NewType;

				switch (NewType)
				{
				default:
					break;
				case TileType::Dirt:
					m_TileTextures[tileIdx]->SetTexture("cback1.png");
					break;
				case TileType::Tunnel:
					m_TileTextures[tileIdx]->SetTexture("Black.png");
					break;
				}
			}
		}
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

