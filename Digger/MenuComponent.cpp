#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <Xinput.h>
#include <SDL.h>
#include "MenuComponent.h"
#include "SceneManager.h"

void LoadSinglePlayer();
void LoadCoOp();
void LoadVersus();

namespace dae {

	MenuComponent::MenuComponent(GameObject* owner)
		: BaseComponent(owner) {
	}

	void MenuComponent::Update(float)
	{
		if (m_HasSelected)
			return;

		XINPUT_STATE state{};
		DWORD xiResult = XInputGetState(0, &state);

		const Uint8* keys = SDL_GetKeyboardState(nullptr);
		bool keyA = keys[SDL_SCANCODE_A] != 0;
		bool keyX = keys[SDL_SCANCODE_X] != 0;
		bool keyY = keys[SDL_SCANCODE_Y] != 0;

		// On first frame after scene loads, seed previous states and ignore input.
		if (m_firstUpdate) {
			if (xiResult == ERROR_SUCCESS) {
				m_PrevButtons = state.Gamepad.wButtons;
			}
			m_prevKeyA = keyA;
			m_prevKeyX = keyX;
			m_prevKeyY = keyY;
			m_firstUpdate = false;
			return; // consume carry-over input
		}

		//  Controller edge detection 
		if (xiResult == ERROR_SUCCESS) {
			uint16_t pressed = static_cast<uint16_t>(state.Gamepad.wButtons & ~m_PrevButtons);

			if (pressed & XINPUT_GAMEPAD_A) {
				LoadSinglePlayer();
				m_HasSelected = true;
			}
			else if (pressed & XINPUT_GAMEPAD_X) {
				LoadCoOp();
				m_HasSelected = true;
			}
			else if (pressed & XINPUT_GAMEPAD_Y) {
				LoadVersus();
				m_HasSelected = true;
			}

			m_PrevButtons = state.Gamepad.wButtons;
		}

		//  Keyboard edge detection 
		bool aPressed = (keyA && !m_prevKeyA);
		bool xPressed = (keyX && !m_prevKeyX);
		bool yPressed = (keyY && !m_prevKeyY);

		if (!m_HasSelected) {
			if (aPressed) {
				LoadSinglePlayer();
				m_HasSelected = true;
			}
			else if (xPressed) {
				LoadCoOp();
				m_HasSelected = true;
			}
			else if (yPressed) {
				LoadVersus();
				m_HasSelected = true;
			}
		}

		m_prevKeyA = keyA;
		m_prevKeyX = keyX;
		m_prevKeyY = keyY;
	}

} // namespace dae
