#pragma once
#include "BaseComponent.h"
#include "GameObject.h"
#include <memory> 


namespace dae
{
	enum class GamepadButton {
		A = 0x1000, 
		B = 0x2000,
		X = 0x4000,
		Y = 0x8000
	};

	enum class DPadButton : int16_t
	{
		DPAD_UP = 0x0001,
		DPAD_DOWN = 0x0002,
		DPAD_LEFT = 0x0004,
		DPAD_RIGHT = 0x0008
	};

	struct ThumbInputReturn
	{
		int16_t ThumbLX{};
		int16_t ThumbRX{};
		int16_t ShoulderLY{};
		int16_t ShoulderRY{};
	};

	class PlayerControllerComponent : public BaseComponent
	{
	public:
		PlayerControllerComponent(GameObject* Owner);
		virtual ~PlayerControllerComponent();

		PlayerControllerComponent(const PlayerControllerComponent&) = delete;
		PlayerControllerComponent(PlayerControllerComponent&&) noexcept = delete;
		PlayerControllerComponent& operator=(const PlayerControllerComponent&) = delete;
		PlayerControllerComponent& operator=(PlayerControllerComponent&&) noexcept = delete;

		void Update(const float deltaTime) override;
		class Command* HandleInput();

		GameObject* ReturnOwner() { return GetOwner(); }
	private:
		class Impl; 
		std::unique_ptr<Impl> pImpl;  



	};
}
