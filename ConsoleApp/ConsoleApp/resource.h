// resource.h : {{NO_DEPENDENCIES}}
// Microsoft Visual C++ generated include file.
// Used by resource_script.rc

/*
ID Range		Description
---------------------------
1 – 99			Reserved system default IDs

100 – 32767		Static resource IDs
					- IDR_: Menus, toolbars, accelerators, static icons
					- IDS_: String table entries (localized text)
					- IDD_: Dialog templates
					- IDI_: Icons

				Dialog control IDs (typically start at id 1000+)
					- IDC_:	Controls inside dialogs (Buttons, Edit controls, Checkboxes, Radio buttons, Static texts, etc.)

32768 - 57343	Command IDs
					- IDM_, ID_: Menu commands, toolbar button commands, accelerator keys

57344 – 65535	System-reserved and predefined Windows IDs
					- Typically reserved for predefined commands, accelerators, or system use. Avoid using.

65535			Maximum 16-bit unsigned integer (Limit for resource IDs)
*/

#pragma once

#ifndef RESOURCE_H_
#define RESOURCE_H_

// static resource IDs
#define IDR_MAINFRAME		100
#define IDR_ACCELERATOR		102

#define IDS_WINDOW_TITLE	103
#define IDS_WINDOW_CLASS	104

#define IDD_ABOUTBOX		105

#define IDI_PRIMARY			106
#define IDI_SMALL			107

// dialog control IDs


// command IDs
#define IDM_ABOUT			32768
#define IDM_EXIT			32769
#define ID_EDIT_OUTPUT		32770
#define ID_EDIT_INPUT		32771
#define ID_BUTTON_SEND		32772

// default static control ID
#ifndef IDC_STATIC
#define IDC_STATIC				-1
#endif

// Next default ID values for new objects
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NO_MFC					130
#define _APS_NEXT_RESOURCE_VALUE	129
#define _APS_NEXT_COMMAND_VALUE		32771
#define _APS_NEXT_CONTROL_VALUE		1000
#define _APS_NEXT_SYMED_VALUE		110
#endif
#endif

#endif
