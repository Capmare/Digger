#include "MapComponent.h"
#include "TextureComponent.h"
#include <iostream>

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

