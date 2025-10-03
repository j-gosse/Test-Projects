#include "KeyHandler.hpp"

#include <iostream>
#include <sstream>

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

static const wchar_t* (*VkCodeToName)(UINT) = [](UINT vkCode) -> const wchar_t*
{
    switch (vkCode)
    {
    case VK_SHIFT:      return L"Shift";
    case VK_CONTROL:    return L"Ctrl";
    case VK_MENU:       return L"Alt";
    case VK_UP:         return L"↑";
    case VK_DOWN:       return L"↓";
    case VK_LEFT:       return L"←";
    case VK_RIGHT:      return L"→";
    case VK_RETURN:     return L"Enter";
    case VK_BACK:       return L"Backspace";
    case VK_TAB:        return L"Tab";
    case VK_ESCAPE:     return L"Escape";
    case VK_CAPITAL:    return L"CapsLock";
    case VK_SPACE:      return L"Spacebar";
    default:            return nullptr;
    }
};

void KeyHandler::Update(HWND hWnd, unsigned int vkCode, bool isDown)
{
    UpdateKey(isDown);
    UpdateKeyState(vkCode);
    UpdateDisplayText(hWnd);

    for (int i = 0; i < BUTTON_COUNT; i++)
    {
        for (unsigned int key : m_buttonBindings[i])
        {
            if (key == vkCode)
            {
                UpdateButtonState(static_cast<Button>(i));
                break;
            }
        }
    }

    PrintKeyState();
    PrintButtonState();
}

void KeyHandler::UpdateDisplayText(HWND hWnd)
{
    m_displayText.clear();

    for (size_t i = 0; i < NUM_KEYS; i++)
    {
        if (m_keyState[i])
        {
            if (const wchar_t* name = VkCodeToName(static_cast<UINT>(i)))
            {
                m_displayText += name;
                m_displayText += L' ';
            }
            else if ((i >= 'A' && i <= 'Z') || (i >= '0' && i <= '9'))
            {
                m_displayText += static_cast<wchar_t>(i);
                m_displayText += L' ';
            }
            else
            {
                m_displayText += std::wstring(L"[VK") + std::to_wstring(i) + L"] ";
            }
        }
    }

    InvalidateRect(hWnd, nullptr, TRUE); // redraw window
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

void KeyHandler::UpdateKeyState(unsigned int vkCode)
{
    if (vkCode >= NUM_KEYS)
        return;

    m_keyState[vkCode] = m_isDown;
}

void KeyHandler::UpdateButtonState(Button button)
{
    m_buttonState[button].isDown = m_isDown;
    m_buttonState[button].changed = m_changed;
}

void KeyHandler::PrintButtonState() const
{
    std::wcout << L"Buttons down: ";
    for (int i = 0; i < BUTTON_COUNT; ++i)
    {
        if (m_buttonState[i].isDown)
        {
            WriteUnicode(ButtonToString(static_cast<Button>(i)));
            std::wcout << L' ';
        }
    }
    std::wcout << L"\n";
}

void KeyHandler::PrintKeyState() const
{
    std::wcout << L"Keys down:";

    for (size_t i = 0; i < NUM_KEYS; ++i)
    {
        if (m_keyState[i])
        {
            if (const wchar_t* name = VkCodeToName(static_cast<UINT>(i)))
            {
                WriteUnicode(name);
                std::wcout << L' ';
            }
            else if ((i >= 'A' && i <= 'Z') || (i >= '0' && i <= '9'))
            {
                std::wcout << L"'" << static_cast<wchar_t>(i) << L"'";
            }
            else
            {
                std::wcout << L" [" << i << L']';
            }
        }
    }
    std::wcout << L'\n';
}

void KeyHandler::ResetKeyState()
{
    m_keyState.reset();
    m_displayText.clear();
}

void KeyHandler::DrawKeys(HWND hWnd)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);

    // customize font
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

    // format text
    SetTextColor(hdc, RGB(0, 0, 0));
    SetBkMode(hdc, TRANSPARENT);

    // draw text to window
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

    // cleanup
    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);
    EndPaint(hWnd, &ps);
    ReleaseDC(hWnd, hdc);
}