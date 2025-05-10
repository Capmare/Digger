#pragma once
#include "BaseComponent.h"
#include <stdexcept>
#include <memory>

namespace dae
{
	class TextureComponent;

	struct FlipBookConfig
	{
		int Frames{};
		float FrameWidth{};
		float FrameHeight{ -1 }; // If Height stays -1 it will use the height of the texture
		float frameTime{};
		bool bRepeats{};
		int Scale{ 1 };
	};

	class FlipBookComponent : public BaseComponent
	{
	public:
		FlipBookComponent() = default;
		FlipBookComponent(GameObject* Owner, TextureComponent* UsedTexture, const FlipBookConfig& Config);
		FlipBookComponent(GameObject* Owner, const std::string& Path, const FlipBookConfig& Config);
		virtual ~FlipBookComponent() = default;
		
		FlipBookComponent(const FlipBookComponent&) = delete;
		FlipBookComponent(FlipBookComponent&&) noexcept = delete;
		FlipBookComponent& operator=(const FlipBookComponent&) = delete;
		FlipBookComponent& operator=(FlipBookComponent&&) noexcept = delete;

		void Play();
		void Stop();

		void ChangeConfig(const FlipBookConfig& NewConfig) { m_TextureConfig = NewConfig; }

		void Update(const float deltaTime) override;

		void Render() const override;

	private:
		std::unique_ptr<TextureComponent> m_UsedTexture{};
		FlipBookConfig m_TextureConfig{};
		
		int m_CurrentFrame{1};
		float m_CurrentFrameTime{};

		bool bIsPlaying{ false };
		bool bPlayedOnce{ false };
	};

}
