#include "FpsComponent.h"
#include "TextComponent.h"
#include "ResourceManager.h"
#include <iostream>

dae::FpsComponent::FpsComponent(GameObject* Owner) : BaseComponent(Owner)
{
	if (GetOwner())
	{
		auto textComponents = GetOwner()->GetAllComponentsOfType<TextComponent>(); // assumes that the first object is a text component, will solve it later
		if (!textComponents.empty())
		{
			m_FpsText = textComponents[0];
			m_FpsText->SetPosition(10, 10);

		}
	}
	
}

void dae::FpsComponent::Update( const float deltaTime)
{
	m_accumulatedTime += deltaTime;
	++m_frameCount;

	if (m_accumulatedTime >= 1.f)
	{
		std::cout << m_accumulatedTime << " " << m_frameCount << std::endl;
		m_fps = m_frameCount;
		m_accumulatedTime = 0.f;
		m_frameCount = 0;
	}

	if (m_FpsText)
	{
		std::string fpsString = "FPS: " + std::to_string(m_fps);
		m_FpsText->SetText(fpsString);
	}

}


