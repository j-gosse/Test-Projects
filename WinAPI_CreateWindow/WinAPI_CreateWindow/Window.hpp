#pragma once

#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#define MAX_LOADSTRING 100

#include "framework.h"
#include "resource.h"

class Window
{
private:
	HWND m_hWindow;
	WNDCLASSEX m_windowClass;
	HINSTANCE m_hInstance;
	HACCEL m_hAccelTable;
	STARTUPINFO m_startupInfo;
	PROCESS_INFORMATION m_processInfo;
	SYSTEM_INFO m_systemInfo;

	WCHAR m_windowName[MAX_LOADSTRING];
	WCHAR m_windowClassName[MAX_LOADSTRING];

	static constexpr const LONG WINDOW_WIDTH = 800;
	static constexpr const LONG WINDOW_HEIGHT = 600;
	int m_screenWidth = WINDOW_WIDTH;
	int m_screenHeight = WINDOW_HEIGHT;
	LONG m_desktopWidth = GetSystemMetrics(SM_CXSCREEN);
	LONG m_desktopHeight = GetSystemMetrics(SM_CYSCREEN);

	/**
	* @brief	Handle messages sent to the window on a switch-case basis.
	* @param	UINT uMsg : The message identifier. This parameter specifies which message is being sent to the window (e.g. WM_CREATE, WM_PAINT, WM_COMMAND).
	* @param	WPARAM wParam : Provides additional message-specific information. Indicates whether the window was minimized, maximized, or resized.
	* @param	LPARAM lParam : Provides additional message-specific information. Contains the new width and height of the window.
	* @return	DefWindowProcW(m_hWindow, uMsg, wParam, lParam) when default switch case or return 0 when switch case
	*/
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	/**
	* @brief	Callback function to process event messages sent to the window.
	* @param	HWND hWnd : A handle to the window that is receiving messages.
	* @param	UINT uMsg : The message identifier. This parameter specifies which message is being sent to the window (e.g. WM_CREATE, WM_PAINT, WM_COMMAND).
	* @param	WPARAM wParam : Provides additional message-specific information. Indicates whether the window was minimized, maximized, or resized.
	* @param	LPARAM lParam : Provides additional message-specific information. Contains the new width and height of the window.
	* @return	DefWindowProcW(hWnd, uMsg, wParam, lParam)
	*/
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	/**
	* @brief	Registers the main window class.
	* @return	RegisterClassExW(&m_windowClass)
	*/
	ATOM RegisterWindowClass();

	/**
	* @brief	Retrieve username, computer name, processor architecture and CPU information.
	*/
	void GetSysInfo();

	/**
	* @brief	Retrieve logical processor count and relationships.
	*/
	void GetProcessorInfo();

	/**
	* @brief	Callback function to generate the About message.
	* @param	HWND hDlg : A handle to the window dialog box that is receiving the message.
	* @param	UINT uMsg : The message identifier. This parameter specifies which message is being sent to the window (e.g. WM_CREATE, WM_PAINT, WM_COMMAND).
	* @param	WPARAM wParam : Provides additional message-specific information.
	* @param	LPARAM lParam : Provides additional message-specific information.
	* @return	(INT_PTR)TRUE when switch case or (INT_PTR)FALSE when no switch case
	*/
	static INT_PTR CALLBACK About(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	/**
	* @brief	Window Constructor.
	*/
	Window();

	/**
	* @brief	1-arg Window Constructor.
	* @param	HINSTANCE hInstance : A handle to the window instance module.
	*/
	Window(HINSTANCE hInstance);

	/**
	* @brief	Copy constructor. (deleted)
	* @param	Window& : constant lvalue reference
	*/
	Window(const Window&) = delete;

	/**
	* @brief	Copy assignment operator. (deleted)
	* @param	Window& : constant lvalue reference
	*/
	Window& operator=(const Window&) = delete;

	/**
	* @brief	Move constructor. (deleted)
	* @param	Window&& : rvalue reference
	*/
	Window(Window&&) = delete;

	/**
	* @brief	Move assignment operator. (deleted)
	* @param	Window&& : rvalue reference
	*/
	Window& operator=(Window&&) = delete;

	/**
	* @brief	Window Destructor.
	*/
	virtual ~Window();

	/**
	* @brief	Customize window properties and create the window.
	*/
	void InitWindow();

	/**
	* @brief	Process queued messages sent to the window.
	* @return	(int)msg.wParam
	*/
	BOOL ProcessMessages() const;

	/**
	* @brief	Destroy the window and unregister window class.
	*/
	void Cleanup();

	/**
	* @brief	Log the last error that occurred to the console.
	* @param	const wchar_t* desc : Description of where the error originated from.
	*/
	void LogLastError(const wchar_t* desc);

	/**
	* @brief	Get the window handle.
	* @return	HWND m_hWindow
	*/
	HWND GetWindow() const { return m_hWindow; }
};

#endif