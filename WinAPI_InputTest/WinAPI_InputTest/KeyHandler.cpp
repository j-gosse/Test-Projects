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

void KeyHandler::UpdateKey(bool isDown)
{
    if (m_isDown != isDown)
    {
        m_isDown = isDown;
        m_changed = true;
    }
    else
    {
        m_changed = false;
    }
}

void KeyHandler::UpdateButton(Button button)
{
    m_buttonState[button].isDown = m_isDown;
    m_buttonState[button].changed = m_changed;
}

static void WriteUnicode(const wchar_t* text)
{
    DWORD written;
    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), text, (DWORD)wcslen(text), &written, nullptr);
}

static const wchar_t* ButtonToString(Button b)
{
    switch (b)
    {
    case BUTTON_UP: return L"↑";
    case BUTTON_DOWN: return L"↓";
    case BUTTON_LEFT: return L"←";
    case BUTTON_RIGHT: return L"→";
    case BUTTON_ACTION: return L"␣";
    default: return L"UNKNOWN";
    }
}

void KeyHandler::PrintButtons() const
{
    std::wcout << L"Buttons down: ";
    for (int i = 0; i < BUTTON_COUNT; ++i)
    {
        if (m_buttonState[i].isDown)
            WriteUnicode(ButtonToString(static_cast<Button>(i)));
    }
    std::wcout << L"\n";
}