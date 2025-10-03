#pragma once

#ifndef KEYHANDLER_HPP_
#define KEYHANDLER_HPP_

#include <windows.h>
#include <bitset>
#include <vector>

enum Button
{
    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_LEFT,
    BUTTON_RIGHT,
    BUTTON_ACTION,

    BUTTON_COUNT
};

struct ButtonState
{
    bool isDown = false;
    bool changed = false;
};

class KeyHandler
{
private:
    bool m_isDown = false;
    bool m_changed = false;

    ButtonState m_buttonState[BUTTON_COUNT];
    std::vector<unsigned int> m_buttonBindings[BUTTON_COUNT] =
    {
        { 'W', VK_UP },      // BUTTON_UP
        { 'S', VK_DOWN },    // BUTTON_DOWN
        { 'A', VK_LEFT },    // BUTTON_LEFT
        { 'D', VK_RIGHT },   // BUTTON_RIGHT
        { VK_SPACE }         // BUTTON_ACTION
    };

    std::wstring m_displayText;

    void UpdateKey(bool isDown);
    void UpdateButton(Button button);
    void PrintButtons() const;

public:
    KeyHandler();
    virtual ~KeyHandler();

    void Update(unsigned int vkCode, bool isDown);
    void UpdateDisplayText(HWND hWnd);
    void PaintKeys(HWND hWnd);

    const bool IsKeyDown() const { return m_isDown; }
    const bool IsKeyChanged() const { return m_changed; }
    const bool IsButtonDown(Button button) const { return m_buttonState[button].isDown; }
    const bool IsButtonChanged(Button button) const { return m_buttonState[button].changed; }
};

#endif