// DefSubclassProc, SetWindowSubclass, RemoveWindowSubclass, etc. are implemented in 
// comctl32.dll and declared in <commctrl.h>, must add comctl32.lib 
// to Linker -> Input -> Additional Dependencies

#include "Console.hpp"
#include "Logger.hpp"

#include <richedit.h>
#include <commctrl.h>

/* CONSTRUCTORS */

Console::Console(LONG parentWindowWidth, LONG parentWindowHeight) :
	m_hParentWindow(nullptr),
	m_hParentInstance(GetModuleHandleW(nullptr)),
	m_hConsoleWindow(nullptr),
	m_hInputWindow(nullptr),
	m_hSendButton(nullptr),
	m_parentWindowWidth(parentWindowWidth),
	m_parentWindowHeight(parentWindowHeight)
{
	std::wcout << L"CONSTRUCTOR: Console(LONG parentWindowWidth, LONG parentWindowHeight)" << L'\n';
}

Console::Console(HWND hParentWindow, HINSTANCE hParentInstance, LONG parentWindowWidth, LONG parentWindowHeight) :
	m_hParentWindow(hParentWindow),
	m_hParentInstance(hParentInstance),
	m_hConsoleWindow(nullptr),
	m_hInputWindow(nullptr),
	m_hSendButton(nullptr),
	m_parentWindowWidth(parentWindowWidth),
	m_parentWindowHeight(parentWindowHeight)
{
	std::wcout << L"CONSTRUCTOR: Console(HWND hParentWindow, HINSTANCE hParentInstance, LONG parentWindowWidth, LONG parentWindowHeight)" << L'\n';

	DWORD dwExStyle = 0;

	int MARGIN = 10;
	int INPUT_HEIGHT = 25;
	int BUTTON_WIDTH = 90;
	int BUTTON_HEIGHT = 25;

	// console window
	int xPosConsole = MARGIN;
	int yPosConsole = MARGIN;
	int consoleWidth = parentWindowWidth - 2 * MARGIN;
	int consoleHeight = parentWindowHeight - INPUT_HEIGHT - 3 * MARGIN;

	// input window
	int xPosInput = MARGIN;
	int yPosInput = yPosConsole + consoleHeight + MARGIN;
	int inputWidth = parentWindowWidth - 3 * MARGIN - BUTTON_WIDTH;

	// send button
	int xPosSendBtn = xPosInput + inputWidth + MARGIN;
	int yPosSendBtn = yPosInput;

	// load the RichEdit library
	LoadLibrary(L"Msftedit.dll");

	// for Microsoft RichEdit 4.1 (Msftedit.dll), specify MSFTEDIT_CLASS as the window class
	m_hConsoleWindow = CreateWindowExW(
		WS_EX_CLIENTEDGE,
		MSFTEDIT_CLASS,
		L"",
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_NOHIDESEL | ES_READONLY,
		xPosConsole,
		yPosConsole,
		consoleWidth,
		consoleHeight,
		m_hParentWindow,
		(HMENU)ID_CONSOLE_OUTPUT,
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
		MSFTEDIT_CLASS,
		L"",
		WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
		xPosInput,
		yPosInput,
		inputWidth,
		INPUT_HEIGHT,
		m_hParentWindow,
		(HMENU)ID_CONSOLE_INPUT,
		m_hParentInstance,
		nullptr
	);

	if (m_hInputWindow == nullptr)
	{
		throw std::runtime_error("Failed to create the input window!");
	}

	SetWindowSubclass(m_hInputWindow, Console::InputProc, 1, (DWORD_PTR)this);

	m_hSendButton = CreateWindowExW(
		dwExStyle,
		L"BUTTON",
		L"Send",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		xPosSendBtn,
		yPosSendBtn,
		BUTTON_WIDTH,
		BUTTON_HEIGHT,
		m_hParentWindow,
		(HMENU)ID_BUTTON_SEND,
		m_hParentInstance,
		nullptr
	);

	if (m_hSendButton == nullptr)
	{
		throw std::runtime_error("Failed to create the send button!");
	}

	SetWindowSubclass(m_hSendButton, Console::InputProc, 1, (DWORD_PTR)this);

	Console::SetConsoleColor();
	Console::SetInputColor();
	SetFocus(m_hInputWindow);

	LOG_MSG(m_hConsoleWindow, L"Console initialized...");
	LOG_ERR(m_hConsoleWindow, L"This is a test error message!");
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
		std::wcout << L"CASE: WM_KEYDOWN" << L'\n';
		if (wParam == VK_RETURN)
		{
			std::wcout << L"CASE: VK_RETURN" << L'\n';
			Console::SendInput();
			return 0;
		}
		return DefSubclassProc(hWnd, uMsg, wParam, lParam);
	case WM_CHAR:
		if (wParam == VK_RETURN)
		{
			// eat enter to prevent ding
			return 0;
		}
		return DefSubclassProc(hWnd, uMsg, wParam, lParam);
	case WM_LBUTTONDOWN:
	{
		std::wcout << L"CASE: WM_LBUTTONDOWN" << L'\n';
		INT id = GetDlgCtrlID(hWnd);
		if (id == ID_BUTTON_SEND) Console::SendInput();
		return DefSubclassProc(hWnd, uMsg, wParam, lParam);
	}
	case WM_LBUTTONUP:
		std::wcout << L"CASE: WM_LBUTTONUP" << L'\n';
		return DefSubclassProc(hWnd, uMsg, wParam, lParam);
	default:
		return DefSubclassProc(hWnd, uMsg, wParam, lParam);
	}
}

void Console::SendInput() const
{
	LOG_SEND(m_hInputWindow, m_hConsoleWindow);
}

void Console::ResizeWindows(WPARAM wParam, LPARAM lParam)
{
	m_parentWindowWidth = LOWORD(lParam);
	m_parentWindowHeight = HIWORD(lParam);

	int MARGIN = 10;
	int INPUT_HEIGHT = 25;
	int BUTTON_WIDTH = 90;
	int BUTTON_HEIGHT = 25;

	// console window
	int xPosConsole = MARGIN;
	int yPosConsole = MARGIN;
	int consoleWidth = m_parentWindowWidth - 2 * MARGIN;
	int consoleHeight = m_parentWindowHeight - INPUT_HEIGHT - 3 * MARGIN;

	// input window
	int xPosInput = MARGIN;
	int yPosInput = yPosConsole + consoleHeight + MARGIN;
	int inputWidth = m_parentWindowWidth - 3 * MARGIN - BUTTON_WIDTH;

	// send button
	int xPosSendBtn = xPosInput + inputWidth + MARGIN;
	int yPosSendBtn = yPosInput;

	MoveWindow(m_hConsoleWindow, xPosConsole, yPosConsole, consoleWidth, consoleHeight, TRUE);
	MoveWindow(m_hInputWindow, xPosInput, yPosInput, inputWidth, INPUT_HEIGHT, TRUE);
	MoveWindow(m_hSendButton, xPosSendBtn, yPosSendBtn, BUTTON_WIDTH, BUTTON_HEIGHT, TRUE);
}

void Console::SetConsoleColor() const
{
	// set RichEdit background color using EM_SETBKGNDCOLOR
	SendMessageW(m_hConsoleWindow, EM_SETBKGNDCOLOR, 0, RGB(0, 0, 0));

	// create a CHARFORMAT2 structure to modify text color and font
	CHARFORMAT2 cf = {};
	cf.cbSize = sizeof(CHARFORMAT2);
	cf.dwMask = CFM_COLOR | CFM_FACE | CFM_SIZE;
	cf.crTextColor = RGB(0, 255, 0);		// Text color: Bright green "console-style"
	cf.yHeight = 200;						// 10pt font height
	wcscpy_s(cf.szFaceName, L"Consolas");	// Console-style font

	// apply to all text with SCF_ALL
	SendMessageW(m_hConsoleWindow, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&cf);
}

void Console::SetInputColor() const
{
	// set RichEdit background color using EM_SETBKGNDCOLOR
	SendMessageW(m_hInputWindow, EM_SETBKGNDCOLOR, 0, RGB(0, 0, 0));

	// create a CHARFORMAT2 structure to modify text color and font
	CHARFORMAT2 cf = {};
	cf.cbSize = sizeof(CHARFORMAT2);
	cf.dwMask = CFM_COLOR | CFM_FACE | CFM_SIZE;
	cf.crTextColor = RGB(0, 255, 0);		// Text color: Bright green "console-style"
	cf.yHeight = 200;						// 10pt font height
	wcscpy_s(cf.szFaceName, L"Consolas");	// Console-style font

	// Apply to all text with SCF_ALL
	SendMessageW(m_hInputWindow, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&cf);
}

void Console::Cleanup()
{
	RemoveWindowSubclass(m_hSendButton, Console::InputProc, 1);
	RemoveWindowSubclass(m_hInputWindow, Console::InputProc, 1);
	RemoveWindowSubclass(m_hConsoleWindow, Console::ConsoleProc, 1);

	DestroyWindow(m_hSendButton);
	m_hSendButton = nullptr;

	DestroyWindow(m_hInputWindow);
	m_hInputWindow = nullptr;

	DestroyWindow(m_hConsoleWindow);
	m_hConsoleWindow = nullptr;
}