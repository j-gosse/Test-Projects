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

	static LRESULT CALLBACK ConsoleProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	LRESULT HandleConsoleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const;
	static LRESULT CALLBACK InputProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	LRESULT HandleInputMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const;

public:
	Console();
	Console(HWND hParentWindow, HINSTANCE hParentInstance);
	virtual ~Console();

	void AppendText(const std::wstring& text) const;
	void SendText() const;
	INT_PTR SetConsoleColor(HDC hdcEdit) const;
	INT_PTR SetInputColor(HDC hdcEdit) const;
	void Cleanup();

	HWND GetConsoleWindow() const { return m_hConsoleWindow; }
	HWND GetInputWindow() const { return m_hInputWindow; }

};

#endif