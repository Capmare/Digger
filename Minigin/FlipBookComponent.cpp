#include "FlipBookComponent.h"
#include "TextureComponent.h"
#include "GameObject.h"

dae::FlipBookComponent::FlipBookComponent(GameObject* Owner, TextureComponent* UsedTexture, const FlipBookConfig& Config) : BaseComponent(Owner), m_UsedTexture{ UsedTexture }, m_TextureConfig{ Config }
{
	if (UsedTexture == nullptr) throw std::runtime_error("m_UsedTexture is nullptr");

	if (Config.FrameHeight == -1)
	{
		m_TextureConfig.FrameHeight = static_cast<float>(UsedTexture->GetTextureResolution().y);
	}
}

dae::FlipBookComponent::FlipBookComponent(GameObject* Owner, const std::string& Path, const FlipBookConfig& Config) : BaseComponent(Owner), m_TextureConfig{ Config }
{
	m_UsedTexture = std::make_unique<TextureComponent>(Owner,Path);


	if (m_UsedTexture == nullptr) throw std::runtime_error("m_UsedTexture is nullptr");

	if (Config.FrameHeight == -1)
	{
		m_TextureConfig.FrameHeight = static_cast<float>(m_UsedTexture->GetTextureResolution().y);
	}
}

void dae::FlipBookComponent::Play()
{
	m_CurrentFrame = 0;
	bIsPlaying = true;
}

void dae::FlipBookComponent::Stop()
{
	bIsPlaying = false;
}

void dae::FlipBookComponent::Update(const float deltaTime)
{
	if (!bIsPlaying) return;

	m_UsedTexture->Update(deltaTime);

	if (!m_TextureConfig.bRepeats && bPlayedOnce) return;
	
	
	m_CurrentFrameTime += deltaTime;
	if (m_CurrentFrameTime > m_TextureConfig.frameTime)
	{
		m_CurrentFrameTime = 0;
		if (m_CurrentFrame < m_TextureConfig.Frames-1)
		{
			++m_CurrentFrame;
		}
		else
		{
			m_CurrentFrame = 0;
		}

	}

	if (!bPlayedOnce && m_CurrentFrame == m_TextureConfig.Frames-1)
	{
		bPlayedOnce = true;
	}

	SDL_Rect DstParams
	{
			static_cast<int>(GetOwner()->GetWorldTransform().m_position.x),static_cast<int>(GetOwner()->GetWorldTransform().m_position.y),
			static_cast<int>(m_TextureConfig.FrameWidth) * m_TextureConfig.Scale, static_cast<int>(m_TextureConfig.FrameHeight) * m_TextureConfig.Scale
	};

	SDL_Rect SrcParams
	{
		static_cast<int>(m_CurrentFrame * m_TextureConfig.FrameWidth),0,
		static_cast<int>(m_TextureConfig.FrameWidth),static_cast<int>(m_TextureConfig.FrameHeight)
	};

	// change flipbook frame
	m_UsedTexture->SetRenderParams(DstParams,SrcParams);


}

void dae::FlipBookComponent::Render() const
{

	m_UsedTexture->Render();

}
