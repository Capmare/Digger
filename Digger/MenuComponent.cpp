#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>
#include <Xinput.h>
#include <SDL.h>
#include "MenuComponent.h"
#include "SceneManager.h"

void LoadSinglePlayer();
void LoadCoOp();
void LoadVersus();

namespace dae
{
	MenuComponent::MenuComponent(GameObject* owner)
		: BaseComponent(owner)
	{

	}

	void MenuComponent::Update(float)
	{
		if (m_HasSelected)
		{
			return;
		}

		XINPUT_STATE state{};
		if (XInputGetState(0, &state) == ERROR_SUCCESS)
		{
			WORD pressed = static_cast<WORD>(state.Gamepad.wButtons & ~m_PrevButtons);
			if (pressed & XINPUT_GAMEPAD_A)
			{
				//SceneManager::GetInstance().GetLastScene().RemoveAll();
				LoadSinglePlayer();
				m_HasSelected = true;
			}
			else if (pressed & XINPUT_GAMEPAD_X)
			{
				//SceneManager::GetInstance().GetLastScene().RemoveAll();
				LoadCoOp();
				m_HasSelected = true;
			}
			else if (pressed & XINPUT_GAMEPAD_Y)
			{
				//SceneManager::GetInstance().GetLastScene().RemoveAll();
				LoadVersus();
				m_HasSelected = true;
			}
			m_PrevButtons = state.Gamepad.wButtons;
		}

		const Uint8* keys = SDL_GetKeyboardState(nullptr);
		if (!m_HasSelected)
		{
			if (keys[SDL_SCANCODE_A])
			{
				//SceneManager::GetInstance().GetLastScene().RemoveAll();
				LoadSinglePlayer();
				m_HasSelected = true;
			}
			else if (keys[SDL_SCANCODE_X])
			{
				//SceneManager::GetInstance().GetLastScene().RemoveAll();
				LoadCoOp();
				m_HasSelected = true;
			}
			else if (keys[SDL_SCANCODE_Y])
			{
				//SceneManager::GetInstance().GetLastScene().RemoveAll();
				LoadVersus();
				m_HasSelected = true;
			}
		}
	}
}