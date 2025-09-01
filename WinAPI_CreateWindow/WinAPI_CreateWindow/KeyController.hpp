// KeyController.hpp : example of how the KeyListener and KeyHandler classes can be utilized 
// in conjunction with Window messages in order to listen for key presses from any class
// 

#pragma once

#ifndef KEYCONTROLLER_HPP_
#define KEYCONTROLLER_HPP_

#include "IKeyListener.hpp"
#include <iostream>

class KeyController : public IKeyListener
{
public:
    KeyController() {}
    virtual ~KeyController() {}

    void OnKeyDown(WPARAM key) override
    {
        std::wcout << L"Key Down: " << key << "\n";

        switch (key)
        {
        case 'W':
            std::cout << "Pressed W\n";
            break;
        case 'A':
            std::cout << "Pressed A\n";
            break;
        case 'S':
            std::cout << "Pressed S\n";
            break;
        case 'D':
            std::cout << "Pressed D\n";
            break;
        case VK_SPACE:
            std::cout << "Pressed Spacebar\n";
            break;
        default:
            break;
        }
    }

    void OnKeyUp(WPARAM key) override
    {
        std::wcout << L"Key Up: " << key << "\n";
    }

    void OnChar(WPARAM ch) override
    {
        std::wcout << L"Char: " << (char)ch << "\n";
    }
};

#endif