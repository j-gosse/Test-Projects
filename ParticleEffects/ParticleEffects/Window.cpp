#include "Window.hpp"

/* CONSTRUCTOR */

Window::Window()
	: m_hInstance(GetModuleHandleW(nullptr)), m_hWindow(nullptr), m_windowClass({}), m_startupInfo({})
{
	std::wcout << L"CONSTRUCTOR: Window()" << '\n';
	m_startupInfo.cb = sizeof(m_startupInfo);
	GetStartupInfoW(&m_startupInfo);
}

/* DESTRUCTOR */

Window::~Window()
{
	std::wcout << L"DESTRUCTOR: ~Window()" << '\n';
}

/* FUNCTION DEFINITIONS */

void Window::Init()
{
	std::wcout << L"Window initialization." << std::endl;

	// create window class
	m_windowClass.cbSize = sizeof(WNDCLASSEX);
	m_windowClass.lpfnWndProc = Window::WindowProc;
	m_windowClass.hInstance = m_hInstance;
	m_windowClass.lpszClassName = Window::CLASS_NAME;
	m_windowClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
	m_windowClass.hIcon = LoadIconW(nullptr, IDI_APPLICATION);
	m_windowClass.hIconSm = LoadIconW(nullptr, IDI_APPLICATION);
	//m_windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);			// white background
	m_windowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);	// black background

	if (!RegisterClassExW(&m_windowClass))
	{
		throw std::runtime_error("Failed to register the window class!");
	}

	// nCmdShow controls how the window is to be shown
	// Paremeter is ignored the first time an application calls show window if STARTUPINFO structure specified
	int nCmdShow = (m_startupInfo.dwFlags & STARTF_USESHOWWINDOW) ? m_startupInfo.wShowWindow : SW_SHOWDEFAULT;

	// create window
	RECT rect = { 0, 0, Window::WINDOW_WIDTH, Window::WINDOW_HEIGHT };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	int windowWidth = rect.right - rect.left;
	int windowHeight = rect.bottom - rect.top;

	m_hWindow = CreateWindowExW(//HWND CreateWindowExW(
		0,						//[in]			 DWORD	   dwExStyle,
		Window::CLASS_NAME,		//[in, optional] LPCSTR    lpClassName,
		Window::WINDOW_NAME,	//[in, optional] LPCSTR    lpWindowName,
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
}

bool Window::ProcessMessages()
{
	MSG msg = {};

	while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT) return false;

		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	return true;
}

void Window::Cleanup()
{
	std::wcout << L"Window cleanup:" << '\n';

	DestroyWindow(m_hWindow);
	m_hWindow = nullptr;

	if (!UnregisterClassW(Window::CLASS_NAME, m_hInstance))
	{
		throw std::runtime_error("Failed to unregister the window class!");
	}
}

LRESULT Window::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
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
		LocalFree(eMsg); // Free the buffer allocated by FormatMessage
	}
	else
	{
		std::wcout << desc << L" failed with unknown error code: " << eCode << std::endl;
	}
}