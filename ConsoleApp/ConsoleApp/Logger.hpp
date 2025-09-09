#pragma once

#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#define LOG_MSG(x, y)	Logger::Log((x), (y))
#define LOG_SEND(x, y)	Logger::Send((x), (y))
#define LOG_ERR(x, y)	Logger::Error((x), (y))

#include <windows.h>
#include <string>
#include <string_view>		// std::string_view - Used for outputting the logger messages (C++17)
#include <source_location>	// std::source_location - Provides a source location in order to determine where errors originate from (C++20)

class Logger
{
private:
	static std::wstring CurrentDate();

public:
	Logger();
	virtual ~Logger();

	static void Log(HWND hConsoleWindow, const std::wstring& text);
	static void Send(HWND hInputWindow, HWND hConsoleWindow);
	static void Error(HWND hConsoleWindow, const std::wstring& text, std::source_location location = std::source_location::current());
};

#endif