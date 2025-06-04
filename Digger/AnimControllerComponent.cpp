#include "AnimControllerComponent.h"

void dae::AnimControllerComponent::Update(const float deltaTime)
{
	m_CurrentState->GetFlipBook()->Update(deltaTime);
}

void dae::AnimControllerComponent::Render() const
{
	m_CurrentState->GetFlipBook()->Render();

}

void dae::AnimControllerComponent::RotateAnimation(const float Degrees)
{
	m_CurrentState->Rotate(Degrees);
}
