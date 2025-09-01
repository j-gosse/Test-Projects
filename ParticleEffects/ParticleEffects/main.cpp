/*! \file       ParticleEffects
    \version    1.5
    \desc	    Windows application for testing various particle rendering effects.
    \author     Jacob Gosse
    \date       August 28, 2025

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

   \note
    SINE & COSINE
    -------------
    sin() - Returns the sine (-1 to 1) of an angle of x radians, one radian is equal to 180 / PI degrees
    cos() - Returns the cosine (0 to 1) of an angle of x radians, one radian is equal to 180 / PI degrees

    e.g.
    constexpr auto PI = 3.14159265;
    double param, result;
    param = 30.0;
    result = sin(param * PI / 180);
    printf("The sine of %f degrees is %f.\n", param, result);

    HEX VALUES
    ----------
    Red = 0xFF0000FF
    Green = 0x00FF00FF
    Blue = 0x0000FFFF
    255 = 0xFF

    COLORS & BIT SHIFTING
    ---------------------
    uint8_t red = static_cast<uint8_t>((1 + std::sin(elapsedTime * 0.001 + 0.0)) * 128);
    uint8_t green = static_cast<uint8_t>((1 + std::sin(elapsedTime * 0.001 + 2.0)) * 128);
    uint8_t blue = static_cast<uint8_t>((1 + std::sin(elapsedTime * 0.001 + 4.0)) * 128);

    uint32_t color = (0xFF << 24) | (red << 16) | (green << 8) | blue;	// ARGB
    uint32_t color = (red << 24) | (green << 16) | (blue << 8) | 0xFF;	// RGBA

    uint32_t color = 0;												    // RGBA8888
    color += red;
    color <<= 8;
    color += green;
    color <<= 8;
    color += blue;
    color <<= 8;
    color += 0xFF;

    RENDERING PIXELS
    ----------------
    // create a vector to hold the pixel data, vector is of type uint32_t because a color value consists of 32 bits
    std::vector<uint32_t> m_buffer;

    // allocate memory to pixel buffer
    m_buffer.resize(static_cast<unsigned long long>(width) * height * 4);
    m_buffer = std::vector<uint32_t>(WINDOW_WIDTH * WINDOW_HEIGHT);

    // clear pixel buffer
    std::fill(m_buffer.begin(), m_buffer.end(), 0xFF000000);

    // DrawPixel accepts x,y positions, width, height, and color. Each position (pixel) in the buffer is assigned a color
    void DrawPixel(int x, int y, int width, int height, uint32_t color)
    {
        // check to ensure pixels are not drawn outside of the window
        if (x < 0 || x >= width || y < 0 || y >= height) return;

        m_buffer[(static_cast<std::vector<uint32_t, std::allocator<uint32_t>>::size_type>(y) * width) + x] = color;
    }

    // Loop x,y values relative to window width and height, use as coordinates to fill the screen with the color
    for (int y = 0; y < WINDOW_HEIGHT; y++) // y position
    {
        for (int x = 0; x < WINDOW_WIDTH; x++) // x position
        {
            DrawPixel(x, y, color);
        }
    }

    // Loop the total number of particles (pixels) and draw the x,y position of each to the buffer
    for (int i = 0; i < NPARTICLES; i++)
    {
        m_particles[i].Update();

        int x = static_cast<int>((m_particles[i].GetX() + 1) * windowWidth / 2.0f);
        int y = static_cast<int>((m_particles[i].GetY() + 1) * windowHeight / 2.0f);

        DrawPixel(x, y, windowWidth, windowHeight, color);
    }

    WINDOWS HANDLES
    ---------------
    In Windows API, a handle acts as a token or a key that allows an application to interact with a specific
    resource managed by the operating system. Handles in general are abstractions which hide a real memory
    address from the API user, allowing the system to analyze and reorganize transparently to the program.
    They are essentially treated as pointers by which resolving a handle locks it to a specific
    memory address, and releasing the handle invalidates the pointer to that memory address.

    WINDOW CLASS PROPERTIES
    -----------------------
    1. m_windowClass.cbSize = sizeof(WNDCLASSEX);
        cbSize signifies a "count of bytes" (cb) specifying the size of the structure.

    2. m_windowClass.lpfnWndProc = Window::WindowProc;
        lpfn: long pointer to a function
        WndProc: Window Procedures

        The lpfnWndProc member points to an application-defined callback function that processes messages
        sent to windows belonging to a specific window class.

        This function defines the behavior of the window, including how it responds to user input
        (like mouse clicks and keyboard presses), system events (like painting or resizing), and other messages.

        When you register a window class using RegisterClass (or RegisterClassEx), you fill a WNDCLASS (or WNDCLASSEX) structure.
        The lpfnWndProc member of this structure is set to the address of your custom window procedure function.
        When a window belonging to this class receives a message, the operating system calls the function pointed
        to by lpfnWndProc to handle that message.

    3. m_windowClass.hInstance = m_hInstance;
        A handle to an instance. This is the base address of the module in memory. The hInstance member in WNDCLASS(EX) determines
        which module owns the registered class. When an application needs to load resources (like icons, cursors, or string tables)
        that are embedded within its .exe or an associated .dll, HINSTANCE is used to specify which module contains those resources.

    4. m_windowClass.lpszClassName = L"WindowClass";
        lpsz: long pointer to a null-terminated string
        String used to uniquely identify a Window class. The class name is a required parameter when unregistering the class
        through the use of UnregisterClassA/UnregisterClassW.

    5. m_windowClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    6. m_windowClass.hIcon = LoadIconW(nullptr, IDI_APPLICATION);
    7. m_windowClass.hIconSm = LoadIconW(nullptr, IDI_APPLICATION);
    8. m_windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);			// white background
       m_windowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);	// black background
*/

// TODO: use custom deleter?
/*
struct MyDeleter
{
    void operator()(Window* ptr) const
    {
        std::cout << "Destroying app pointer." << std::endl;
        delete ptr;
        ptr = nullptr;
    }
};
std::unique_ptr<Window, MyDeleter> ptr;
//std::unique_ptr<Window, MyDeleter> window(new Window(), MyDeleter{});
*/

#include "Window.hpp"
#include "Swarm.hpp"

int main()
{
    try
    {
        ULONGLONG startTime = GetTickCount64();
        ULONGLONG currentTime;
        ULONGLONG elapsedTime;

        std::unique_ptr<Window, std::default_delete<Window>> window = std::make_unique<Window>();
        window->Init();

        std::unique_ptr<Swarm, std::default_delete<Swarm>> swarm = std::make_unique<Swarm>();
        swarm->Init(window->GetWindow());

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

            swarm->Update(window->GetWindow(), elapsedTime);
            swarm->Render(window->GetWindow());

            Sleep(16);  // simulate ~60 FPS
        }

        swarm.reset();
        swarm = nullptr;

        window.reset();
        window = nullptr;

        std::wcout << L"Application exited successfully." << std::endl;

        return 0;
    }
    catch (const std::exception& error)
    {
        std::wstring errMsg = std::wstring(L"Error: ") + std::wstring(error.what(), error.what() + strlen(error.what()));
        MessageBoxExW(nullptr, errMsg.c_str(), L"Fatal Error!", MB_OK | MB_ICONERROR, LANG_USER_DEFAULT);
        return -1;
    }
}