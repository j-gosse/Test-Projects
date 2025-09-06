#include "Window.hpp"

#include <lmcons.h>	// UNLEN and MAX_COMPUTERNAME_LENGTH

/* CONSTRUCTOR */

Window::Window(HINSTANCE hInstance) :
	m_hWindow(nullptr),
	m_mainWindowClass({}),
	m_hInstance(hInstance),
	m_hAccelTable(nullptr),
	m_startupInfo({}),
	m_processInfo({}),
	m_systemInfo({}),
	m_szTitle(L""),
	m_szWindowClass(L""),
	m_keyHandler(std::make_unique<KeyHandler>())
{
	std::wcout << L"CONSTRUCTOR: Window(HINSTANCE hInstance)" << L'\n';

	ZeroMemory(&m_startupInfo, sizeof(m_startupInfo));
	m_startupInfo.cb = sizeof(m_startupInfo);
	ZeroMemory(&m_processInfo, sizeof(m_processInfo));

	m_startupInfo.dwFlags = STARTF_USESHOWWINDOW;
	m_startupInfo.wShowWindow = SW_SHOW;
}

/* DESTRUCTOR */

Window::~Window()
{
	std::wcout << L"DESTRUCTOR: ~Window()" << L'\n';
}

/* FUNCTION DEFINITIONS */

void Window::InitWindow()
{
	Window::GetSysInfo();
	//Window::GetProcessorInfo();

	LoadStringW(m_hInstance, IDS_WINDOW_TITLE, m_szTitle, MAX_LOADSTRING);
	LoadStringW(m_hInstance, IDS_WINDOW_CLASS, m_szWindowClass, MAX_LOADSTRING);
	Window::RegisterWindowClass();

	// nCmdShow controls how the window is to be shown
	// Paremeter is ignored the first time an application calls show window if STARTUPINFO structure specified
	int nCmdShow = (m_startupInfo.dwFlags & STARTF_USESHOWWINDOW) ? m_startupInfo.wShowWindow : SW_SHOWDEFAULT;

	DWORD dwExStyle = 0;
	RECT rect = { 0, 0, Window::WINDOW_WIDTH, Window::WINDOW_HEIGHT };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, TRUE, dwExStyle);
	int windowWidth = rect.right - rect.left;
	int windowHeight = rect.bottom - rect.top;

	m_hWindow = CreateWindowExW(//HWND CreateWindowExW(
		dwExStyle,				//[in]			 DWORD	   dwExStyle,
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

	// set window position centered relative to screen resolution
	int centerWidth = (m_screenWidth - windowWidth) / 2;
	int centerHeight = (m_screenHeight - windowHeight) / 2;

	SetWindowPos(				//BOOL SetWindowPos(
		m_hWindow,				//[in]           HWND hWnd,
		HWND_TOP,				//[in, optional] HWND hWndInsertAfter,
		centerWidth,			//[in]           int  X,	 // The new position of the left side of the window, in client coordinates
		centerHeight,			//[in]           int  Y,	 // The new position of the top of the window, in client coordinates
		windowWidth,			//[in]           int  cx,	 // The new width of the window, in pixels
		windowHeight,			//[in]           int  cy,	 // The new height of the window, in pixels
		SWP_SHOWWINDOW			//[in]           UINT uFlags // The window sizing and positioning flags
	);

	ShowWindow(m_hWindow, nCmdShow);
	UpdateWindow(m_hWindow);

	m_hAccelTable = LoadAccelerators(m_hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR));
}

BOOL Window::ProcessMessages() const
{
	MSG msg = {};
	UINT msgFilterMin = 0;
	UINT msgFilterMax = 0;

	while (PeekMessageW(&msg, m_hWindow, msgFilterMin, msgFilterMax, PM_REMOVE))
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
	UINT msgFilterMin = 0;
	UINT msgFilterMax = 0;

	while (GetMessage(&msg, m_hWindow, msgFilterMin, msgFilterMax))
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
		if (MessageBoxW(m_hWindow, L"Do you wish to exit?", L"Create Window", MB_OKCANCEL) == IDOK)
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
			if (MessageBoxW(m_hWindow, L"Do you wish to exit?", L"Create Window", MB_OKCANCEL) == IDOK)
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
	int extraClassBytes = 0;
	int extraWindowBytes = 0;

	m_mainWindowClass.cbSize = sizeof(WNDCLASSEX);
	m_mainWindowClass.lpfnWndProc = Window::WindowProc;
	m_mainWindowClass.hInstance = m_hInstance;
	m_mainWindowClass.lpszClassName = Window::m_szWindowClass;
	m_mainWindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_OWNDC;
	m_mainWindowClass.lpszMenuName = MAKEINTRESOURCEW(IDR_MAINFRAME);
	m_mainWindowClass.cbClsExtra = extraClassBytes;
	m_mainWindowClass.cbWndExtra = extraWindowBytes;
	m_mainWindowClass.hCursor = LoadCursorW(NULL, IDC_ARROW);
	m_mainWindowClass.hIcon = LoadIconW(m_hInstance, MAKEINTRESOURCE(IDI_PRIMARY));
	m_mainWindowClass.hIconSm = LoadIcon(m_mainWindowClass.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	m_mainWindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	//m_mainWindowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	//m_mainWindowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	if (!RegisterClassExW(&m_mainWindowClass))
	{
		throw std::runtime_error("Failed to register the window class!");
	}

	return RegisterClassExW(&m_mainWindowClass);
}

void Window::GetSysInfo()
{
	WCHAR buffer1[UNLEN + 1]{};
	DWORD size1 = UNLEN + 1;
	GetUserNameW(buffer1, &size1);
	std::wcout << L"\nUsername: " << buffer1 << L'\n';

	WCHAR buffer2[MAX_COMPUTERNAME_LENGTH + 1]{};
	DWORD size2 = MAX_COMPUTERNAME_LENGTH + 1;
	GetComputerNameW(buffer2, &size2);
	std::wcout << L"Computer Name: " << buffer2 << L'\n';

	/*
	typedef struct _SYSTEM_INFO {
	  union {
		DWORD dwOemId;
		struct {
		  WORD wProcessorArchitecture;
		  WORD wReserved;
		} DUMMYSTRUCTNAME;
	  } DUMMYUNIONNAME;
	  DWORD     dwPageSize;
	  LPVOID    lpMinimumApplicationAddress;
	  LPVOID    lpMaximumApplicationAddress;
	  DWORD_PTR dwActiveProcessorMask;
	  DWORD     dwNumberOfProcessors;
	  DWORD     dwProcessorType;
	  DWORD     dwAllocationGranularity;
	  WORD      wProcessorLevel;
	  WORD      wProcessorRevision;
	} SYSTEM_INFO, *LPSYSTEM_INFO;

	Use the wProcessorArchitecture, wProcessorLevel, and wProcessorRevision members to determine the type of processor.
	*/
	GetNativeSystemInfo(&m_systemInfo);

	std::wcout << L"Processor Architecture: ";
	switch (m_systemInfo.wProcessorArchitecture)
	{
	case PROCESSOR_ARCHITECTURE_INTEL:
		std::wcout << L"x86\n";
		break;
	case PROCESSOR_ARCHITECTURE_AMD64:
		std::wcout << L"x64 (AMD or Intel)\n";
		break;
	case PROCESSOR_ARCHITECTURE_ARM:
		std::wcout << L"ARM\n";
		break;
	case PROCESSOR_ARCHITECTURE_ARM64:
		std::wcout << L"ARM64\n";
		break;
	case PROCESSOR_ARCHITECTURE_IA64:
		std::wcout << L"Intel Itanium-based\n";
		break;
	default:
		std::wcout << L"Unknown architecture\n";
		break;
	}
	std::wcout << L"Processor Level: " << m_systemInfo.wProcessorLevel << L'\n';
	std::wcout << L"Processor Revision: " << m_systemInfo.wProcessorRevision << L'\n';
	std::wcout << L"Logical Processors (threads): " << m_systemInfo.dwNumberOfProcessors << L'\n';

	WORD revision = m_systemInfo.wProcessorRevision;
	BYTE model = HIBYTE(revision);
	BYTE stepping = LOBYTE(revision);

	std::wcout << L"CPU Model: " << model << L'\n';
	std::wcout << L"Stepping: " << stepping << L'\n';

	std::wcout << std::endl;
}

void Window::GetProcessorInfo()
{
	PSYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer = NULL;
	DWORD size = 0;

	/*
	typedef struct _SYSTEM_LOGICAL_PROCESSOR_INFORMATION
	{
		ULONG_PTR ProcessorMask;
		LOGICAL_PROCESSOR_RELATIONSHIP Relationship;

		union
		{
			struct
			{
				BYTE Flags;
			} ProcessorCore;

			struct
			{
				DWORD NodeNumber;
			} NumaNode;

			CACHE_DESCRIPTOR Cache;
			ULONGLONG Reserved[2];
		} DUMMYUNIONNAME;
	} SYSTEM_LOGICAL_PROCESSOR_INFORMATION, *PSYSTEM_LOGICAL_PROCESSOR_INFORMATION;
	*/
	GetLogicalProcessorInformation(NULL, &size); // Get the required buffer size
	buffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)malloc(size);
	if (buffer == NULL)
	{
		Window::LogLastError(L"Memory allocation failed");
		throw std::runtime_error("Failed to allocate memory!");
	}

	if (!GetLogicalProcessorInformation(buffer, &size))
	{
		Window::LogLastError(L"GetLogicalProcessorInformation failed");
		free(buffer);
		throw std::runtime_error("Failed to call GetLogicalProcessorInformation!");
	}

	DWORD count = size / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
	for (DWORD i = 0; i < count; ++i)
	{
		printf("Processor %u: Relationship = %d\n", i, buffer[i].Relationship);
	}

	free(buffer);

	std::wcout << std::endl;
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
	DWORD errorCode = GetLastError();

	if (errorCode == 0)
	{
		std::wcout << desc << L": No error." << std::endl;
		return;
	}

	wchar_t* errorMsg = nullptr;

	FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		errorCode,
		0,
		reinterpret_cast<LPWSTR>(&errorMsg),
		0,
		nullptr
	);

	if (errorMsg)
	{
		std::wcout << desc << L" failed with error code " << errorCode << L": " << errorMsg << std::endl;
		LocalFree(errorMsg); // Free buffer allocated by FormatMessage
	}
	else
	{
		std::wcout << desc << L" failed with unknown error code: " << errorCode << std::endl;
	}
}