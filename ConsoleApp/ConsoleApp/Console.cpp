// DefSubclassProc, SetWindowSubclass, RemoveWindowSubclass, etc. are implemented 
// in comctl32.dll and declared in <commctrl.h>, must add comctl32.lib 
// to Linker -> Input -> Additional Dependencies

#include "Console.hpp"
#include <commctrl.h>

/* CONSTRUCTORS */

Console::Console() :
	m_hParentWindow(nullptr),
	m_hParentInstance(nullptr),
	m_hConsoleWindow(nullptr),
	m_hInputWindow(nullptr)
{
	std::wcout << L"CONSTRUCTOR: Console()" << L'\n';
}

Console::Console(HWND hParentWindow, HINSTANCE hParentInstance) :
	m_hParentWindow(hParentWindow),
	m_hParentInstance(hParentInstance),
	m_hConsoleWindow(nullptr),
	m_hInputWindow(nullptr)
{
	std::wcout << L"CONSTRUCTOR: Console(HWND hParentWindow, HINSTANCE hParentInstance)" << L'\n';

	// "EDIT" is a built-in Windows control class.
	// It comes with all the necessary behavior for editor control (text rendering, scrolling, caret management, etc.)
	m_hConsoleWindow = CreateWindowExW(
		WS_EX_CLIENTEDGE,
		L"EDIT",
		L"",
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
		10,
		10,
		780,
		550,
		m_hParentWindow,
		(HMENU)ID_EDIT_OUTPUT,
		m_hParentInstance,
		nullptr
	);

	if (m_hConsoleWindow == nullptr)
	{
		throw std::runtime_error("Failed to create the console window!");
	}

	SetWindowSubclass(m_hConsoleWindow, Console::ConsoleProc, 1, (DWORD_PTR)this);

	m_hInputWindow = CreateWindowExW(
		WS_EX_CLIENTEDGE,
		L"EDIT",
		L"",
		WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
		10,
		565,
		680,
		25,
		m_hParentWindow,
		(HMENU)ID_EDIT_INPUT,
		m_hParentInstance,
		nullptr
	);

	if (m_hInputWindow == nullptr)
	{
		throw std::runtime_error("Failed to create the input window!");
	}

	SetWindowSubclass(m_hInputWindow, Console::InputProc, 1, (DWORD_PTR)this);

	CreateWindowW(
		L"BUTTON",
		L"Send",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		700,
		565,
		90,
		25,
		m_hParentWindow,
		(HMENU)ID_BUTTON_SEND,
		m_hParentInstance,
		nullptr
	);

	SetWindowTextW(m_hConsoleWindow, L"Console initialized...\r\n");
}

/* DESTRUCTOR */

Console::~Console()
{
	std::wcout << L"DESTRUCTOR: ~Console()" << L'\n';
	Console::Cleanup();
}

/* FUNCTION DEFINITIONS */

LRESULT CALLBACK Console::ConsoleProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	Console* pThis = reinterpret_cast<Console*>(dwRefData);
	return pThis->HandleConsoleMessages(hWnd, uMsg, wParam, lParam);
}

LRESULT Console::HandleConsoleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const
{
	switch (uMsg)
	{
	default:
		return DefSubclassProc(hWnd, uMsg, wParam, lParam);
	}
}

LRESULT CALLBACK Console::InputProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	Console* pThis = reinterpret_cast<Console*>(dwRefData);
	return pThis->HandleInputMessages(hWnd, uMsg, wParam, lParam);
}

LRESULT Console::HandleInputMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
		std::wcout << L"CASE: WM_KEYDOWN" << '\n';
		if (wParam == VK_RETURN)
		{
			std::wcout << L"CASE: VK_RETURN" << '\n';
			Console::SendText();
			return 0;
		}
		return 0;
	case WM_CHAR:
		if (wParam == VK_RETURN)
		{
			// eat enter to prevent ding
			return 0;
		}
		return DefSubclassProc(hWnd, uMsg, wParam, lParam);
	default:
		return DefSubclassProc(hWnd, uMsg, wParam, lParam);
	}
}

void Console::AppendText(const std::wstring& text) const
{
	int len = GetWindowTextLengthW(m_hConsoleWindow);
	SendMessageW(m_hConsoleWindow, EM_SETSEL, len, len);
	SendMessageW(m_hConsoleWindow, EM_REPLACESEL, FALSE, (LPARAM)text.c_str());
}

void Console::SendText() const
{
	// Read user input from input box
	wchar_t inputBuffer[256] = {};
	if (!GetWindowTextW(m_hInputWindow, inputBuffer, sizeof(inputBuffer) / sizeof(wchar_t))) return;

	// Build the message string
	std::wstringstream ss;
	ss << L"> " << inputBuffer << L"\r\n";

	// Append it to the output console area and move caret to the end
	Console::AppendText(ss.str());

	// Clear the input box
	SetWindowTextW(m_hInputWindow, L"");
}

//	SetBkColor(hdcEdit, RGB(255, 255, 255));	// Optional: Text background color
//	SetTextColor(hdcEdit, RGB(0, 0, 0));		// Optional: Text color

INT_PTR Console::SetConsoleColor(HDC hdcEdit) const
{
	//HBRUSH hBrush = CreateSolidBrush(RGB(240, 240, 255)); // Background color: light blue
	HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));			// Background color: black

	SetBkColor(hdcEdit, RGB(0, 0, 0));		// Text background color: black
	//SetBkColor(hdcEdit, RGB(30, 30, 30));	// Text background color: dark grey
	SetTextColor(hdcEdit, RGB(0, 255, 0));	// Text color: Bright green "console-style"

	return (INT_PTR)hBrush;
}

INT_PTR Console::SetInputColor(HDC hdcEdit) const
{
	//HBRUSH hBrush = CreateSolidBrush(RGB(240, 240, 255)); // Background color: light blue
	HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));			// Background color: black

	SetBkColor(hdcEdit, RGB(0, 0, 0));		// Text background color: black
	//SetBkColor(hdcEdit, RGB(30, 30, 30));	// Text background color: dark grey
	SetTextColor(hdcEdit, RGB(0, 255, 0));	// Text color: Bright green "console-style"

	return (INT_PTR)hBrush;
}

void Console::Cleanup()
{
	RemoveWindowSubclass(m_hConsoleWindow, Console::ConsoleProc, 1);
	RemoveWindowSubclass(m_hInputWindow, Console::InputProc, 1);

	DestroyWindow(m_hConsoleWindow);
	m_hConsoleWindow = nullptr;

	DestroyWindow(m_hInputWindow);
	m_hInputWindow = nullptr;
}