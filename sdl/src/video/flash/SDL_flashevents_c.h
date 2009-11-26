/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2006 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Sam Lantinga
    slouken@libsdl.org
*/
#include "SDL_config.h"

#include "SDL_flashvideo.h"

/* Variables and functions exported by SDL_sysevents.c to other parts 
   of the native video subsystem (SDL_sysvideo.c)
*/
extern void FLASH_InitOSKeymap(_THIS);
extern void FLASH_PumpEvents(_THIS);

extern AS3_Val FLASH_EMPTY_PARAMS;
extern AS3_Val FLASH_EVENT_MANAGER_OBJECT;
extern int FLASH_mouseX, FLASH_mouseY;



/* Scancodes */

#define SCANCODE_BACKSPACE		8
#define SCANCODE_TAB			9
#define SCANCODE_ENTER			13
#define SCANCODE_SHIFT			16
#define SCANCODE_CONTROL		17
#define SCANCODE_BREAK			19
#define SCANCODE_CAPSLOCK		20
#define SCANCODE_ESCAPE			27
#define SCANCODE_SPACE			32
#define SCANCODE_PAGEUP			33
#define SCANCODE_PAGEDOWN		34
#define SCANCODE_END			35
#define SCANCODE_HOME			36
#define SCANCODE_INSERT			45
#define SCANCODE_DELETE			46

#define SCANCODE_0			48
#define SCANCODE_1			49
#define SCANCODE_2			50
#define SCANCODE_3			51
#define SCANCODE_4			52
#define SCANCODE_5			53
#define SCANCODE_6			54
#define SCANCODE_7			55
#define SCANCODE_8			56
#define SCANCODE_9			57

#define SCANCODE_A			65
#define SCANCODE_B			66
#define SCANCODE_C			67
#define SCANCODE_D			68
#define SCANCODE_E			69
#define SCANCODE_F			70
#define SCANCODE_G			71
#define SCANCODE_H			72
#define SCANCODE_I			73
#define SCANCODE_J			74
#define SCANCODE_K			75
#define SCANCODE_L			76
#define SCANCODE_M			77
#define SCANCODE_N			78
#define SCANCODE_O			79
#define SCANCODE_P			80
#define SCANCODE_Q			81
#define SCANCODE_R			82
#define SCANCODE_S			83
#define SCANCODE_T			84
#define SCANCODE_U			85
#define SCANCODE_V			86
#define SCANCODE_W			87
#define SCANCODE_X			88
#define SCANCODE_Y			89
#define SCANCODE_Z			90

#define SCANCODE_KEYPAD0		96
#define SCANCODE_KEYPAD1		97
#define SCANCODE_KEYPAD2		98
#define SCANCODE_KEYPAD3		99
#define SCANCODE_KEYPAD4		100
#define SCANCODE_KEYPAD5		101
#define SCANCODE_KEYPAD6		102
#define SCANCODE_KEYPAD7		103
#define SCANCODE_KEYPAD8		104
#define SCANCODE_KEYPAD9		105
#define SCANCODE_KEYPADMULTIPLY		106
#define SCANCODE_KEYPADPLUS			107
#define SCANCODE_KEYPADMINUS		109
#define SCANCODE_KEYPADPERIOD		110
#define SCANCODE_KEYPADDIVIDE		111

#define SCANCODE_F1					112
#define SCANCODE_F2					113
#define SCANCODE_F3					114
#define SCANCODE_F4					115
#define SCANCODE_F5					116
#define SCANCODE_F6					117
#define SCANCODE_F7					118
#define SCANCODE_F8					119
#define SCANCODE_F9					120
//#define SCANCODE_F10 -- Reserved
#define SCANCODE_F11				122
#define SCANCODE_F12				123

#define SCANCODE_NUMLOCK		144
#define SCANCODE_SCROLLLOCK		145

#define SCANCODE_SEMICOLON		186
#define SCANCODE_EQUAL			187
#define SCANCODE_COMMA			188
#define SCANCODE_MINUS			189
#define SCANCODE_PERIOD			190
#define SCANCODE_SLASH			191

#define SCANCODE_BRACKET_LEFT		219
#define SCANCODE_BACKSLASH			220
#define SCANCODE_BRACKET_RIGHT		221
#define SCANCODE_APOSTROPHE			222