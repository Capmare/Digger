// PlayerControllerComponent.cpp
#include "GameCommands.h"
#include "PlayerControllerComponent.h"

#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>
#include <Xinput.h>

#include <SDL.h>
#include <iostream>

constexpr int m_MoveDistance{ 15 };
extern void OnAllEmeraldsCollected();


namespace {
	bool g_waitF1Release = false;
}

namespace dae
{
	class PlayerControllerComponent::Impl
	{
	public:
		Impl(GameObject* owner, int controllerIdx)
			: m_Owner{ owner }
			, MoveUp{ std::make_unique<dae::Command_Move>(glm::vec2{ 0, -m_MoveDistance }) }
			, MoveDown{ std::make_unique<dae::Command_Move>(glm::vec2{ 0,  m_MoveDistance }) }
			, MoveRight{ std::make_unique<dae::Command_Move>(glm::vec2{  m_MoveDistance, 0 }) }
			, MoveLeft{ std::make_unique<dae::Command_Move>(glm::vec2{ -m_MoveDistance, 0 }) }
			, MoveLeftUp{ std::make_unique<dae::Command_Move>(glm::vec2{ -1, -1 }) }
			, MoveRightUp{ std::make_unique<dae::Command_Move>(glm::vec2{  1, -1 }) }
			, MoveLeftDown{ std::make_unique<dae::Command_Move>(glm::vec2{ -1,  1 }) }
			, MoveRightDown{ std::make_unique<dae::Command_Move>(glm::vec2{  1,  1 }) }
			, DecreaseHealth{ std::make_unique<dae::Command_DecreaseHealth>() }
			, IncreaseScore{ std::make_unique<dae::Command_IncreaseScore>() }
			, StopAllSound{ std::make_unique<dae::Command_StopAllSound>() }
			, ShootLeft{ std::make_unique<dae::Command_Shoot>(glm::vec2{ -1, 0 }) }
			, ShootRight{ std::make_unique<dae::Command_Shoot>(glm::vec2{  1, 0 }) }
			, ShootUp{ std::make_unique<dae::Command_Shoot>(glm::vec2{  0,-1 }) }
			, ShootDown{ std::make_unique<dae::Command_Shoot>(glm::vec2{  0, 1 }) }
			, m_ControllerIdx{ controllerIdx }
		{
			m_bIsSecondController = (m_ControllerIdx == 1);
			m_DeadZonePercentage = 0.24f; 
		}

		void Update(const float deltaTime)
		{
			HandleButtonXInput();
			ThumbInputReturn thumb = HandleThumbXInput(); (void)thumb;

			if (m_LevelAdvanceCooldown > 0.f)
				m_LevelAdvanceCooldown -= deltaTime;

			std::cout << m_LevelAdvanceCooldown << std::endl;

			if (Command* cmd = HandleInput())
				cmd->Exec(*m_Owner);
		}

		Command* HandleInput()
		{
			const Uint8* key = SDL_GetKeyboardState(nullptr);
			const bool f1 = key[SDL_SCANCODE_F1] != 0; 

			if (g_waitF1Release)
			{
				if (!f1) g_waitF1Release = false;  
				m_prevF1 = f1;                     
			}
			else
			{
				if (f1 && !m_prevF1 && m_LevelAdvanceCooldown <= 0.f)
				{
					OnAllEmeraldsCollected();
					m_LevelAdvanceCooldown = 0.35f;
					g_waitF1Release = true;        
				}
				m_prevF1 = f1;
			}

			if (!m_bIsSecondController)
			{

				if (IsDpadPressed(DPadButton::DPAD_UP)) { m_LastDirection = { 0,-1 }; return MoveUp.get(); }
				if (IsDpadPressed(DPadButton::DPAD_DOWN)) { m_LastDirection = { 0, 1 }; return MoveDown.get(); }
				if (IsDpadPressed(DPadButton::DPAD_LEFT)) { m_LastDirection = { -1, 0 }; return MoveLeft.get(); }
				if (IsDpadPressed(DPadButton::DPAD_RIGHT)) { m_LastDirection = { 1, 0 }; return MoveRight.get(); }

				if (IsGamepadPressed(GamepadButton::A))
				{
					if (m_LastDirection.y == 0)
					{
						if (m_LastDirection.x < 0) return ShootLeft.get();
						if (m_LastDirection.x > 0) return ShootRight.get();
					}
					else if (m_LastDirection.x == 0)
					{
						if (m_LastDirection.y < 0) return ShootUp.get();
						if (m_LastDirection.y > 0) return ShootDown.get();
					}
				}


			}
			else
			{
				SDL_PumpEvents();

				if (key[SDL_SCANCODE_X] && !previousKeyState[SDL_SCANCODE_X]) previousKeyState[SDL_SCANCODE_X] = true;
				if (!key[SDL_SCANCODE_X]) previousKeyState[SDL_SCANCODE_X] = false;

				if (key[SDL_SCANCODE_C] && !previousKeyState[SDL_SCANCODE_C]) previousKeyState[SDL_SCANCODE_C] = true;
				if (!key[SDL_SCANCODE_C]) previousKeyState[SDL_SCANCODE_C] = false;

				if (key[SDL_SCANCODE_W]) { m_LastDirection = { 0,-1 }; return MoveUp.get(); }
				if (key[SDL_SCANCODE_S]) { m_LastDirection = { 0, 1 }; return MoveDown.get(); }
				if (key[SDL_SCANCODE_A]) { m_LastDirection = { -1, 0 }; return MoveLeft.get(); } 
				if (key[SDL_SCANCODE_D]) { m_LastDirection = { 1, 0 }; return MoveRight.get(); }


				if (key[SDL_SCANCODE_SPACE])
				{
					if (m_LastDirection.y == 0)
					{
						if (m_LastDirection.x < 0) return ShootLeft.get();
						if (m_LastDirection.x > 0) return ShootRight.get();
					}
					else if (m_LastDirection.x == 0)
					{
						if (m_LastDirection.y < 0) return ShootUp.get();
						if (m_LastDirection.y > 0) return ShootDown.get();
					}
				}
			}
			return nullptr;
		}

		struct ThumbInputReturn
		{
			short ThumbLX{};
			short ThumbRX{};
			short ShoulderLY{};
			short ShoulderRY{};
		};

		ThumbInputReturn HandleThumbXInput()
		{
			ThumbInputReturn r{};
			const auto Lx = m_CurrentState.Gamepad.sThumbLX;
			const auto Rx = m_CurrentState.Gamepad.sThumbRX;
			r.ShoulderLY = m_CurrentState.Gamepad.sThumbLY;
			r.ShoulderRY = m_CurrentState.Gamepad.sThumbRY;

			if (std::abs(Lx) > SHRT_MAX * m_DeadZonePercentage) r.ThumbLX = Lx;
			if (std::abs(Rx) > SHRT_MAX * m_DeadZonePercentage) r.ThumbRX = Rx;
			return r;
		}

		void HandleButtonXInput()
		{
			CopyMemory(&m_PrevState, &m_CurrentState, sizeof(XINPUT_STATE));
			ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
			XInputGetState(m_ControllerIdx, &m_CurrentState);

			const auto changes = m_CurrentState.Gamepad.wButtons ^ m_PrevState.Gamepad.wButtons;
			m_ThisFramePressedButtons = changes & m_CurrentState.Gamepad.wButtons;
			m_ThisFrameReleasedButtons = changes & (~m_CurrentState.Gamepad.wButtons);

#ifdef _DEBUG
			if (m_ThisFramePressedButtons & XINPUT_GAMEPAD_A)           std::cout << "P" << m_ControllerIdx << " A\n";
			if (m_ThisFramePressedButtons & XINPUT_GAMEPAD_B)           std::cout << "P" << m_ControllerIdx << " B\n";
			if (m_ThisFramePressedButtons & XINPUT_GAMEPAD_X)           std::cout << "P" << m_ControllerIdx << " X\n";
			if (m_ThisFramePressedButtons & XINPUT_GAMEPAD_Y)           std::cout << "P" << m_ControllerIdx << " Y\n";
			if (m_ThisFramePressedButtons & XINPUT_GAMEPAD_DPAD_UP)     std::cout << "P" << m_ControllerIdx << " UP\n";
			if (m_ThisFramePressedButtons & XINPUT_GAMEPAD_DPAD_DOWN)   std::cout << "P" << m_ControllerIdx << " DOWN\n";
			if (m_ThisFramePressedButtons & XINPUT_GAMEPAD_DPAD_LEFT)   std::cout << "P" << m_ControllerIdx << " LEFT\n";
			if (m_ThisFramePressedButtons & XINPUT_GAMEPAD_DPAD_RIGHT)  std::cout << "P" << m_ControllerIdx << " RIGHT\n";
#endif
		}

		bool IsDpadPressed(DPadButton b) const
		{
			return (m_CurrentState.Gamepad.wButtons & static_cast<int>(b)) != 0;
		}
		bool IsGamepadPressed(GamepadButton b) const
		{
			return (m_ThisFramePressedButtons & static_cast<int>(b)) != 0;
		}

	private:
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
		std::unique_ptr<Command_IncreaseScore>  IncreaseScore;
		std::unique_ptr<Command_StopAllSound>   StopAllSound;

		std::unique_ptr<Command_Shoot> ShootLeft;
		std::unique_ptr<Command_Shoot> ShootRight;
		std::unique_ptr<Command_Shoot> ShootUp;
		std::unique_ptr<Command_Shoot> ShootDown;

		bool m_bIsSecondController{ false };
		glm::vec2 m_LastDirection{ 1, 0 }; 

		XINPUT_STATE m_CurrentState{};
		XINPUT_STATE m_PrevState{};
		int   m_ControllerIdx{};
		int   m_ThisFramePressedButtons{};
		int   m_ThisFrameReleasedButtons{};
		float m_DeadZonePercentage{ 0.24f };


		bool  m_prevF1{ false };
		float m_LevelAdvanceCooldown{ 0.f };

		bool previousKeyState[SDL_NUM_SCANCODES]{}; 
	};


	PlayerControllerComponent::PlayerControllerComponent(GameObject* owner, int controllerIdx)
		: BaseComponent(owner)
		, pImpl(std::make_unique<Impl>(owner, controllerIdx))
	{
	}

	PlayerControllerComponent::~PlayerControllerComponent() = default;

	void PlayerControllerComponent::Update(const float dt)
	{
		pImpl->Update(dt);
	}

	Command* PlayerControllerComponent::HandleInput()
	{
		return pImpl->HandleInput();
	}
} 
