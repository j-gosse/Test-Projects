// IKeyListener.hpp : The functions in this class are overridden by the classes it is included with (other than KeyHandler) 
// which allows for decoupling the process of listening for key presses from the Window class
//

#pragma once

#ifndef IKEYLISTENER_HPP_
#define IKEYLISTENER_HPP_

#include "windows.h"

class IKeyListener
{
public:
    IKeyListener() {}
    virtual ~IKeyListener() = default;

    virtual void OnKeyDown(WPARAM key) = 0;
    virtual void OnKeyUp(WPARAM key) = 0;
    virtual void OnChar(WPARAM ch) = 0;
};

#endif