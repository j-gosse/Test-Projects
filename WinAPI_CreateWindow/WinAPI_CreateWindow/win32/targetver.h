// targetver.h : defines the application's targeted Windows build platform

/*
Including SDKDDKVer.h defines the highest available Windows platform.
If you wish to build your application for a previous Windows platform, include WinSDKVer.h and
set the _WIN32_WINNT macro to the platform you wish to support before including SDKDDKVer.h

Windows Version		Macro Value
-------------------------------
Windows 11			0x0A00
Windows 10			0x0A00
Windows 8.1			0x0603
Windows 8			0x0602
Windows 7			0x0601
Windows Vista		0x0600
Windows XP			0x0501
Windows 2000		0x0500
*/

#pragma once

#ifndef TARGETVER_H_
#define TARGETVER_H_

//#include <WinSDKVer.h>

//#define _WIN32_WINNT 0x0A00  // target Windows 10

#include <SDKDDKVer.h>

#endif
