#pragma once

#ifndef KEYHANDLER_HPP_
#define KEYHANDLER_HPP_

#include <windows.h>
#include <unordered_map>

enum
{
    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_LEFT,
    BUTTON_RIGHT,
    BUTTON_ACTION,

    BUTTON_COUNT
};

class KeyHandler
{
private:
	std::unordered_map<WPARAM, bool> m_keyState;
    bool m_buttonState[BUTTON_COUNT] = { false };

public:
    KeyHandler();
    virtual ~KeyHandler();

    void OnKeyDown(WPARAM key);
    void OnKeyUp(WPARAM key);
    bool IsDown(WPARAM key) const;
    bool IsUp(WPARAM key) const;

    void UpdateButtonState();
    bool IsButtonDown(int button) const;
};

#endif