#pragma once
#include <BaseComponent.h>
#include <vector>
#include <iostream>
#include "glm.hpp"
#include "TextureComponent.h"
#include "ResourceManager.h"

namespace dae
{
	enum class TileType
	{
		Dirt,
		Tunnel,
	};

	class MapComponent :
		public BaseComponent
	{
	public:
		MapComponent(GameObject* Owner, const int GridXSize, const int GridYSize, const std::vector<std::vector<TileType>>& TileVector) : BaseComponent(Owner), m_Width{ GridXSize }, m_Height{ GridYSize } {
			
			for (const auto& row : TileVector) {
				m_Tiles.insert(m_Tiles.end(), row.begin(), row.end());
			}
			m_TileTextures.reserve(m_Tiles.size());

			for (size_t idx{}; idx < m_Tiles.size(); ++idx)
			{
				std::string Path{};
				switch (m_Tiles[idx])
				{
				default:
					break;
				case TileType::Dirt:
					Path = "cback1.png";
					break;
				case TileType::Tunnel:
					Path = "Black.png";
					break;
				}

				std::cout << Path << std::endl;

				size_t x = idx % GridXSize;
				size_t y = idx / GridXSize;


				std::unique_ptr<TextureComponent> Texture = std::make_unique<TextureComponent>(Owner, Path);
				Texture->bOverrideOwnerPosition = false;
				

				m_TileWidth = Texture->GetTextureResolution().x;
				m_TileHeight = Texture->GetTextureResolution().y;

				SDL_Rect dstRect = {
					static_cast<int>(m_TileWidth * x),
					static_cast<int>(m_TileHeight * y),
					m_TileWidth, m_TileHeight
				};
				SDL_Rect srcRect = {
					0, 0,
					Texture->GetTextureResolution().x,
					Texture->GetTextureResolution().y
				};

				Texture->SetRenderParams(dstRect, srcRect);

				m_TileTextures.emplace_back(std::move(Texture));
			}
			m_SharedMergedTexture = std::make_shared<Texture2D>(ResourceManager::GetInstance().MergeTextures(m_TileTextures, 16));
			m_MergedTexture = std::make_unique<TextureComponent>(Owner, m_SharedMergedTexture);

			for (int idx{}; idx < m_Tiles.size(); ++idx)
			{
				
				int col = idx % GridXSize;
				int row = idx / GridXSize;

				int centerX = col * m_TileWidth + m_TileWidth / 2;
				int centerY = row * m_TileHeight + m_TileHeight / 2;

				int offset = 7;
				switch (m_Tiles[idx])
				{
				default:
					break;
				case TileType::Dirt:
					break;
				case TileType::Tunnel:
					ClearTunnelArea({ centerX-offset, centerY },offset);
					ClearTunnelArea({ centerX-offset-5, centerY },offset);
					ClearTunnelArea({ centerX, centerY }, offset);
					ClearTunnelArea({ centerX+offset, centerY }, offset)	;
					ClearTunnelArea({ centerX+offset+5, centerY }, offset)	;
					break;
				}

			}

			// for (auto& t : m_TileTextures)
			// {
			// 	t.release();
			// }

		};


		virtual ~MapComponent() = default;
		
		MapComponent(const MapComponent&) = delete;
		MapComponent(MapComponent&&) noexcept = delete;
		MapComponent& operator=(const MapComponent&) = delete;
		MapComponent& operator=(MapComponent&&) noexcept = delete;

		void UpdateTileType(const glm::ivec2 Tile, const TileType NewType);

		void Update(const float deltaTime) override;

		glm::ivec2 GetTileAtPixel(int pixelX, int pixelY) const;
		void Render() const override;
		
		TileType GetTileType(glm::ivec2 Tile) { 
			int TileIdx = Tile.y * m_Width + Tile.x;
			return m_Tiles.at(TileIdx); 
		}


		void ClearTunnelArea(glm::ivec2 Middle, int Rad) const;

	private:
		std::vector<TileType> m_Tiles{};
		std::vector<std::unique_ptr<TextureComponent>> m_TileTextures{};
		std::unique_ptr<TextureComponent> m_MergedTexture{};
		std::shared_ptr<Texture2D> m_SharedMergedTexture{};
		int m_Width{};
		int m_Height{};

		int m_TileWidth = 20;
		int m_TileHeight = 4;

		mutable int cnt{};
	};

}
