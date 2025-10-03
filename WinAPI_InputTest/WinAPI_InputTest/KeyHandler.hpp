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

    static constexpr const size_t NUM_KEYS = 256;
    std::bitset<NUM_KEYS> m_keyState{};

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
    void UpdateKeyState(unsigned int vkCode);
    void UpdateButtonState(Button button);
    void UpdateDisplayText(HWND hWnd);

    void PrintKeyState() const;
    void PrintButtonState() const;

public:
    KeyHandler();
    virtual ~KeyHandler();

    void Update(HWND hWnd, unsigned int vkCode, bool isDown);
    void DrawKeys(HWND hWnd);
    void ResetKeyState();

    const bool IsKeyDown(unsigned int vkCode) const { return m_keyState[vkCode]; }
    const bool IsButtonDown(Button button) const { return m_buttonState[button].isDown; }
    const bool IsButtonChanged(Button button) const { return m_buttonState[button].changed; }
};

#endif