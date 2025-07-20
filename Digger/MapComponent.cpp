#include "MapComponent.h"
#include "TextureComponent.h"
#include <iostream>
#include "Renderer.h"
#include <algorithm>
#include <execution>

dae::MapComponent::~MapComponent()
{
	SDL_DestroyTexture(m_MapTunnelTexture);
	SDL_FreeSurface(m_MapSurface);
}

void dae::MapComponent::Update(const float deltaTime)
{

	if (!m_ClearTunnelRequest.empty())
	{
		m_WaitTimeForClearTunnel += deltaTime;
		if (m_WaitTimeForClearTunnel > m_MaxWaitTimeForClearTunnel)
		{
			bWasUpdated = true;
			m_WaitTimeForClearTunnel = 0;

			// lock texture
			m_MergedTexture->Lock();
			std::for_each(std::execution::par, m_ClearTunnelRequest.begin(), m_ClearTunnelRequest.end(),
				[&](const std::pair<glm::ivec2, int>& req)
				{
					m_MergedTexture->DrawFilledCircleOnTexture(req.first, req.second, SDL_Color(0, 0, 0, 1));
				});

			// unlock texture
			m_MergedTexture->Unlock();

			m_ClearTunnelRequest.clear();

		}
	}
	
}


void dae::MapComponent::Render() const
{
	if (m_MergedTexture)
	{
		m_MergedTexture->Render();
	}

}

void dae::MapComponent::ClearTunnelAreaRequest(glm::ivec2 Middle, int Rad)
{
	if (m_MergedTexture)
	{
		m_ClearTunnelRequest.emplace_back(Middle,Rad);
		// m_MergedTexture->DrawFilledCircleOnTexture(Middle, Rad, SDL_Color(0, 0, 0, 1));
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

