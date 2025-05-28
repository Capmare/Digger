#pragma once
#include <BaseComponent.h>
#include <vector>
#include "glm.hpp"

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
		std::vector<TileType> m_Tiles{};
		std::vector<class TextureComponent*> m_TileTextures{};
		int m_Width{};
		int m_Height{};
	};

}
