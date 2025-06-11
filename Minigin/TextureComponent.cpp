#include "TextureComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include <iostream>
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
	SDL_Texture* oldTex = m_Texture->GetSDLTexture();
	if (!oldTex) return;

	SDL_Renderer* renderer = Renderer::GetInstance().GetSDLRenderer();

	int width = 0, height = 0;
	SDL_QueryTexture(oldTex, nullptr, nullptr, &width, &height);

	// create render target
	SDL_Texture* renderTarget = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		width, height);

	if (!renderTarget) {
		SDL_Log("Failed to create render target: %s", SDL_GetError());
		return;
	}

	// clear render target
	SDL_SetRenderTarget(renderer, renderTarget);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	// old texture -> render target
	SDL_RenderCopy(renderer, oldTex, nullptr, nullptr);

	// render target -> buffer
	std::vector<Uint32> pixels(width * height);
	if (SDL_RenderReadPixels(renderer, nullptr, SDL_PIXELFORMAT_RGBA8888, pixels.data(), width * sizeof(Uint32)) != 0) {
		SDL_Log("Failed to read pixels: %s", SDL_GetError());
		SDL_DestroyTexture(renderTarget);
		SDL_SetRenderTarget(renderer, nullptr);
		return;
	}

	// draw the circle
	Uint32 mappedColor = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888), color.r, color.g, color.b, color.a);

	for (int y = -radius; y <= radius; ++y)
	{
		for (int x = -radius; x <= radius; ++x)
		{
			if (x * x + y * y <= radius * radius)
			{
				int px = Middle.x + x;
				int py = Middle.y + y;

				if (px >= 0 && px < width && py >= 0 && py < height)
				{
					pixels[py * width + px] = mappedColor;
				}
			}
		}
	}

	
	SDL_Texture* newTex = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_STREAMING,
		width, height);

	if (!newTex) {
		SDL_Log("Failed to create new streaming texture: %s", SDL_GetError());
		SDL_DestroyTexture(renderTarget);
		SDL_SetRenderTarget(renderer, nullptr);
		return;
	}

	// copy modified pixels
	void* newPixels = nullptr;
	int newPitch = 0;
	if (SDL_LockTexture(newTex, nullptr, &newPixels, &newPitch) != 0) {
		SDL_Log("Failed to lock new texture: %s", SDL_GetError());
		SDL_DestroyTexture(newTex);
		SDL_DestroyTexture(renderTarget);
		SDL_SetRenderTarget(renderer, nullptr);
		return;
	}

	// copy line by line
	for (int row = 0; row < height; ++row) {
		memcpy(
			static_cast<Uint8*>(newPixels) + row * newPitch,
			pixels.data() + row * width,
			width * sizeof(Uint32));
	}

	SDL_UnlockTexture(newTex);

	SDL_DestroyTexture(renderTarget);
	SDL_SetRenderTarget(renderer, nullptr);

	SDL_DestroyTexture(oldTex);
	m_Texture->SetSDLTexture(newTex);

}
