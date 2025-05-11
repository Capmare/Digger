#pragma once
#include <BaseComponent.h>
#include "AnimationState.h"

namespace dae
{
	class AnimControllerComponent :
		public BaseComponent
	{
	public:
		AnimControllerComponent(GameObject* Owner) : BaseComponent(Owner) {};
		virtual ~AnimControllerComponent() = default;
		
		AnimControllerComponent(const AnimControllerComponent&) = delete;
		AnimControllerComponent(AnimControllerComponent&&) noexcept = delete;
		AnimControllerComponent& operator=(const AnimControllerComponent&) = delete;
		AnimControllerComponent& operator=(AnimControllerComponent&&) noexcept = delete;

		void ChangeState(AnimationState* NewState)
		{
			if (m_CurrentState)
			{
				m_CurrentState->ExitState(this);
			}
			if (NewState)
			{
				m_CurrentState = NewState;
				m_CurrentState->EnterState(this);
			}
		}


	private:
		AnimationState* m_CurrentState{};
	};

}
