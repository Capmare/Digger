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
		AnimControllerComponent(GameObject* Owner) : BaseComponent(Owner) { };
		virtual ~AnimControllerComponent() = default;
		
		AnimControllerComponent(const AnimControllerComponent&) = delete;
		AnimControllerComponent(AnimControllerComponent&&) noexcept = delete;
		AnimControllerComponent& operator=(const AnimControllerComponent&) = delete;
		AnimControllerComponent& operator=(AnimControllerComponent&&) noexcept = delete;

		void ChangeState(const std::string& State)
		{
			// avoid getting back into the same state
			if (m_CurrentState == m_AnimationStates.at(State).get()) return;

			if (m_CurrentState)
			{
				m_CurrentState->ExitState(this);
			}
			m_CurrentState = m_AnimationStates.at(State).get();
			if (m_CurrentState)
			{
				m_CurrentState->EnterState(this);
				m_CurrentState->Rotate(0);
			}
		}

		template<typename T>
		void CreateState(const FlipBookConfig& FlipBook)
		{
			if constexpr (std::is_base_of<dae::AnimationState,T>::value)
			{
				auto FlipBookComp = std::make_unique<dae::FlipBookComponent>(this->GetOwner(), FlipBook.Path, FlipBook);
				auto AnimState = std::make_unique<T>(std::move(FlipBookComp));
				m_AnimationStates.insert({ AnimState->GetStateName(),std::move(AnimState) });
			}


			
		};

		void Update(const float deltaTime) override;


		void Render() const override;

		void RotateAnimation(const float Degrees);

		const dae::AnimationState* GetCurrentState() const { return m_CurrentState; }
	private:
		std::unordered_map<std::string, std::unique_ptr<AnimationState>> m_AnimationStates{};
		AnimationState* m_CurrentState{};
	};

	

}
