#pragma once
#include "BaseComponent.h"
#include <memory>
#include "Transform.h"
#include "Texture2D.h"
#include "SDL_rect.h"

namespace dae  {
	class TextureComponent final: public BaseComponent
	{
	public:
		TextureComponent(GameObject* Owner) : BaseComponent(Owner) {};
		TextureComponent(GameObject* Owner, const std::string& Path) : BaseComponent(Owner) { SetTexture(Path); };
		virtual ~TextureComponent() = default;
		
		TextureComponent(const TextureComponent&) = delete;
		TextureComponent(TextureComponent&&) noexcept = delete;
		TextureComponent& operator=(const TextureComponent&) = delete;
		TextureComponent& operator=(TextureComponent&&) noexcept = delete;

		void Render() const override;

		void SetTexture(const std::string& filename);

		void SetRenderParams(const SDL_Rect& Params, const SDL_Rect& RegionRenderParams) { m_RenderParams = Params; m_RegionRenderParams = RegionRenderParams; }

		const glm::ivec2 GetTextureResolution() const { return m_Texture->GetSize(); }

	private:
		std::shared_ptr<Texture2D> m_Texture{};

		SDL_Rect m_RenderParams{};
		SDL_Rect m_RegionRenderParams{};

	};
}


