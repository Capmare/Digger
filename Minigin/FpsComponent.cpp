#include "FpsComponent.h"
#include "TextComponent.h"
#include "ResourceManager.h"
#include <iostream>

dae::FpsComponent::FpsComponent()
{
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	m_FpsText = make_unique<TextComponent>("FPS:", font);
	m_FpsText->SetPosition(10, 10);

}

void dae::FpsComponent::Update([[maybe_unused]] const float deltaTime)
{
	m_accumulatedTime += deltaTime;
	++m_frameCount;

	if (m_accumulatedTime >= 1.0f)
	{
		std::cout << m_accumulatedTime << " " << m_frameCount << std::endl;
		m_fps = m_frameCount;
		m_accumulatedTime -= 1.0f; // remove one and keep the left over for next frame calculation
		m_frameCount = 0;
	}

	std::string fpsString = "FPS: " + std::to_string(m_fps);
	m_FpsText->Update(deltaTime);
	m_FpsText->SetText(fpsString);


}

void dae::FpsComponent::Render() const
{
	m_FpsText->Render();
}

