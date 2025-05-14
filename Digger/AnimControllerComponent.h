#pragma once
#include <BaseComponent.h>
#include <vector>
#include <memory>
#include <unordered_map>
#include "AnimationState.h"
#include "FlipBookComponent.h"

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

		template<typename T>
		void CreateState(const FlipBookConfig& FlipBook)
		{
			if constexpr (std::is_base_of<dae::AnimationState,T>::value)
			{
				
				m_AnimationStates.insert({ T::GetStateName(),std::make_unique<T>(std::make_unique<dae::FlipBookComponent>(FlipBook)) });
			}

		};
	private:
		std::unordered_map<std::string, std::unique_ptr<AnimationState>> m_AnimationStates{};
		AnimationState* m_CurrentState{};
	};

}
