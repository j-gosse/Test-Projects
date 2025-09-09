#include "Logger.hpp"

#include <format>
#include <chrono>
#include <ctime>
#include <iostream>

/* CONSTRUCTOR */

Logger::Logger()
{
    std::wcout << L"CONSTRUCTOR: Logger()" << L'\n';
}

/* DESTRUCTOR */

Logger::~Logger()
{
    std::wcout << L"DESTRUCTOR: ~Logger()" << L'\n';
}

/* FUNCTION DEFINITIONS */

std::wstring Logger::CurrentDate()
{
    auto now = std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now());
    auto time = std::chrono::current_zone()->to_local(now);
    return std::format(L"{:%y-%m-%d %H:%M:%S}", time);
}

void Logger::Log(HWND hConsoleWindow, const std::wstring& text)
{
    std::wstringstream ss;
    ss << CurrentDate() << L" - " << text << L"\r\n";
    std::wstring msg = ss.str();

    int len = GetWindowTextLengthW(hConsoleWindow);
    SendMessageW(hConsoleWindow, EM_SETSEL, len, len);
    SendMessageW(hConsoleWindow, EM_REPLACESEL, FALSE, (LPARAM)msg.c_str());
}

void Logger::Send(HWND hInputWindow, HWND hConsoleWindow)
{
    wchar_t inputBuffer[256] = {};
    if (!GetWindowTextW(hInputWindow, inputBuffer, sizeof(inputBuffer) / sizeof(wchar_t))) return;

    std::wstringstream ss;
    ss << inputBuffer;

    Logger::Log(hConsoleWindow, ss.str());
    SetWindowTextW(hInputWindow, L"");
    SetFocus(hInputWindow);
}

void Logger::Error(HWND hConsoleWindow, const std::wstring& text, std::source_location location)
{
    std::wstringstream ss;
    ss << L"ERROR: " << CurrentDate() << " - " << text << L" FILE: " << location.file_name() << L" FUNC: " << location.function_name() << L" LINE: " << location.line() << L"\r\n";
    
    Logger::Log(hConsoleWindow, ss.str());
}