#pragma once
#include <BaseComponent.h>
#include <vector>
#include <iostream>
#include "glm.hpp"
#include "TextureComponent.h"

namespace dae
{
	enum class TileType
	{
		Dirt,
		Tunnel,
	};

	class GridComponent :
		public BaseComponent
	{
	public:
		GridComponent(GameObject* Owner, const int GridXSize, const int GridYSize, const std::vector<std::vector<TileType>>& TileVector) : BaseComponent(Owner), m_Width{ GridXSize }, m_Height{ GridYSize } {
			
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

				const int TileWidth = 10;
				const int TileHeight = 10;

				std::unique_ptr<TextureComponent> Texture = std::make_unique<TextureComponent>(Owner, Path);
				Texture->bOverrideOwnerPosition = false;
				
				SDL_Rect dstRect = {
					static_cast<int>(TileWidth * x),
					static_cast<int>(TileHeight * y),
					TileWidth, TileHeight
				};
				SDL_Rect srcRect = {
					0, 0,
					Texture->GetTextureResolution().x,
					Texture->GetTextureResolution().y
				};

				Texture->SetRenderParams(dstRect, srcRect);

				m_TileTextures.emplace_back(std::move(Texture));

			}

		};


		virtual ~GridComponent() = default;
		
		GridComponent(const GridComponent&) = delete;
		GridComponent(GridComponent&&) noexcept = delete;
		GridComponent& operator=(const GridComponent&) = delete;
		GridComponent& operator=(GridComponent&&) noexcept = delete;

		void UpdateTileType(const glm::ivec2 Tile, const TileType NewType);

		void Update(const float deltaTime) override;

		void Render() const override;

		
	private:

		void CreateTileSize(int size, TileType TyleType);

		std::vector<TileType> m_Tiles{};
		std::vector<std::unique_ptr<TextureComponent>> m_TileTextures{};
		int m_Width{};
		int m_Height{};
	};

}
