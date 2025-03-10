#pragma once
#include "BaseComponent.h"
#include "GameObject.h"
#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>
#include <Xinput.h>
#include "Command.h"

#pragma comment(lib, "Xinput9_1_0.lib")

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
		PlayerControllerComponent(GameObject* Owner) : BaseComponent(Owner) {};
		virtual ~PlayerControllerComponent() = default;
		
		PlayerControllerComponent(const PlayerControllerComponent&) = delete;
		PlayerControllerComponent(PlayerControllerComponent&&) noexcept = delete;
		PlayerControllerComponent& operator=(const PlayerControllerComponent&) = delete;
		PlayerControllerComponent& operator=(PlayerControllerComponent&&) noexcept = delete;

		void Update(const float deltaTime) override;
		Command* HandleInput();

	private:

		bool IsDownThisFrame(dae::DPadButton Button) const
		{
			return m_ThisFramePressedButtons & static_cast<int>(Button);
		}

		bool IsUpThisFrame(dae::DPadButton Button) const
		{
			return m_ThisFrameReleasedButtons& static_cast<int>(Button);
		}
		bool IsPressed(dae::DPadButton Button) const
		{
			return m_CurrentState.Gamepad.wButtons & static_cast<int>(Button);
		}

		ThumbInputReturn HandleThumbXInput();

		void HandleButtonXInput();

		XINPUT_STATE m_CurrentState{};
		XINPUT_STATE m_PrevState{};

		float m_DeadZonePercentage{};

		int m_ControllerIdx{};
		int m_ThisFramePressedButtons{};
		int m_ThisFrameReleasedButtons{};
		int m_PreviousFramePressedButtons{};
	};
}

