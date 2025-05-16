#pragma once
#include "BaseComponent.h"
#include <stdexcept>
#include <memory>
#include "TextureComponent.h"

namespace dae
{

	struct FlipBookConfig
	{
		std::string Path{};
		int Frames{};
		float FrameWidth{};
		float FrameHeight{ -1 }; // If Height stays -1 it will use the height of the texture
		float frameTime{};
		bool bRepeats{};
		int Scale{ 1 };
	};

	class FlipBookComponent final
	{
	public:
		FlipBookComponent() = default;
		FlipBookComponent(GameObject* ParentComponentOwner,TextureComponent* UsedTexture, const FlipBookConfig& Config);
		FlipBookComponent(GameObject* ParentComponentOwner, const std::string& Path, const FlipBookConfig& Config);
		virtual ~FlipBookComponent() = default;
		
		FlipBookComponent(const FlipBookComponent&) = delete;
		FlipBookComponent(FlipBookComponent&&) noexcept = delete;
		FlipBookComponent& operator=(const FlipBookComponent&) = delete;
		FlipBookComponent& operator=(FlipBookComponent&&) noexcept = delete;

		void Play();
		void Stop();

		void ChangeConfig(const FlipBookConfig& NewConfig) { m_TextureConfig = NewConfig; }

		void Update(const float deltaTime);

		void Render() const;

	private:
		
		GameObject* m_ParentComponentOwner{};

		std::unique_ptr<TextureComponent> m_UsedTexture{};
		FlipBookConfig m_TextureConfig{};
		
		int m_CurrentFrame{1};
		float m_CurrentFrameTime{};

		bool bIsPlaying{ false };
		bool bPlayedOnce{ false };
	};

}
