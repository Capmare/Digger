#include "FlipBookComponent.h"
#include "TextureComponent.h"
#include "GameObject.h"
#include <iostream>

dae::FlipBookComponent::FlipBookComponent(GameObject* ParentComponentOwner, TextureComponent* UsedTexture, const FlipBookConfig& Config) 
	: m_ParentComponentOwner{ParentComponentOwner}, m_UsedTexture{ UsedTexture }, m_TextureConfig{ Config }
{
	if (UsedTexture == nullptr) throw std::runtime_error("m_UsedTexture is nullptr");

	if (Config.FrameHeight == -1)
	{
		m_TextureConfig.FrameHeight = static_cast<float>(UsedTexture->GetTextureResolution().y);
	}
}

dae::FlipBookComponent::FlipBookComponent(GameObject* ParentComponentOwner, const std::string& Path, const FlipBookConfig& Config) 
	: m_ParentComponentOwner{ParentComponentOwner}, m_TextureConfig { Config }
{
	m_UsedTexture = std::make_unique<TextureComponent>(m_ParentComponentOwner,Path);

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

void dae::FlipBookComponent::Rotate(const float Degrees)
{
	m_UsedTexture->m_RotationAngle = Degrees;
}

void dae::FlipBookComponent::Update(const float deltaTime)
{
	m_UsedTexture->Update(deltaTime);

	if (bIsPlaying)
	{
		if (!m_TextureConfig.bRepeats && bPlayedOnce)
		{
			// do nothing
		}
		else
		{
			m_CurrentFrameTime += deltaTime;
			if (m_CurrentFrameTime > m_TextureConfig.frameTime)
			{
				m_CurrentFrameTime = 0;
				if (m_CurrentFrame < m_TextureConfig.Frames - 1)
				{
					++m_CurrentFrame;
				}
				else
				{
					m_CurrentFrame = 0;
				}
			}

			if (!bPlayedOnce && m_CurrentFrame == m_TextureConfig.Frames - 1)
			{
				bPlayedOnce = true;
			}
		}
	}

	// Always update position and rendering parameters
	
	const glm::ivec2 OverridenPos = bPositionOverriden ? m_OverridenPosition : glm::ivec2{ m_ParentComponentOwner->GetWorldTransform().m_position.x, m_ParentComponentOwner->GetWorldTransform().m_position.y };

	const SDL_Rect DstParams
	{
		static_cast<int>(OverridenPos.x),
		static_cast<int>(OverridenPos.y),
		static_cast<int>(m_TextureConfig.FrameWidth) * m_TextureConfig.Scale,
		static_cast<int>(m_TextureConfig.FrameHeight) * m_TextureConfig.Scale
	};

	const SDL_Rect SrcParams
	{
		static_cast<int>(m_CurrentFrame * m_TextureConfig.FrameWidth), 0,
		static_cast<int>(m_TextureConfig.FrameWidth), static_cast<int>(m_TextureConfig.FrameHeight)
	};

	m_UsedTexture->SetRenderParams(DstParams, SrcParams);
}


void dae::FlipBookComponent::Render() const
{
	m_UsedTexture->Render();
}
