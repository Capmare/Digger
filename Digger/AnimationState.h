#pragma once
#include <BaseComponent.h>

namespace dae
{
	class AnimControllerComponent;
	class FlipBookComponent;

	class AnimationState
	{
	public:
		AnimationState(FlipBookComponent* Animation, const std::string& StateName) : m_FlipBook{ Animation }, m_StateName{StateName} {};
		virtual ~AnimationState() = default;
		
		AnimationState(const AnimationState&) = delete;
		AnimationState(AnimationState&&) noexcept = delete;
		AnimationState& operator=(const AnimationState&) = delete;
		AnimationState& operator=(AnimationState&&) noexcept = delete;

		virtual void EnterState(AnimControllerComponent* AnimController) = 0;
		virtual void ExitState(AnimControllerComponent* AnimController) = 0;

		std::string GetStateName() const { return m_StateName; }

	protected:
		void SetEnterStateFlipBook(AnimControllerComponent* AnimController);
		void SetExitStateFlipBook(AnimControllerComponent* AnimController);

		FlipBookComponent* m_FlipBook{};
		FlipBookComponent* m_PreviousFlipBook{};

		const std::string m_StateName{ "Unnamed" };
	};


	class IdleState : public AnimationState
	{
	public:
		IdleState(FlipBookComponent* Animation) : AnimationState(Animation,"Idle") {};
		virtual ~IdleState() = default;
		
		IdleState(const IdleState&) = delete;
		IdleState(IdleState&&) noexcept = delete;
		IdleState& operator=(const IdleState&) = delete;
		IdleState& operator=(IdleState&&) noexcept = delete;

		void EnterState(AnimControllerComponent* AnimController) override;
		void ExitState(AnimControllerComponent* AnimController) override;


	};

	class FallingState : public AnimationState
	{
	public:
		FallingState(FlipBookComponent* Animation) : AnimationState(Animation,"Falling") {};
		virtual ~FallingState() = default;
		
		FallingState(const FallingState&) = delete;
		FallingState(FallingState&&) noexcept = delete;
		FallingState& operator=(const FallingState&) = delete;
		FallingState& operator=(FallingState&&) noexcept = delete;

		void EnterState(AnimControllerComponent* AnimController) override;
		void ExitState(AnimControllerComponent* AnimController) override;

	};

	
}
