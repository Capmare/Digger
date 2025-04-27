#include "TextureComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "GameObject.h"
void dae::TextureComponent::Render() const
{
	const auto& pos = GetOwner()->GetWorldPosition();
	Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
}

void dae::TextureComponent::SetTexture(const std::string& filename)
{

	m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
}
