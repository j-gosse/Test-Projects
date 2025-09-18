// framework.h : include file for standard system include files, or project specific include files

#pragma once

#ifndef FRAMEWORK_H_
#define FRAMEWORK_H_

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN	// Exclude rarely-used stuff from Windows headers

// Windows Headers
#include <windows.h>
#include <io.h>
#include <conio.h>	// _getch()
#include <lmcons.h>	// UNLEN and MAX_COMPUTERNAME_LENGTH

// C RunTime Headers
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// C Standard Library Headers
#include <stdio.h>
#include <fcntl.h>

// C++ Standard Library Headers
#include <string>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <cstdio>
#include <thread>

#endif
