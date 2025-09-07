#pragma once

#ifndef CONSOLE_HPP_
#define CONSOLE_HPP_

#include "framework.h"
#include "resource.h"

#include <cstdio>
#include <stdexcept>
#include <iostream>

class Console
{
private:
	HWND m_hParentWindow;
	HWND m_hConsoleWindow;
	HWND m_hInputWindow;
	HINSTANCE m_hParentInstance;

public:
	Console();
	Console(HWND hParentWindow, HINSTANCE hParentInstance);
	virtual ~Console();

	void AppendText(const std::wstring& text) const;
	void SendText() const;
	void Cleanup();

};

#endif