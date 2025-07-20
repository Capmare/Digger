#include "TextureComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include <iostream>

dae::TextureComponent::~TextureComponent()
{
	SDL_DestroyTexture(m_OriginalTexture);
}
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

void dae::TextureComponent::DrawFilledCircleOnTexture(glm::ivec2 Middle, int radius, SDL_Color color)
{
	if (!m_IsLocked) return;

	Uint32 mappedColor = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888), color.r, color.g, color.b, color.a);

	for (int y = -radius; y <= radius; ++y)
	{
		for (int x = -radius; x <= radius; ++x)
		{
			if (x * x + y * y <= radius * radius)
			{
				int px = Middle.x + x;
				int py = Middle.y + y;

				if (px >= 0 && px < m_LockedWidth && py >= 0 && py < m_LockedHeight)
				{
					m_LockedPixels[py * m_LockedWidth + px] = mappedColor;
				}
			}
		}
	}

}

void dae::TextureComponent::Lock()
{
	if (m_IsLocked) return;

	m_OriginalTexture = m_Texture->GetSDLTexture();
	if (!m_OriginalTexture) return;

	SDL_Renderer* renderer = Renderer::GetInstance().GetSDLRenderer();
	SDL_QueryTexture(m_OriginalTexture, nullptr, nullptr, &m_LockedWidth, &m_LockedHeight);

	SDL_Texture* renderTarget = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		m_LockedWidth, m_LockedHeight);

	SDL_SetRenderTarget(renderer, renderTarget);
	SDL_RenderCopy(renderer, m_OriginalTexture, nullptr, nullptr);

	m_LockedPixels.resize(m_LockedWidth * m_LockedHeight);
	if (SDL_RenderReadPixels(renderer, nullptr, SDL_PIXELFORMAT_RGBA8888, m_LockedPixels.data(), m_LockedWidth * sizeof(Uint32)) != 0)
	{
		SDL_Log("Failed to read pixels: %s", SDL_GetError());
		m_LockedPixels.clear();
		SDL_DestroyTexture(renderTarget);
		SDL_SetRenderTarget(renderer, nullptr);
		return;
	}

	SDL_DestroyTexture(renderTarget);
	SDL_SetRenderTarget(renderer, nullptr);
	m_IsLocked = true;
}

void dae::TextureComponent::Unlock()
{
	if (!m_IsLocked) return;

	SDL_Renderer* renderer = Renderer::GetInstance().GetSDLRenderer();

	SDL_Texture* newTex = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_STREAMING,
		m_LockedWidth, m_LockedHeight);

	if (!newTex)
	{
		SDL_Log("Failed to create new texture: %s", SDL_GetError());
		return;
	}

	void* pixels = nullptr;
	int pitch = 0;
	if (SDL_LockTexture(newTex, nullptr, &pixels, &pitch) != 0)
	{
		SDL_Log("Failed to lock new texture: %s", SDL_GetError());
		SDL_DestroyTexture(newTex);
		return;
	}

	for (int y = 0; y < m_LockedHeight; ++y)
	{
		memcpy(
			static_cast<Uint8*>(pixels) + y * pitch,
			m_LockedPixels.data() + y * m_LockedWidth,
			m_LockedWidth * sizeof(Uint32));
	}

	SDL_UnlockTexture(newTex);

	if (m_OriginalTexture)
		SDL_DestroyTexture(m_OriginalTexture);

	m_Texture->SetSDLTexture(newTex);

	m_LockedPixels.clear();
	m_LockedWidth = 0;
	m_LockedHeight = 0;
	m_IsLocked = false;
}
