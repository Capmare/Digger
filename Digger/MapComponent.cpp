#include "MapComponent.h"
#include "TextureComponent.h"
#include <iostream>
#include "Renderer.h"

void dae::MapComponent::Update(const float )
{

}


void dae::MapComponent::Render() const
{
	if (m_MergedTexture)
	{
		m_MergedTexture->Render();
	}

}

void dae::MapComponent::ClearTunnelArea(glm::ivec2 Middle, int Rad) const
{
	if (m_MergedTexture)
	{
		m_MergedTexture->DrawFilledCircleOnTexture(Middle, Rad, SDL_Color(0, 0, 0, 1));
	}
}

void dae::MapComponent::UpdateSurfaceFromTexture()
{
	if (!m_MapSurface)
	{
		int width = 1280; 
		int height = 960;
		m_MapSurface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA8888);
		if (!m_MapSurface)
		{
			SDL_Log("Failed to create surface: %s", SDL_GetError());
			return;
		}
	}

	SDL_Renderer* renderer = Renderer::GetInstance().GetSDLRenderer();

	if (SDL_RenderReadPixels(renderer, nullptr, SDL_PIXELFORMAT_RGBA8888, m_MapSurface->pixels, m_MapSurface->pitch) != 0)
	{
		SDL_Log("Failed to read pixels from renderer: %s", SDL_GetError());
	}
}


void dae::MapComponent::CreateSurface()
{
	const int width = 1280;
	const int height = 960;

	if (!m_MapSurface)
	{
		m_MapSurface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA8888);
		if (!m_MapSurface)
		{
			SDL_Log("Failed to create surface: %s", SDL_GetError());
			return;
		}
	}

	SDL_Renderer* renderer = Renderer::GetInstance().GetSDLRenderer();
	SDL_SetRenderTarget(renderer, m_MapTunnelTexture);

	if (SDL_RenderReadPixels(renderer, nullptr, SDL_PIXELFORMAT_RGBA8888, m_MapSurface->pixels, m_MapSurface->pitch) != 0)
	{
		SDL_Log("Failed to read pixels from render target: %s", SDL_GetError());
	}

	SDL_SetRenderTarget(renderer, nullptr);
}

