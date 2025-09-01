// KeyHandler.hpp : Included in the Window class to create an object which dispatches messages sent by
// Windows in relation to key presses
//

#pragma once

#ifndef KEYHANDLER_HPP_
#define KEYHANDLER_HPP_

#include "IKeyListener.hpp"
#include <vector>
#include <algorithm>
#include <iostream>

class KeyHandler
{
private:
    std::vector<IKeyListener*> m_listeners;

public:
    KeyHandler() {}
    virtual ~KeyHandler() {}

    void AddListener(IKeyListener* listener)
    {
        m_listeners.push_back(listener);
    }

    void RemoveListener(IKeyListener* listener)
    {
        m_listeners.erase(std::remove(m_listeners.begin(), m_listeners.end(), listener), m_listeners.end());
    }

    void DispatchKeyDown(WPARAM key)
    {
        for (auto* l : m_listeners) l->OnKeyDown(key);
    }

    void DispatchKeyUp(WPARAM key)
    {
        for (auto* l : m_listeners) l->OnKeyUp(key);
    }

    void DispatchChar(WPARAM ch)
    {
        for (auto* l : m_listeners) l->OnChar(ch);
    }
};

//class KeyHandler : public IKeyListener
//{
//private:
//
//public:
//    void OnKeyDown(WPARAM key) override
//    {
//        std::cout << "Key Down: " << key << "\n";
//    }
//
//    void OnKeyUp(WPARAM key) override
//    {
//        std::cout << "Key Up: " << key << "\n";
//    }
//
//    void OnChar(WPARAM ch) override
//    {
//        std::cout << "Char: " << static_cast<char>(ch) << "\n";
//    }
//};

#endif