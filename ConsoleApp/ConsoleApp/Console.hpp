#pragma once

#ifndef CONSOLE_HPP_
#define CONSOLE_HPP_

#include "framework.h"
#include "resource.h"

class Console
{
private:
	HWND m_hParentWindow;
	HINSTANCE m_hParentInstance;
	HWND m_hConsoleWindow;
	HWND m_hInputWindow;
	HWND m_hSendButton;

	LONG m_parentWindowWidth;
	LONG m_parentWindowHeight;

	static LRESULT CALLBACK ConsoleProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	LRESULT HandleConsoleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const;
	static LRESULT CALLBACK InputProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	LRESULT HandleInputMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const;
	static LRESULT CALLBACK ButtonProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	LRESULT HandleButtonMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const;

public:
	Console(LONG parentWindowWidth, LONG parentWindowHeight);
	Console(HWND hParentWindow, HINSTANCE hParentInstance, LONG parentWindowWidth, LONG parentWindowHeight);
	virtual ~Console();

	void SendInput() const;
	void ResizeWindows(WPARAM wParam, LPARAM lParam);
	void SetConsoleColor() const;
	void SetInputColor() const;
	void Cleanup();

	HWND GetConsoleWindow() const { return m_hConsoleWindow; }
	HWND GetInputWindow() const { return m_hInputWindow; }

};

#endif