#include "TextureComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "GameObject.h"
void dae::TextureComponent::Render() const
{

	const auto& pos = !bOverrideOwnerPosition ? GetOwner()->GetWorldPosition() : OverridenPosition;

	SDL_RendererFlip Flip = SDL_FLIP_NONE;
	if (m_RotationAngle > 90 && m_RotationAngle < 270)
	{
		Flip = SDL_FLIP_VERTICAL;
	}


	if (m_RenderParams.w == 0 || m_RenderParams.h == 0)
	{
		// Fallback to no size: Get texture size automatically
		glm::ivec2 size = m_Texture->GetSize();
		SDL_Rect dstRect{ static_cast<int>(pos.x), static_cast<int>(pos.y), size.x, size.y };
		SDL_Point center{ dstRect.w / 2, dstRect.h / 2 };

		SDL_RenderCopyEx(Renderer::GetInstance().GetSDLRenderer(),
			m_Texture->GetSDLTexture(), nullptr, &dstRect, m_RotationAngle, &center, Flip);
	}
	else
	{
		SDL_Point center{ m_RenderParams.w / 2, m_RenderParams.h / 2 };

		SDL_RenderCopyEx(Renderer::GetInstance().GetSDLRenderer(),
			m_Texture->GetSDLTexture(), &m_RegionRenderParams, &m_RenderParams, m_RotationAngle, &center, Flip);
	}
}

void dae::TextureComponent::SetTexture(const std::string& filename)
{

	m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
}
