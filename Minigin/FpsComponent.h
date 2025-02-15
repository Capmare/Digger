#pragma once
#include "BaseComponent.h"
#include <memory>
#include "TextComponent.h"

namespace dae
{
	class FpsComponent final : public BaseComponent
	{
	public:
		FpsComponent();
		~FpsComponent() = default;
		
		FpsComponent(const FpsComponent&) = delete;
		FpsComponent(FpsComponent&&) noexcept = delete;
		FpsComponent& operator=(const FpsComponent&) = delete;
		FpsComponent& operator=(FpsComponent&&) noexcept = delete;


		void Update(const float deltaTime) override;
		void Render() const override;
		int GetFrameRate() const { return m_fps; }
		void ShowFpsText(bool val) { bShowFpsText = val; }

	protected:
	private:

		std::unique_ptr<TextComponent> m_FpsText;

		int m_fps{};
		bool bShowFpsText{ true };
		float m_accumulatedTime{};
		int m_frameCount{};
		float m_smoothFps;
	};
}


