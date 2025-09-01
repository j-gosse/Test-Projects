/*! \file       WinAPI_CreateWindow
    \version    1.2
    \desc	    Windows application for testing the creation of a window through use of the Windows API.
    \author     Jacob Gosse
    \date       September 1, 2025

    \MSVC       /std:c++20
    \GNUC       -m64 -std=c++20

   Copyright (c) 2025, Jacob Gosse

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "Window.hpp"
#include "KeyController.hpp"
#include <cstdio>
#include <conio.h>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    try
    {
        if (AllocConsole())
        {
            // Console successfully allocated
        }
        else
        {
            // Error allocating console
        }

        FILE* pCout = nullptr;
        FILE* pCin = nullptr;
        FILE* pCerr = nullptr;

        // Redirect stdout
        freopen_s(&pCout, "CONOUT$", "w", stdout);
        // Redirect stdin
        freopen_s(&pCin, "CONIN$", "r", stdin);
        // Redirect stderr
        freopen_s(&pCerr, "CONOUT$", "w", stderr);

        ULONGLONG startTime = GetTickCount64();
        ULONGLONG currentTime;
        ULONGLONG elapsedTime;

        std::unique_ptr<Window, std::default_delete<Window>> window = std::make_unique<Window>(hInstance);
        window->InitWindow();

        KeyController controller;
        window->GetKeyHandler().AddListener(&controller);

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

        window.reset();
        window = nullptr;

        std::wcout << L"Application exited successfully." << std::endl;

        std::cout << "Program finished. Press any key to continue..." << std::endl;
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