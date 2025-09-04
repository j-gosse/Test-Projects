#include "Window.hpp"
#include <lmcons.h>	// UNLEN and MAX_COMPUTERNAME_LENGTH

/* CONSTRUCTOR */

Window::Window(HINSTANCE hInstance) :
	m_hWindow(nullptr),
	m_windowClass({}),
	m_hInstance(hInstance),
	m_hAccelTable(nullptr),
	m_startupInfo({}),
	m_processInfo({}),
	m_systemInfo({}),
	m_szTitle(L""),
	m_szWindowClass(L""),
	m_keyHandler(std::make_unique<KeyHandler>())
{
	std::wcout << L"CONSTRUCTOR: Window()" << '\n';

	ZeroMemory(&m_startupInfo, sizeof(m_startupInfo));
	m_startupInfo.cb = sizeof(m_startupInfo);
	ZeroMemory(&m_processInfo, sizeof(m_processInfo));

	m_startupInfo.dwFlags = STARTF_USESHOWWINDOW;
	m_startupInfo.wShowWindow = SW_SHOW;

	GetNativeSystemInfo(&m_systemInfo);

	std::wcout << L"\nUser Information:\n";
	WCHAR buffer1[UNLEN + 1]{};
	DWORD size1 = UNLEN + 1;
	GetUserNameW(buffer1, &size1);
	std::wcout << L"Username: " << buffer1 << '\n';

	WCHAR buffer2[MAX_COMPUTERNAME_LENGTH + 1]{};
	DWORD size2 = MAX_COMPUTERNAME_LENGTH + 1;
	GetComputerNameW(buffer2, &size2);
	std::wcout << L"Computer Name: " << buffer2 << '\n';

	std::wcout << L"Processor Information:\n";
	std::wcout << L"Architecture: ";
	switch (m_systemInfo.wProcessorArchitecture)
	{
	case PROCESSOR_ARCHITECTURE_AMD64:
		std::wcout << L"x64 (AMD/Intel)";
		break;
	case PROCESSOR_ARCHITECTURE_INTEL:
		std::wcout << L"x86";
		break;
	case PROCESSOR_ARCHITECTURE_ARM64:
		std::wcout << L"ARM64";
		break;
	default:
		std::wcout << L"Unknown";
		break;
	}
	std::wcout << L"\nLogical Processors (threads): " << m_systemInfo.dwNumberOfProcessors << L"\n\n";
}

/* DESTRUCTOR */

Window::~Window()
{
	std::wcout << L"DESTRUCTOR: ~Window()" << '\n';
}

/* FUNCTION DEFINITIONS */

void Window::InitWindow()
{
	LoadStringW(m_hInstance, IDS_WINDOW_TITLE, m_szTitle, MAX_LOADSTRING);
	LoadStringW(m_hInstance, IDC_WINDOW_CLASS, m_szWindowClass, MAX_LOADSTRING);

	RegisterWindowClass();

	// nCmdShow controls how the window is to be shown
	// Paremeter is ignored the first time an application calls show window if STARTUPINFO structure specified
	int nCmdShow = (m_startupInfo.dwFlags & STARTF_USESHOWWINDOW) ? m_startupInfo.wShowWindow : SW_SHOWDEFAULT;

	RECT rect = { 0, 0, Window::WINDOW_WIDTH, Window::WINDOW_HEIGHT };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	int windowWidth = rect.right - rect.left;
	int windowHeight = rect.bottom - rect.top;

	m_hWindow = CreateWindowExW(//HWND CreateWindowExW(
		0,						//[in]			 DWORD	   dwExStyle,
		Window::m_szWindowClass,//[in, optional] LPCSTR    lpClassName,
		Window::m_szTitle,		//[in, optional] LPCSTR    lpWindowName,
		WS_OVERLAPPEDWINDOW,	//[in]           DWORD     dwStyle,
		CW_USEDEFAULT,			//[in]           int       X,
		CW_USEDEFAULT,			//[in]           int       Y,
		windowWidth,			//[in]           int       nWidth,
		windowHeight,			//[in]           int       nHeight,
		nullptr,				//[in, optional] HWND      hWndParent,
		nullptr,				//[in, optional] HMENU     hMenu,
		m_hInstance,			//[in, optional] HINSTANCE hInstance,
		this					//[in, optional] LPVOID    lpParam
	);

	if (m_hWindow == nullptr)
	{
		throw std::runtime_error("Failed to create the window!");
	}

	// set window position relative to screen resolution
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	int centerWidth = (screenWidth - windowWidth) / 2;
	int centerHeight = (screenHeight - windowHeight) / 2;

	SetWindowPos(				//BOOL SetWindowPos(
		m_hWindow,				//[in]           HWND hWnd,
		HWND_TOPMOST,			//[in, optional] HWND hWndInsertAfter,
		centerWidth,			//[in]           int  X,	 // The new position of the left side of the window, in client coordinates
		centerHeight,			//[in]           int  Y,	 // The new position of the top of the window, in client coordinates
		windowWidth,			//[in]           int  cx,	 // The new width of the window, in pixels
		windowHeight,			//[in]           int  cy,	 // The new height of the window, in pixels
		SWP_SHOWWINDOW			//[in]           UINT uFlags // The window sizing and positioning flags
	);

	ShowWindow(m_hWindow, nCmdShow);
	UpdateWindow(m_hWindow);

	m_hAccelTable = LoadAccelerators(m_hInstance, MAKEINTRESOURCE(IDC_WINDOW_CLASS));
}

BOOL Window::ProcessMessages() const
{
	MSG msg = {};

	while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT) return FALSE;

		if (!TranslateAccelerator(msg.hwnd, m_hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return TRUE;
}

/*
BOOL Window::ProcessMessages() const
{
	MSG msg = {};

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, m_hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}
*/

void Window::Cleanup()
{
	m_keyHandler.reset();
	m_keyHandler = nullptr;

	DestroyAcceleratorTable(m_hAccelTable);
	m_hAccelTable = nullptr;

	DestroyWindow(m_hWindow);
	m_hWindow = nullptr;

	if (!UnregisterClassW(Window::m_szWindowClass, m_hInstance))
	{
		throw std::runtime_error("Failed to unregister the window class!");
	}
}

LRESULT Window::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
		m_keyHandler->DispatchKeyDown(wParam);
		return 0;
	case WM_KEYUP:
		m_keyHandler->DispatchKeyUp(wParam);
		return 0;
	case WM_CHAR:
		m_keyHandler->DispatchChar(wParam);
		return 0;
	case WM_SIZE:
		std::wcout << L"CASE: WM_SIZE" << '\n';
		std::wcout << LOWORD(lParam) << L'x' << HIWORD(lParam) << '\n';
		return 0;
	case WM_DESTROY:
		std::wcout << L"CASE: WM_DESTROY" << '\n';
		PostQuitMessage(0);
		return 0;
	case WM_CLOSE:
		std::wcout << L"CASE: WM_CLOSE" << '\n';
		if (MessageBoxW(m_hWindow, L"Do you wish to exit?", L"Particle Effects", MB_OKCANCEL) == IDOK)
		{
			Window::Cleanup();
		}
		return 0;
	case WM_COMMAND:
		std::wcout << L"CASE: WM_COMMAND" << '\n';
		switch (LOWORD(wParam))
		{
		case IDM_ABOUT:
			std::wcout << L"CASE: IDM_ABOUT" << '\n';
			DialogBox(m_hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), m_hWindow, About);
			return 0;
		case IDM_EXIT:
			std::wcout << L"CASE: IDM_EXIT" << '\n';
			if (MessageBoxW(m_hWindow, L"Do you wish to exit?", L"Particle Effects", MB_OKCANCEL) == IDOK)
			{
				Window::Cleanup();
			}
			return 0;
		}
		return 0;
	default:
		return DefWindowProcW(m_hWindow, uMsg, wParam, lParam);
	}
}

LRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Window* pWindow = nullptr;

	if (uMsg == WM_NCCREATE)
	{
		LPCREATESTRUCTW createStruct = reinterpret_cast<LPCREATESTRUCTW>(lParam);
		pWindow = static_cast<Window*>(createStruct->lpCreateParams);
		SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
		pWindow->m_hWindow = hWnd;
	}
	else
	{
		pWindow = reinterpret_cast<Window*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
	}

	if (pWindow) return pWindow->Window::HandleMessage(uMsg, wParam, lParam);

	return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

ATOM Window::RegisterWindowClass()
{
	m_windowClass.cbSize = sizeof(WNDCLASSEX);
	m_windowClass.lpfnWndProc = Window::WindowProc;
	m_windowClass.hInstance = m_hInstance;
	m_windowClass.lpszClassName = Window::m_szWindowClass;
	m_windowClass.style = CS_HREDRAW | CS_VREDRAW;
	m_windowClass.lpszMenuName = MAKEINTRESOURCEW(IDC_WINDOW_CLASS);
	m_windowClass.cbClsExtra = 0;
	m_windowClass.cbWndExtra = 0;
	m_windowClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
	m_windowClass.hIcon = LoadIconW(m_hInstance, MAKEINTRESOURCE(IDI_PRIMARY));
	m_windowClass.hIconSm = LoadIcon(m_windowClass.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	m_windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	//m_windowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	//m_windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	if (!RegisterClassExW(&m_windowClass))
	{
		throw std::runtime_error("Failed to register the window class!");
	}

	return RegisterClassExW(&m_windowClass);
}

INT_PTR CALLBACK Window::About(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (uMsg)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void Window::LogLastError(const wchar_t* desc) {
	DWORD eCode = GetLastError();

	if (eCode == 0)
	{
		std::wcout << desc << L": No error." << std::endl;
		return;
	}

	wchar_t* eMsg = nullptr;

	FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		eCode,
		0,
		reinterpret_cast<LPWSTR>(&eMsg),
		0,
		nullptr
	);

	if (eMsg)
	{
		std::wcout << desc << L" failed with error code " << eCode << L": " << eMsg << std::endl;
		LocalFree(eMsg); // Free buffer allocated by FormatMessage
	}
	else
	{
		std::wcout << desc << L" failed with unknown error code: " << eCode << std::endl;
	}
}