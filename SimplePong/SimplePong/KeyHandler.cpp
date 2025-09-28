#include "KeyHandler.hpp"

#include <iostream>

/* CONSTRUCTOR */

KeyHandler::KeyHandler()
{
	std::wcout << L"CONSTRUCTOR: KeyHandler()" << L'\n';
}

/* DESTRUCTOR */

KeyHandler::~KeyHandler()
{
	std::wcout << L"DESTRUCTOR: ~KeyHandler()" << L'\n';
}

/* FUNCTION DEFINITIONS */

void KeyHandler::OnKeyDown(WPARAM key)
{
	m_keyState[key] = true;
}

void KeyHandler::OnKeyUp(WPARAM key)
{
	m_keyState[key] = false;
}

bool KeyHandler::IsDown(WPARAM key) const
{
	std::unordered_map<WPARAM, bool>::const_iterator it = m_keyState.find(key);
	return it != m_keyState.end() && it->second;
}

bool KeyHandler::IsUp(WPARAM key) const
{
	std::unordered_map<WPARAM, bool>::const_iterator it = m_keyState.find(key);
	return it == m_keyState.end() || !it->second;
}

void KeyHandler::UpdateButtonState()
{
    m_buttonState[BUTTON_UP] = IsDown(VK_UP) || IsDown('W');
    m_buttonState[BUTTON_DOWN] = IsDown(VK_DOWN) || IsDown('S');
    m_buttonState[BUTTON_LEFT] = IsDown(VK_LEFT) || IsDown('A');
    m_buttonState[BUTTON_RIGHT] = IsDown(VK_RIGHT) || IsDown('D');
	m_buttonState[BUTTON_ACTION] = IsDown(VK_SPACE);
}

bool KeyHandler::IsButtonDown(int button) const
{
    if (button < 0 || button >= BUTTON_COUNT) return false;
    return m_buttonState[button];
}