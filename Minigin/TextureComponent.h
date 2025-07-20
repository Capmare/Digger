#pragma once
#include "BaseComponent.h"
#include <memory>
#include "Transform.h"
#include "Texture2D.h"
#include "SDL_rect.h"
#include <vector>

namespace dae  {
	class TextureComponent final: public BaseComponent
	{
	public:
		TextureComponent(GameObject* Owner, const float RotationAngle = 0) : BaseComponent(Owner), m_RotationAngle(RotationAngle) {};
		TextureComponent(GameObject* Owner, const std::string& Path) : BaseComponent(Owner) { SetTexture(Path); };
		TextureComponent(GameObject* Owner, std::shared_ptr<Texture2D> Texture) : BaseComponent(Owner), m_Texture(std::move(Texture)) { };
		virtual ~TextureComponent();;
		
		TextureComponent(const TextureComponent&) = delete;
		//TextureComponent(TextureComponent&&) noexcept = delete; // will need the move constructor for merging textures
		TextureComponent& operator=(const TextureComponent&) = delete;
		TextureComponent& operator=(TextureComponent&&) noexcept = delete;

		void Render() const override;

		void SetTexture(const std::string& filename);

		void SetRenderParams(const SDL_Rect& Params, const SDL_Rect& RegionRenderParams) { m_RenderParams = Params; m_RegionRenderParams = RegionRenderParams; }

		const glm::ivec2 GetTextureResolution() const { return m_Texture->GetSize(); }

		bool bOverrideOwnerPosition{ false };
		glm::vec3 OverridenPosition{};

		float m_RotationAngle = 0.f; 

		struct SDL_Texture* GetTexture() const { return m_Texture->GetSDLTexture(); }

		void DrawFilledCircleOnTexture(glm::ivec2 Middle, int radius, SDL_Color color);

		void Lock();
		void Unlock();
	private:
		std::shared_ptr<Texture2D> m_Texture{};
		SDL_Rect m_RenderParams{};
		SDL_Rect m_RegionRenderParams{};

		SDL_Texture* m_OriginalTexture{nullptr};
		std::vector<Uint32> m_LockedPixels{};
		int m_LockedWidth{};
		int m_LockedHeight{};
		bool m_IsLocked{false};
	};
}


