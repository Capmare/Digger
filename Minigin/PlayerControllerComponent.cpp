#include "PlayerControllerComponent.h"
#include <iostream>
#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>
#include <Xinput.h>
#include "Command.h"

#pragma comment(lib, "Xinput9_1_0.lib")

namespace dae
{
	class PlayerControllerComponent::Impl
	{
		GameObject* m_Owner{};
	public:
		Impl(GameObject* Owner) : m_Owner{ Owner } {};
		~Impl() = default;

		void Update(const float)
		{
			ThumbInputReturn ThumbInput = HandleThumbXInput();
			HandleButtonXInput();
			Command* CurrentCommand = HandleInput();
			if (CurrentCommand)
			{
				CurrentCommand->Exec(*m_Owner);
			}
		}

		Command* HandleInput()
		{
			if (IsPressed(DPadButton::DPAD_LEFT) && IsPressed(DPadButton::DPAD_UP))
			{
				return new Command_Move({ -1,-1 });
			}
			if (IsPressed(DPadButton::DPAD_RIGHT) && IsPressed(DPadButton::DPAD_UP))
			{
				return new Command_Move({ 1,-1 });
			}
			if (IsPressed(DPadButton::DPAD_LEFT) && IsPressed(DPadButton::DPAD_DOWN))
			{
				return new Command_Move({ -1,1 });
			}
			if (IsPressed(DPadButton::DPAD_RIGHT) && IsPressed(DPadButton::DPAD_DOWN))
			{
				return new Command_Move({ 1,1 });
			}

			// Normal movement
			if (IsPressed(DPadButton::DPAD_UP))
			{
				return new Command_Move({ 0,-1 });
			}
			if (IsPressed(DPadButton::DPAD_DOWN))
			{
				return new Command_Move({ 0,1 });
			}
			if (IsPressed(DPadButton::DPAD_LEFT))
			{
				return new Command_Move({ -1,0 });
			}
			if (IsPressed(DPadButton::DPAD_RIGHT))
			{
				return new Command_Move({ 1,0 });
			}

			return nullptr;
		}

		ThumbInputReturn HandleThumbXInput()
		{
			ThumbInputReturn ReturnValues{};

			auto LThumbValue = m_CurrentState.Gamepad.sThumbLX;
			auto RThumbValue = m_CurrentState.Gamepad.sThumbRX;
			ReturnValues.ShoulderLY = m_CurrentState.Gamepad.sThumbLY;
			ReturnValues.ShoulderRY = m_CurrentState.Gamepad.sThumbRY;

			if (LThumbValue > (SHRT_MAX * m_DeadZonePercentage) || LThumbValue < (SHRT_MIN * m_DeadZonePercentage))
			{
				ReturnValues.ThumbLX = LThumbValue;
			}
			if (RThumbValue > (SHRT_MAX * m_DeadZonePercentage) || RThumbValue < (SHRT_MIN * m_DeadZonePercentage))
			{
				ReturnValues.ThumbRX = RThumbValue;
			}

			return ReturnValues;
		}

		void HandleButtonXInput()
		{
			CopyMemory(&m_PrevState, &m_CurrentState, sizeof(XINPUT_STATE));
			ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
			XInputGetState(m_ControllerIdx, &m_CurrentState);

			auto ButtonChanges = m_CurrentState.Gamepad.wButtons ^ m_PrevState.Gamepad.wButtons;
			m_ThisFramePressedButtons = ButtonChanges & m_CurrentState.Gamepad.wButtons;
			m_ThisFrameReleasedButtons = ButtonChanges & (~m_CurrentState.Gamepad.wButtons);
			m_PreviousFramePressedButtons = m_ThisFramePressedButtons;

#ifdef _DEBUG
			if (m_ThisFramePressedButtons & XINPUT_GAMEPAD_A) std::cout << "A button pressed" << std::endl;
			if (m_ThisFramePressedButtons & XINPUT_GAMEPAD_B) std::cout << "B button pressed" << std::endl;
			if (m_ThisFramePressedButtons & XINPUT_GAMEPAD_X) std::cout << "X button pressed" << std::endl;
			if (m_ThisFramePressedButtons & XINPUT_GAMEPAD_Y) std::cout << "Y button pressed" << std::endl;
			if (m_ThisFramePressedButtons & XINPUT_GAMEPAD_DPAD_UP) std::cout << "DPad UP pressed" << std::endl;
			if (m_ThisFramePressedButtons & XINPUT_GAMEPAD_DPAD_DOWN) std::cout << "DPad DOWN pressed" << std::endl;
			if (m_ThisFramePressedButtons & XINPUT_GAMEPAD_DPAD_LEFT) std::cout << "DPad LEFT pressed" << std::endl;
			if (m_ThisFramePressedButtons & XINPUT_GAMEPAD_DPAD_RIGHT) std::cout << "DPad RIGHT pressed" << std::endl;
#endif
		}

		bool IsPressed(DPadButton Button) const
		{
			return m_CurrentState.Gamepad.wButtons & static_cast<int>(Button);
		}

		XINPUT_STATE m_CurrentState{};
		XINPUT_STATE m_PrevState{};
		int m_ControllerIdx{};
		int m_ThisFramePressedButtons{};
		int m_ThisFrameReleasedButtons{};
		int m_PreviousFramePressedButtons{};
		float m_DeadZonePercentage{};
	};


	PlayerControllerComponent::PlayerControllerComponent(GameObject* Owner) : BaseComponent(Owner), pImpl(std::make_unique<Impl>(Owner)) {};

	PlayerControllerComponent::~PlayerControllerComponent() = default;

	void PlayerControllerComponent::Update(const float deltaTime)
	{
		pImpl->Update(deltaTime);
	}

	Command* PlayerControllerComponent::HandleInput()
	{
		return pImpl->HandleInput();
	}

}
