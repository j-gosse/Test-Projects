#include "Logger.hpp"

#include <format>
#include <chrono>
#include <ctime>
#include <iostream>
#include <vector>

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
    /*
    std::chrono::current_zone() is C++20's time zone support, which is part of the <chrono> library
    using current_zone() will load IANA time zone database strings which will slow down initial loading 
    of a program and are not explicitly cleaned upon a program's exit
    */
    //auto now = std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now());
    //auto time = std::chrono::current_zone()->to_local(now);
    //return std::format(L"{:%y-%m-%d %H:%M:%S}", time);

    std::chrono::time_point<std::chrono::system_clock, std::chrono::system_clock::duration> now =
        std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now());

    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::tm localTime;
    localtime_s(&localTime, &time);  // Thread-safe on Windows

    wchar_t buffer[100];
    std::wcsftime(buffer, sizeof(buffer) / sizeof(wchar_t), L"%y-%m-%d %H:%M:%S", &localTime);

    return std::wstring(buffer);
}

void Logger::Log(HWND hConsoleWindow, const std::wstring& text)
{
    std::wstringstream ss;
    ss << Logger::CurrentDate() << L" - " << text << L"\r\n";
    std::wstring msg = ss.str();

    int len = GetWindowTextLengthW(hConsoleWindow);
    SendMessageW(hConsoleWindow, EM_SETSEL, len, len);
    SendMessageW(hConsoleWindow, EM_REPLACESEL, FALSE, (LPARAM)msg.c_str());
}

void Logger::Send(HWND hInputWindow, HWND hConsoleWindow)
{
    int charLength = GetWindowTextLengthW(hInputWindow);
    if (charLength <= 0) return;

    std::vector<wchar_t> buffer(charLength + 1);
    if (!GetWindowTextW(hInputWindow, buffer.data(), charLength + 1)) return;

    std::wstringstream ss;
    ss << buffer.data();

    Logger::Log(hConsoleWindow, ss.str());
    SetWindowTextW(hInputWindow, L"");
}

void Logger::Error(HWND hConsoleWindow, const std::wstring& text, std::source_location location)
{
    std::wstringstream ss;
    ss << L"ERROR: " << text << L" FILE: " << location.file_name() << L" FUNC: " << location.function_name() << L" LINE: " << location.line();
    
    Logger::Log(hConsoleWindow, ss.str());
}