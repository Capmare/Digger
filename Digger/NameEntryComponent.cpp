// NameEntryComponent.cpp
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>   // include first so the SDK sees the target architecture
#include <Xinput.h>
#include <SDL.h>

#include "NameEntryComponent.h"
#include "TextComponent.h"

using namespace dae;


NameEntryComponent::NameEntryComponent(GameObject* owner,
	TextComponent* text,
	int padIndex,
	std::function<void(const std::string&)> onDone)
	: BaseComponent(owner), m_Text(text), m_PadIndex(padIndex), m_OnDone(std::move(onDone)) {
	RefreshText();
}

void NameEntryComponent::RefreshText() {
	// Show candidate in brackets; underscore if empty
	std::string view = "NAME: ";
	view += m_Name;
	view += '[';
	view += m_Candidate;
	view += ']';
	if (m_Name.empty()) view += "  (_ to start)";
	if (m_Text) m_Text->SetText(view);
}

void NameEntryComponent::BumpCandidate(int delta) {
	// rotate through ALPHABET
	const std::string chars = ALPHABET;
	auto pos = chars.find(m_Candidate);
	if (pos == std::string::npos) pos = 0;
	int i = static_cast<int>(pos) + delta;
	while (i < 0) i += static_cast<int>(chars.size());
	i %= static_cast<int>(chars.size());
	m_Candidate = chars[static_cast<size_t>(i)];
	RefreshText();
}

void NameEntryComponent::Update(float) {
	// --- Controller ---
	XINPUT_STATE state{};
	if (XInputGetState(m_PadIndex, &state) == ERROR_SUCCESS) {
		WORD pressed = static_cast<WORD>(state.Gamepad.wButtons & ~m_PrevButtons);

		if (pressed & XINPUT_GAMEPAD_DPAD_UP)    BumpCandidate(+1);
		if (pressed & XINPUT_GAMEPAD_DPAD_DOWN)  BumpCandidate(-1);
		if (pressed & XINPUT_GAMEPAD_DPAD_RIGHT) BumpCandidate(+5);
		if (pressed & XINPUT_GAMEPAD_DPAD_LEFT)  BumpCandidate(-5);

		if (pressed & XINPUT_GAMEPAD_A) { // accept current candidate
			if (static_cast<int>(m_Name.size()) < MAX_LEN) {
				m_Name.push_back(m_Candidate);
				RefreshText();
			}
		}

		if (pressed & (XINPUT_GAMEPAD_X | XINPUT_GAMEPAD_B)) { // backspace
			if (!m_Name.empty()) {
				m_Name.pop_back();
				RefreshText();
			}
		}

		if ((pressed & XINPUT_GAMEPAD_Y) || (pressed & XINPUT_GAMEPAD_START)) { // confirm
			if (!m_Name.empty() && m_OnDone) m_OnDone(m_Name);
		}

		m_PrevButtons = state.Gamepad.wButtons;
	}

	// Keyboard fallback 
	const Uint8* keys = SDL_GetKeyboardState(nullptr);

	static bool prevUp = false, prevDown = false, prevLeft = false, prevRight = false, prevRet = false, prevBack = false, prevY = false;
	bool up = keys[SDL_SCANCODE_UP], down = keys[SDL_SCANCODE_DOWN], left = keys[SDL_SCANCODE_LEFT], right = keys[SDL_SCANCODE_RIGHT];
	bool ret = keys[SDL_SCANCODE_RETURN], back = keys[SDL_SCANCODE_BACKSPACE], y = keys[SDL_SCANCODE_Y];

	if (up && !prevUp)       BumpCandidate(+1);
	if (down && !prevDown)   BumpCandidate(-1);
	if (right && !prevRight) BumpCandidate(+5);
	if (left && !prevLeft)   BumpCandidate(-5);

	if (ret && !prevRet) { // accept
		if (static_cast<int>(m_Name.size()) < MAX_LEN) {
			m_Name.push_back(m_Candidate);
			RefreshText();
		}
	}
	if (back && !prevBack) { // backspace
		if (!m_Name.empty()) {
			m_Name.pop_back();
			RefreshText();
		}
	}
	if (y && !prevY) { // confirm
		if (!m_Name.empty() && m_OnDone) m_OnDone(m_Name);
	}

	prevUp = up; prevDown = down; prevLeft = left; prevRight = right; prevRet = ret; prevBack = back; prevY = y;
}
