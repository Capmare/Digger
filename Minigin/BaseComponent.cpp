#include "BaseComponent.h"
#include <typeinfo>

dae::BaseComponent::BaseComponent() : TYPE{typeid(this).name()}
{

}

void dae::BaseComponent::Update([[maybe_unused]] const float deltaTime)
{

}

void dae::BaseComponent::FixedUpdate([[maybe_unused]] const float fixedDeltaTime)
{

}
