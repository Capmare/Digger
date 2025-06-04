#include "AnimationState.h"
#include "AnimControllerComponent.h"
#include "FlipBookComponent.h"
#include "GameObject.h"
#include <iostream>


void dae::IdleState::EnterState(AnimControllerComponent* AnimController)
{
	SetEnterStateFlipBook(AnimController);
}

void dae::IdleState::ExitState(AnimControllerComponent* AnimController)
{
	SetExitStateFlipBook(AnimController);
}

void dae::FallingState::EnterState(AnimControllerComponent* AnimController)
{
	SetEnterStateFlipBook(AnimController);
	// get gravity component (need to implement) and activate gravity
}


void dae::FallingState::ExitState(AnimControllerComponent* AnimController)
{
	SetExitStateFlipBook(AnimController);
}

void dae::AnimationState::Rotate(const float Degrees)
{
	m_FlipBook->Rotate(Degrees);
}

void dae::AnimationState::SetEnterStateFlipBook(AnimControllerComponent* )
{
	std::cout << "Entering state" << std::endl;
	m_FlipBook->Play();
}

void dae::AnimationState::SetExitStateFlipBook(AnimControllerComponent* )
{
	m_FlipBook->Stop();
	std::cout << "Exiting state" << std::endl;
}
