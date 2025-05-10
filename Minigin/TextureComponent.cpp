#include "TextureComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "GameObject.h"
void dae::TextureComponent::Render() const
{
	const auto& pos = GetOwner()->GetWorldPosition();
	if (m_RenderParams.w == 0 || m_RenderParams.h == 0)
	{
		Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
	}
	else
	{
		Renderer::GetInstance().RenderTexture(*m_Texture, m_RenderParams, m_RegionRenderParams);
	}
}

void dae::TextureComponent::SetTexture(const std::string& filename)
{

	m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
}
