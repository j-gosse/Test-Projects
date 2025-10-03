#include "Window.hpp"

/* CONSTRUCTORS */

Window::Window() :
	m_hWindow(nullptr),
	m_windowClass({}),
	m_hInstance(GetModuleHandleW(nullptr)),
	m_hAccelTable(nullptr),
	m_startupInfo({}),
	m_windowName(L""),
	m_windowClassName(L"")
{
	std::wcout << L"CONSTRUCTOR: Window()" << L'\n';
	Window::InitWindow();
}

Window::Window(HINSTANCE hInstance) :
	m_hWindow(nullptr),
	m_windowClass({}),
	m_hInstance(hInstance),
	m_hAccelTable(nullptr),
	m_startupInfo({}),
	m_windowName(L""),
	m_windowClassName(L"")
{
	std::wcout << L"CONSTRUCTOR: Window(HINSTANCE hInstance)" << L'\n';
	Window::InitWindow();
}

/* DESTRUCTOR */

Window::~Window()
{
	std::wcout << L"DESTRUCTOR: ~Window()" << L'\n';
}

/* FUNCTION DEFINITIONS */

BOOL Window::ProcessMessages() const
{
	MSG msg = {};
	UINT msgFilterMin = 0;
	UINT msgFilterMax = 0;

	while (PeekMessageW(&msg, nullptr, msgFilterMin, msgFilterMax, PM_REMOVE))
	{
		if (msg.message == WM_QUIT) return FALSE;

		if (m_hAccelTable && !TranslateAcceleratorW(msg.hwnd, m_hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
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

	while (GetMessageW(&msg, nullptr, msgFilterMin, msgFilterMax))
	{
		if (m_hAccelTable && !TranslateAcceleratorW(msg.hwnd, m_hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}
	return (int)msg.wParam;
}
*/

LRESULT Window::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_PAINT:
	{
		m_keyHandler.PaintKeys(m_hWindow);
		return 0;
	}
	case WM_KEYUP:
	case WM_KEYDOWN:
	{
		unsigned int vkCode = static_cast<unsigned int>(wParam);
		bool isDown = ((lParam & (1LL << 31)) == 0);
		//bool isDown = (uMsg == WM_KEYDOWN);
		m_keyHandler.Update(vkCode, isDown);
		m_keyHandler.UpdateDisplayText(m_hWindow);
	} return 0;
	case WM_CHAR:
		return 0;
	case WM_SIZE:
		std::wcout << L"CASE: WM_SIZE" << L'\n';
		std::wcout << LOWORD(lParam) << L'x' << HIWORD(lParam) << L'\n';
		m_screenWidth = LOWORD(lParam);
		m_screenHeight = HIWORD(lParam);
		return 0;
	case WM_CLOSE:
		std::wcout << L"CASE: WM_CLOSE" << L'\n';
		if (MessageBoxW(m_hWindow, L"Do you wish to exit?", L"WinAPI_InputTest", MB_OKCANCEL) == IDOK)
		{
			Window::Cleanup();
		}
		return 0;
	case WM_COMMAND:
		std::wcout << L"CASE: WM_COMMAND" << L'\n';
		switch (LOWORD(wParam))
		{
		case IDM_ABOUT:
			std::wcout << L"CASE: IDM_ABOUT" << L'\n';
			DialogBox(m_hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), m_hWindow, About);
			return 0;
		case IDM_EXIT:
			std::wcout << L"CASE: IDM_EXIT" << L'\n';
			if (MessageBoxW(m_hWindow, L"Do you wish to exit?", L"WinAPI_InputTest", MB_OKCANCEL) == IDOK)
			{
				Window::Cleanup();
			}
			return 0;
		}
		return 0;
	case WM_DESTROY:
		std::wcout << L"CASE: WM_DESTROY" << L'\n';
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProcW(m_hWindow, uMsg, wParam, lParam);
	}
}

LRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Window* pWindow = nullptr;

	switch (uMsg)
	{
	case WM_NCCREATE:
	{
		LPCREATESTRUCTW createStruct = reinterpret_cast<LPCREATESTRUCTW>(lParam);
		pWindow = static_cast<Window*>(createStruct->lpCreateParams);
		SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
		pWindow->m_hWindow = hWnd;
		break;
	}
	default:
		pWindow = reinterpret_cast<Window*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
		break;
	}

	if (pWindow) return pWindow->Window::HandleMessage(uMsg, wParam, lParam);
	return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

ATOM Window::RegisterWindowClass()
{
	int extraClassBytes = 0;
	int extraWindowBytes = 0;

	m_windowClass.cbSize = sizeof(WNDCLASSEX);
	m_windowClass.lpfnWndProc = Window::WindowProc;
	m_windowClass.hInstance = m_hInstance;
	m_windowClass.lpszClassName = m_windowClassName;
	m_windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_OWNDC;
	m_windowClass.lpszMenuName = MAKEINTRESOURCEW(IDR_MAIN_MENU);
	m_windowClass.cbClsExtra = extraClassBytes;
	m_windowClass.cbWndExtra = extraWindowBytes;
	m_windowClass.hCursor = LoadCursorW(NULL, IDC_ARROW);
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

HWND Window::BuildWindow(HINSTANCE hInstance, int windowWidth, int windowHeight, DWORD dwExStyle)
{
	HWND hWnd = CreateWindowExW
	(
		dwExStyle,				//[in]			 DWORD	   dwExStyle,
		m_windowClassName,		//[in, optional] LPCSTR    lpClassName,
		m_windowName,			//[in, optional] LPCSTR    lpWindowName,
		WS_OVERLAPPEDWINDOW,	//[in]           DWORD     dwStyle,
		CW_USEDEFAULT,			//[in]           int       X,
		CW_USEDEFAULT,			//[in]           int       Y,
		windowWidth,			//[in]           int       nWidth,
		windowHeight,			//[in]           int       nHeight,
		nullptr,				//[in, optional] HWND      hWndParent,
		nullptr,				//[in, optional] HMENU     hMenu,
		hInstance,				//[in, optional] HINSTANCE hInstance,
		this					//[in, optional] LPVOID    lpParam
	);

	if (hWnd == nullptr)
	{
		throw std::runtime_error("Failed to create the window!");
	}

	return hWnd;
}

void Window::RegisterRawInput(HWND hWnd)
{
	RAWINPUTDEVICE rid{};
	rid.usUsagePage = 0x01;		// Generic desktop controls
	rid.usUsage = 0x06;			// Keyboard
	rid.dwFlags = 0;			// No RIDEV_INPUTSINK, so only receive input when focused
	rid.hwndTarget = hWnd;

	if (!RegisterRawInputDevices(&rid, 1, sizeof(rid)))
	{
		throw std::runtime_error("Failed to register raw input device!");
	}
}

void Window::RepositionWindow(HWND hWnd, int leftX, int topY, int windowWidth, int windowHeight)
{
	SetWindowPos
	(
		hWnd,					//[in]           HWND hWnd,
		HWND_TOP,				//[in, optional] HWND hWndInsertAfter,
		leftX,					//[in]           int  X,	 // The new position of the left side of the window, in client coordinates
		topY,					//[in]           int  Y,	 // The new position of the top of the window, in client coordinates
		windowWidth,			//[in]           int  cx,	 // The new width of the window, in pixels
		windowHeight,			//[in]           int  cy,	 // The new height of the window, in pixels
		SWP_SHOWWINDOW			//[in]           UINT uFlags // The window sizing and positioning flags
	);
}

void Window::InitWindow()
{
	DWORD dwExStyle = 0;
	LONG leftX = 0;
	LONG topY = 0;
	RECT rect = { leftX, topY, Window::WINDOW_WIDTH, Window::WINDOW_HEIGHT };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, TRUE, dwExStyle);
	int windowWidth = rect.right - rect.left;
	int windowHeight = rect.bottom - rect.top;

	Window::SysInfo();
	//Window::ProcessorInfo();

	m_hAccelTable = LoadAcceleratorsW(m_hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR));

	LoadStringW(m_hInstance, IDS_WINDOW_TITLE, m_windowName, MAX_LOADSTRING);
	LoadStringW(m_hInstance, IDS_WINDOW_CLASS, m_windowClassName, MAX_LOADSTRING);
	Window::RegisterWindowClass();

	ZeroMemory(&m_startupInfo, sizeof(m_startupInfo));
	m_startupInfo.cb = sizeof(m_startupInfo);
	ZeroMemory(&m_processInfo, sizeof(m_processInfo));
	m_startupInfo.dwFlags = STARTF_USESHOWWINDOW;
	m_startupInfo.wShowWindow = SW_SHOW;
	int nCmdShow = (m_startupInfo.dwFlags & STARTF_USESHOWWINDOW) ? m_startupInfo.wShowWindow : SW_SHOWDEFAULT;

	m_hWindow = Window::BuildWindow(m_hInstance, windowWidth, windowHeight, dwExStyle);

	Window::RegisterRawInput(m_hWindow);

	int centerWidth = (m_desktopWidth - windowWidth) / 2;
	int centerHeight = (m_desktopHeight - windowHeight) / 2;
	Window::RepositionWindow(m_hWindow, centerWidth, centerHeight, windowWidth, windowHeight);

	ShowWindow(m_hWindow, nCmdShow);
	UpdateWindow(m_hWindow);
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

void Window::SysInfo()
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

void Window::ProcessorInfo()
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

void Window::Cleanup()
{
	DestroyAcceleratorTable(m_hAccelTable);
	m_hAccelTable = nullptr;

	DestroyWindow(m_hWindow);
	m_hWindow = nullptr;

	if (!UnregisterClassW(m_windowClassName, m_hInstance))
	{
		throw std::runtime_error("Failed to unregister the window class!");
	}
}

void Window::LogLastError(const wchar_t* desc)
{
	DWORD errorCode = GetLastError();

	if (errorCode == 0)
	{
		std::wcout << desc << L": No error." << std::endl;
		return;
	}

	wchar_t* errorMsg = nullptr;

	FormatMessageW
	(
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
		LocalFree(errorMsg);
	}
	else
	{
		std::wcout << desc << L" failed with unknown error code: " << errorCode << std::endl;
	}
}