/*! \file       ConsoleApp
    \version    1.3
    \desc	    Windows desktop application that emulates a console for output and creates an input bar for user input.
    \author     Jacob Gosse
    \date       September 6, 2025

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
*/

#include "Window.hpp"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    try
    {
        if (AllocConsole())
        {
            // Console successfully allocated
            std::wcout << L"Console successfully allocated." << L'\n';
        }
        else
        {
            // Error allocating console
            throw std::runtime_error("Error allocating console!");
        }

        FILE* pCout = nullptr;
        FILE* pCin = nullptr;
        FILE* pCerr = nullptr;
        freopen_s(&pCout, "CONOUT$", "w", stdout);  // Redirect stdout
        freopen_s(&pCin, "CONIN$", "r", stdin);     // Redirect stdin
        freopen_s(&pCerr, "CONOUT$", "w", stderr);  // Redirect stderr

        ULONGLONG startTime = GetTickCount64();
        ULONGLONG currentTime;
        ULONGLONG elapsedTime;

        std::unique_ptr<Window, std::default_delete<Window>> window = std::make_unique<Window>(hInstance);
        window->InitWindow();

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

        std::cout << "Program finished. Press any key to continue..." << std::endl;
        _getch(); // Waits for a single character input

        // memory leak checking code
        //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

        return 0;
    }
    catch (const std::exception& error)
    {
        std::wstring errMsg = std::wstring(L"Error: ") + std::wstring(error.what(), error.what() + strlen(error.what()));
        MessageBoxExW(nullptr, errMsg.c_str(), L"Fatal Error!", MB_OK | MB_ICONERROR, LANG_USER_DEFAULT);
        return -1;
    }
}