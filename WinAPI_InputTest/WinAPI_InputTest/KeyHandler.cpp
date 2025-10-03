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

static void WriteUnicode(const wchar_t* text)
{
    DWORD written;
    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), text, (DWORD)wcslen(text), &written, nullptr);
}

static const wchar_t* ButtonToString(Button b)
{
    switch (b)
    {
    case BUTTON_UP:
        return L"↑";
    case BUTTON_DOWN:
        return L"↓";
    case BUTTON_LEFT:
        return L"←";
    case BUTTON_RIGHT:
        return L"→";
    case BUTTON_ACTION:
        return L"␣";
    default:
        return L"UNKNOWN";
    }
}

void KeyHandler::Update(unsigned int vkCode, bool isDown)
{
    for (int i = 0; i < BUTTON_COUNT; i++)
    {
        for (unsigned int key : m_buttonBindings[i])
        {
            if (key == vkCode)
            {
                UpdateKey(isDown);
                UpdateButton(static_cast<Button>(i));
                PrintButtons();
                //std::wcout << L"vkCode: " << vkCode << L'\n';
                //std::wcout << L"isKeyDown: " << m_keyHandler.IsKeyDown() << L'\n';
                //std::wcout << L"isKeyChanged: " << m_keyHandler.IsKeyChanged() << L'\n';
                //std::wcout << L"isButtonDown: " << m_keyHandler.IsButtonDown(BUTTON_UP) << L'\n';
                //std::wcout << L"isButtonChanged: " << m_keyHandler.IsButtonChanged(BUTTON_UP) << L'\n';
                break;
            }
        }
    }
}

void KeyHandler::UpdateDisplayText(HWND hWnd)
{
    m_displayText.clear();
    for (int i = 0; i < BUTTON_COUNT; ++i)
    {
        if (m_buttonState[i].isDown)
        {
            m_displayText += ButtonToString(static_cast<Button>(i));
        }
    }
    InvalidateRect(hWnd, nullptr, TRUE);
}

void KeyHandler::PaintKeys(HWND hWnd)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);

    // create a larger font
    HFONT hFont = CreateFontW
    (
        -48,                    // Height (negative for point size)
        0, 0, 0,                // Width, Escapement, Orientation
        FW_NORMAL,              // Weight
        FALSE, FALSE, FALSE,    // Italic, Underline, Strikeout
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        L"Segoe UI"             // Font face name
    );
    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

    // text formatting
    SetTextColor(hdc, RGB(0, 0, 0));
    SetBkMode(hdc, TRANSPARENT);

    // center the text
    RECT rect;
    GetClientRect(hWnd, &rect);
    DrawTextW
    (
        hdc,
        m_displayText.c_str(),
        -1,
        &rect,
        DT_CENTER | DT_VCENTER | DT_SINGLELINE
    );

    // clean up font resources
    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);
    EndPaint(hWnd, &ps);
}

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