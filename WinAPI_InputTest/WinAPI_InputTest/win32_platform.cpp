/*! \file       WinAPI_InputTest
    \version    1.1
    \desc	    Windows desktop application for testing the handling of keyboard input through use of the Windows API.
    \author     Jacob Gosse
    \date       October 3, 2025

    \MSVC       /std:c++20
    \GNUC       -m64 -std=c++20

    Copyright (c) 2025, Jacob Gosse
    
    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at:
    
        http://www.apache.org/licenses/LICENSE-2.0
    
    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

    \note
    Keyboard Ghosting
    -----------------
    Keyboard ghosting is when a keyboard fails to register multiple simultaneous key presses because
    its internal electronics are unable to keep up with the number of inputs, often ignoring some key
    down/up events. This is known as N-Key Rollover, referring to the number of keys that can be pressed 
    simultaneously on a keyboard, and thus a low quality keyboard will be more prone to ghosting keys. 
    This can have an impact on overall testing of keyboard input, and should be kept in mind that some 
    issues with the handling of input may be related to the hardware itself rather than the program code.

    Keyboard Testing: https://keyboardchecker.com/
*/

#include "Window.hpp"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    try
    {
        // allocate console
        if (AllocConsole())
        {
            // redirect console input/output
            FILE* pCout = nullptr;
            FILE* pCin = nullptr;
            FILE* pCerr = nullptr;
            freopen_s(&pCout, "CONOUT$", "w", stdout);  // Redirect stdout
            freopen_s(&pCin, "CONIN$", "r", stdin);     // Redirect stdin
            freopen_s(&pCerr, "CONOUT$", "w", stderr);  // Redirect stderr

            std::wcout << L"Console successfully allocated." << L'\n';
        }
        else
        {
            throw std::runtime_error("Failed to allocate a console!");
        }

        #if defined(_DEBUG) && defined(_WIN32)
        {
            // enable CRT memory leak checking
            int dbgFlags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
            dbgFlags |= _CRTDBG_CHECK_ALWAYS_DF;
            dbgFlags |= _CRTDBG_DELAY_FREE_MEM_DF;
            dbgFlags |= _CRTDBG_LEAK_CHECK_DF;
            _CrtSetDbgFlag(dbgFlags);

            // redirect leak warnings to console and debug window
            _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE);
            _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
        }
        #endif

        // declare time values
        ULONGLONG startTime = GetTickCount64();
        ULONGLONG currentTime;
        ULONGLONG elapsedTime;

        // construct window
        std::unique_ptr<Window, std::default_delete<Window>> window = std::make_unique<Window>(hInstance);

        // main loop
        while (window->ProcessMessages())
        {
            currentTime = GetTickCount64();
            elapsedTime = currentTime - startTime;

            /*
            This is where the main loop logic or call to a update/render loop occurs.
            For example:
                obj->Update(window->GetWindow(), elapsedTime);
                obj->Render(window->GetWindow());
            */

            Sleep(16);  // simulate ~60 FPS
        }

        // destroy window
        window.reset();
        window = nullptr;

        // dump memory leaks if any occurred
        std::wcout << L'\n';
        if (!_CrtDumpMemoryLeaks())
        {
            std::wcout << L"No memory leaks detected." << std::endl;
        }

        std::wcout << L"\nProgram finished. Press any key to continue..." << std::endl;
        _getch(); // Waits for a single character input

        return 0;
    }
    catch (const std::exception& error)
    {
        std::wstring errMsg = std::wstring(L"Error: ") + std::wstring(error.what(), error.what() + strlen(error.what()));
        MessageBoxExW(nullptr, errMsg.c_str(), L"Fatal Error!", MB_OK | MB_ICONERROR, LANG_USER_DEFAULT);
        return -1;
    }
}