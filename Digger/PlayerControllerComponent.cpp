#include "GameCommands.h"
#include "PlayerControllerComponent.h"
#include <iostream>
#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>
#include <Xinput.h>
#include "imgui_impl_sdl2.h"
#include <SDL.h>

constexpr int m_MoveDistance{ 15 };
extern void OnAllEmeraldsCollected();

namespace dae
{
	class PlayerControllerComponent::Impl
	{
		GameObject* m_Owner{};

		std::unique_ptr<Command_Move> MoveUp;
		std::unique_ptr<Command_Move> MoveDown;
		std::unique_ptr<Command_Move> MoveRight;
		std::unique_ptr<Command_Move> MoveLeft;
													   
		std::unique_ptr<Command_Move> MoveLeftUp;
		std::unique_ptr<Command_Move> MoveRightUp;
		std::unique_ptr<Command_Move> MoveLeftDown;
		std::unique_ptr<Command_Move> MoveRightDown;

		std::unique_ptr<Command_DecreaseHealth> DecreaseHealth;
		std::unique_ptr<Command_IncreaseScore> IncreaseScore;
		std::unique_ptr<Command_StopAllSound> StopAllSound;
		std::unique_ptr<Command_Shoot> ShootLeft;
		std::unique_ptr<Command_Shoot> ShootRight;
		std::unique_ptr<Command_Shoot> ShootUp;
		std::unique_ptr<Command_Shoot> ShootDown;

		bool m_bIsSecondController{false};

		glm::vec2 m_LastDirection{};

	public:

		Impl(GameObject* Owner,int ControllerIdx) : 
			m_Owner			{ Owner },
			MoveUp			{ std::make_unique<dae::Command_Move>(glm::vec2{ 0,-m_MoveDistance }) },
			MoveDown		{ std::make_unique<dae::Command_Move>(glm::vec2{ 0,m_MoveDistance }) },
			MoveRight		{ std::make_unique<dae::Command_Move>(glm::vec2{ m_MoveDistance,0 }) },
			MoveLeft		{ std::make_unique<dae::Command_Move>(glm::vec2{ -m_MoveDistance,0 }) },

			MoveLeftUp		{ std::make_unique<dae::Command_Move>(glm::vec2{ -1,-1 }) },
			MoveRightUp		{ std::make_unique<dae::Command_Move>(glm::vec2{ 1,-1 }) },
			MoveLeftDown	{ std::make_unique<dae::Command_Move>(glm::vec2{ -1,1 }) },
			MoveRightDown	{ std::make_unique<dae::Command_Move>(glm::vec2{ 1,1 }) },
			DecreaseHealth	{ std::make_unique<dae::Command_DecreaseHealth>() },
			IncreaseScore	{ std::make_unique<dae::Command_IncreaseScore>() },
			StopAllSound	{ std::make_unique<dae::Command_StopAllSound>() },
			ShootLeft		{ std::make_unique<dae::Command_Shoot>(glm::vec2{ -1,0 })},
			ShootRight		{ std::make_unique<dae::Command_Shoot>(glm::vec2{ 1,0 })},
			ShootUp			{ std::make_unique<dae::Command_Shoot>( glm::vec2{ 0,-1 })},
			ShootDown		{ std::make_unique<dae::Command_Shoot>(glm::vec2{ 0,1 })},
			m_ControllerIdx	{ ControllerIdx }

		{};
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
			if (m_bIsSecondController == false)
			{
				// diagonal
				//if (IsDpadPressed(DPadButton::DPAD_LEFT) && IsDpadPressed(DPadButton::DPAD_UP))
				//{
				//	return MoveLeftUp.get();
				//}
				//if (IsDpadPressed(DPadButton::DPAD_RIGHT) && IsDpadPressed(DPadButton::DPAD_UP))
				//{
				//	return MoveRightUp.get();
				//}
				//if (IsDpadPressed(DPadButton::DPAD_LEFT) && IsDpadPressed(DPadButton::DPAD_DOWN))
				//{
				//	return MoveLeftDown.get();
				//}
				//if (IsDpadPressed(DPadButton::DPAD_RIGHT) && IsDpadPressed(DPadButton::DPAD_DOWN))
				//{
				//	return MoveRightDown.get();
				//}





				// Normal movement
				if (IsDpadPressed(DPadButton::DPAD_UP))
				{
					m_LastDirection = glm::vec2{ 0,-1 };
					return MoveUp.get();
				}
				if (IsDpadPressed(DPadButton::DPAD_DOWN))
				{
					m_LastDirection = glm::vec2{ 0,1 };

					return MoveDown.get();
				}
				if (IsDpadPressed(DPadButton::DPAD_LEFT))
				{
					m_LastDirection = glm::vec2{ -1,0 };

					return MoveLeft.get();
				}
				if (IsDpadPressed(DPadButton::DPAD_RIGHT))
				{
					m_LastDirection = glm::vec2{ 1,0 };
					return MoveRight.get();
				}

				if (IsGamepadPressed(GamepadButton::A))
				{
					if (m_LastDirection.x == -1)
					{
						return ShootLeft.get();
					}
					if (m_LastDirection.x == 1)
					{
						return ShootRight.get();
					}
					
				}
			
			}
			else
			{
				const Uint8* keyState = SDL_GetKeyboardState(NULL);
				
				// on pressed
				if (keyState[SDL_SCANCODE_X] && !previousKeyState[SDL_SCANCODE_X])
				{
					previousKeyState[SDL_SCANCODE_X] = true; 
					//return DecreaseHealth.get();
				}
				if (keyState[SDL_SCANCODE_C] && !previousKeyState[SDL_SCANCODE_C])
				{
					previousKeyState[SDL_SCANCODE_C] = true;
					//return IncreaseScore.get();
				}
				// on release
				if (!keyState[SDL_SCANCODE_X]) {
					previousKeyState[SDL_SCANCODE_X] = false;  
				}
				if (!keyState[SDL_SCANCODE_C]) {
					previousKeyState[SDL_SCANCODE_C] = false;  
				}

				// Normal movement
				if (keyState[SDL_SCANCODE_W]) 
				{
					m_LastDirection = glm::vec2{ 0,-1 };
					return MoveUp.get();
				}
				if (keyState[SDL_SCANCODE_S])  
				{
					m_LastDirection = glm::vec2{ 0,1 };
					return MoveDown.get();
				}
				if (keyState[SDL_SCANCODE_A])  
				{
					m_LastDirection = glm::vec2{ 1,0 };
					return MoveLeft.get();
				}
				if (keyState[SDL_SCANCODE_D])  
				{
					m_LastDirection = glm::vec2{ 1,0 };
					return MoveRight.get();
				}
				if (keyState[SDL_SCANCODE_F11])
				{
					OnAllEmeraldsCollected();
				}

				if (keyState[SDL_SCANCODE_L])
				{
					//return StopAllSound.get();
				}
				
				
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
			if (m_ThisFramePressedButtons & XINPUT_GAMEPAD_A)
			{

				std::cout << "A button pressed" << std::endl;
			}
			if (m_ThisFramePressedButtons & XINPUT_GAMEPAD_B) std::cout << "B button pressed" << std::endl;
			if (m_ThisFramePressedButtons & XINPUT_GAMEPAD_X) std::cout << "X button pressed" << std::endl;
			if (m_ThisFramePressedButtons & XINPUT_GAMEPAD_Y) std::cout << "Y button pressed" << std::endl;
			if (m_ThisFramePressedButtons & XINPUT_GAMEPAD_DPAD_UP) std::cout << "DPad UP pressed" << std::endl;
			if (m_ThisFramePressedButtons & XINPUT_GAMEPAD_DPAD_DOWN) std::cout << "DPad DOWN pressed" << std::endl;
			if (m_ThisFramePressedButtons & XINPUT_GAMEPAD_DPAD_LEFT) std::cout << "DPad LEFT pressed" << std::endl;
			if (m_ThisFramePressedButtons & XINPUT_GAMEPAD_DPAD_RIGHT) std::cout << "DPad RIGHT pressed" << std::endl;
#endif
		}

		bool IsDpadPressed(DPadButton Button) const
		{
			return m_CurrentState.Gamepad.wButtons & static_cast<int>(Button);
		}

		bool IsGamepadPressed(GamepadButton Button) const
		{
			return m_ThisFramePressedButtons & static_cast<int>(Button);

		}


		XINPUT_STATE m_CurrentState{};
		XINPUT_STATE m_PrevState{};
		int m_ControllerIdx{};
		int m_ThisFramePressedButtons{};
		int m_ThisFrameReleasedButtons{};
		int m_PreviousFramePressedButtons{};
		float m_DeadZonePercentage{};


		bool previousKeyState[SDL_NUM_SCANCODES] = { false };
	};


	PlayerControllerComponent::PlayerControllerComponent(GameObject* Owner, int ControllerIdx) : BaseComponent(Owner), pImpl(std::make_unique<Impl>(Owner,ControllerIdx)) {};

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
