#pragma once
#include <BaseComponent.h>
#include <memory>

namespace dae
{
	class AnimControllerComponent;
	class FlipBookComponent;

	class AnimationState
	{
	public:
		AnimationState(std::unique_ptr<FlipBookComponent>&& Animation, const std::string& StateName) : m_FlipBook{ std::move(Animation) }, m_StateName{StateName} {};

		virtual ~AnimationState() = default;
		
		AnimationState(const AnimationState&) = delete;
		AnimationState(AnimationState&&) noexcept = delete;
		AnimationState& operator=(const AnimationState&) = delete;
		AnimationState& operator=(AnimationState&&) noexcept = delete;

		virtual void EnterState(AnimControllerComponent* AnimController) = 0;
		virtual void ExitState(AnimControllerComponent* AnimController) = 0;

		std::string GetStateName() const { return m_StateName; }

		FlipBookComponent* GetFlipBook() const { return m_FlipBook.get(); }

		void Rotate(const float Degrees);
	protected:
		void SetEnterStateFlipBook(AnimControllerComponent* AnimController);
		void SetExitStateFlipBook(AnimControllerComponent* AnimController);

		std::unique_ptr<FlipBookComponent> m_FlipBook{};
		FlipBookComponent* m_PreviousFlipBook{};

		const std::string m_StateName{ "Unnamed" };
	};


	class IdleState : public AnimationState
	{
	public:
		IdleState(std::unique_ptr<FlipBookComponent>&& Animation) : AnimationState(std::move(Animation),"Idle") {};

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
		FallingState(std::unique_ptr<FlipBookComponent>&& Animation) : AnimationState(std::move(Animation), "Falling") {};
		virtual ~FallingState() = default;
		
		FallingState(const FallingState&) = delete;
		FallingState(FallingState&&) noexcept = delete;
		FallingState& operator=(const FallingState&) = delete;
		FallingState& operator=(FallingState&&) noexcept = delete;

		void EnterState(AnimControllerComponent* AnimController) override;
		void ExitState(AnimControllerComponent* AnimController) override;

	};

	
}
