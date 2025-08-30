#pragma once

#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <windows.h>
#include <stdexcept>
#include <iostream>

class Window
{
private:
	HINSTANCE m_hInstance;
	WNDCLASSEX m_windowClass;
	HWND m_hWindow;
	STARTUPINFO m_startupInfo;

	static constexpr const uint32_t WINDOW_WIDTH = 800;
	static constexpr const uint32_t WINDOW_HEIGHT = 600;
	static constexpr const wchar_t CLASS_NAME[] = L"MainWindowClass";
	static constexpr const wchar_t WINDOW_NAME[] = L"Particle Effects";

	/**
	* @brief	Function to deliver specific messages to a window.
	* @param	UINT uMsg : uMsg is the message code; for example, the WM_SIZE message indicates the window was resized.
	* @param	WPARAM wParam : Provides additional message-specific information. Indicates whether the window was minimized, maximized, or resized.
	* @param	LPARAM lParam : Provides additional message-specific information. Contains the new width and height of the window.
	*/
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	/**
	* @brief	Callback function to process messages sent to a window.
	* @param	HWND hWnd : A handle to the window that is receiving the message.
	* @param	UINT uMsg : The message identifier. This parameter specifies which message is being sent to the window (e.g. WM_CREATE, WM_PAINT, WM_COMMAND).
	* @param	WPARAM wParam : Provides additional message-specific information. Indicates whether the window was minimized, maximized, or resized.
	* @param	LPARAM lParam : Provides additional message-specific information. Contains the new width and height of the window.
	*/
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	/**
	* @brief	Window Constructor.
	*/
	Window();

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
	* @brief	Customize window properties and create window.
	*/
	void Init();

	/**
	* @brief	Process queued messages sent to the window.
	*/
	bool ProcessMessages();

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
};

#endif