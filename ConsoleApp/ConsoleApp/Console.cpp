#include "Console.hpp"

#include <string>
#include <sstream>

/* CONSTRUCTOR */
Console::Console() :
	m_hParentWindow(nullptr),
	m_hConsoleWindow(nullptr),
	m_hInputWindow(nullptr),
	m_hParentInstance(nullptr)
{
	std::wcout << L"CONSTRUCTOR: Console()" << L'\n';
}

Console::Console(HWND hParentWindow, HINSTANCE hParentInstance) :
	m_hParentWindow(hParentWindow),
	m_hConsoleWindow(nullptr),
	m_hInputWindow(nullptr),
	m_hParentInstance(hParentInstance)
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
	GetWindowTextW(m_hInputWindow, inputBuffer, sizeof(inputBuffer) / sizeof(wchar_t));

	// Build the message string
	std::wstringstream ss;
	ss << L"> " << inputBuffer << L"\r\n";

	// Append it to the output console area and move caret to the end
	Console::AppendText(ss.str());

	// Clear the input box
	SetWindowTextW(m_hInputWindow, L"");
}

void Console::Cleanup()
{
	DestroyWindow(m_hConsoleWindow);
	m_hConsoleWindow = nullptr;

	DestroyWindow(m_hInputWindow);
	m_hInputWindow = nullptr;
}