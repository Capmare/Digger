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

void dae::AnimationState::SetEnterStateFlipBook(AnimControllerComponent* AnimController)
{
	if (FlipBookComponent* CurrentFlipBook = AnimController->GetOwner()->GetFirstComponentOfType<FlipBookComponent>())
	{
		m_PreviousFlipBook = CurrentFlipBook;
		CurrentFlipBook = m_FlipBook;
		std::cout << "Entering state" << std::endl;
		CurrentFlipBook->Play();
	}
}

void dae::AnimationState::SetExitStateFlipBook(AnimControllerComponent* AnimController)
{
	if (FlipBookComponent* CurrentFlipBook = AnimController->GetOwner()->GetFirstComponentOfType<FlipBookComponent>())
	{
		CurrentFlipBook->Stop();
		CurrentFlipBook = m_PreviousFlipBook;
		std::cout << "Exiting state" << std::endl;

	}
}
