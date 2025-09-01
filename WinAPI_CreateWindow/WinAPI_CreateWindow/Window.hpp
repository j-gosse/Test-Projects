#pragma once

#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#define MAX_LOADSTRING 100

#include "framework.h"
#include "resource.h"
#include "KeyHandler.hpp"

#include <stdexcept>
#include <iostream>

class Window
{
private:
	HWND m_hWindow;
	WNDCLASSEX m_windowClass;
	HINSTANCE m_hInstance;
	HACCEL m_hAccelTable;
	STARTUPINFO m_startupInfo;

	WCHAR m_szTitle[MAX_LOADSTRING];		// title bar text
	WCHAR m_szWindowClass[MAX_LOADSTRING];	// main window class name

	static constexpr const uint32_t WINDOW_WIDTH = 800;
	static constexpr const uint32_t WINDOW_HEIGHT = 600;

	std::unique_ptr<KeyHandler> m_keyHandler;

	/**
	* @brief	Function to deliver specific messages to a window.
	* @param	UINT uMsg : uMsg is the message code; for example, the WM_SIZE message indicates the window was resized.
	* @param	WPARAM wParam : Provides additional message-specific information. Indicates whether the window was minimized, maximized, or resized.
	* @param	LPARAM lParam : Provides additional message-specific information. Contains the new width and height of the window.
	* @return	DefWindowProcW(m_hWindow, uMsg, wParam, lParam) when default switch case or return 0 when switch case
	*/
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	/**
	* @brief	Callback function to process messages sent to a window.
	* @param	HWND hWnd : A handle to the window that is receiving the message.
	* @param	UINT uMsg : The message identifier. This parameter specifies which message is being sent to the window (e.g. WM_CREATE, WM_PAINT, WM_COMMAND).
	* @param	WPARAM wParam : Provides additional message-specific information. Indicates whether the window was minimized, maximized, or resized.
	* @param	LPARAM lParam : Provides additional message-specific information. Contains the new width and height of the window.
	* @return	DefWindowProcW(hWnd, uMsg, wParam, lParam)
	*/
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	/**
	* @brief	Register the window class.
	* @return	RegisterClassExW(&m_windowClass)
	*/
	ATOM RegisterWindowClass();

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
	* @brief	Return the window handle.
	* @return	HWND m_hWindow
	*/
	HWND GetWindow() const { return m_hWindow; }

	/**
	* @brief	Return the key handler.
	* @return	KeyHandler m_keyHandler
	*/
	KeyHandler& GetKeyHandler() { return *m_keyHandler; }
};

#endif