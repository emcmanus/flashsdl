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

#include "SDL.h"
#include "../../events/SDL_sysevents.h"
#include "../../events/SDL_events_c.h"

#include "SDL_flashvideo.h"
#include "SDL_flashevents_c.h"

#include "AS3.h"

static SDL_keysym *TranslateKey(int scancode, SDL_keysym *keysym);
static SDLKey keymap[256];

AS3_Val FLASH_EVENT_MANAGER_OBJECT;
AS3_Val FLASH_EMPTY_PARAMS;
int FLASH_mouseX, FLASH_mouseY;

// Go check a Flash event queue for events SDL should be aware of!
void FLASH_PumpEvents(_THIS)
{
	if (!FLASH_EMPTY_PARAMS)
		FLASH_EMPTY_PARAMS = AS3_Array("");
	
	// Event Vars
	AS3_Val mouseEvent, mouseEvents, mousePosition, keyboardEvent, keyboardEvents;
	int buttonState, rawKeyboardEvent, scanCode, keyState;
	SDL_keysym keysym;
	
	mousePosition = AS3_CallS( "pumpMousePosition", FLASH_EVENT_MANAGER_OBJECT, FLASH_EMPTY_PARAMS );
	mouseEvents = AS3_CallS( "pumpMouseEvents", FLASH_EVENT_MANAGER_OBJECT, FLASH_EMPTY_PARAMS );
	keyboardEvents = AS3_CallS( "pumpKeyEvents", FLASH_EVENT_MANAGER_OBJECT, FLASH_EMPTY_PARAMS );
	
	// Mouse Position
	if (mousePosition) {
		AS3_ObjectValue( mousePosition, "x:IntType, y:IntType", &FLASH_mouseX, &FLASH_mouseY );	
		SDL_PrivateMouseMotion( 0, 0, FLASH_mouseX, FLASH_mouseY );
	}
	
	// Mouse Click Events
	while( AS3_IntValue(AS3_GetS(mouseEvents, "size")) > 0 ) {
		mouseEvent = AS3_CallS( "dequeue", mouseEvents, FLASH_EMPTY_PARAMS );
		buttonState = AS3_IntValue( mouseEvent )? SDL_PRESSED: SDL_RELEASED;
		SDL_PrivateMouseButton(buttonState, SDL_BUTTON_LEFT, FLASH_mouseX, FLASH_mouseY);
	}
	
	// Keyboard Events
//	sztrace("keyboard events");
//	AS3_Trace(AS3_GetS(keyboardEvents, "size"));
	while( AS3_IntValue(AS3_GetS(keyboardEvents, "size")) > 0 ) {
		keyboardEvent = AS3_CallS( "dequeue", keyboardEvents, FLASH_EMPTY_PARAMS );
		rawKeyboardEvent = AS3_IntValue( keyboardEvent );
		scanCode = rawKeyboardEvent & 0xFF;
		keyState = rawKeyboardEvent >> 8;
		SDL_PrivateKeyboard(keyState, TranslateKey(scanCode, &keysym));
	}
}

// TODO: Figure out if SDL handles shift key translation! (Eg, ; + shift = :)
void FLASH_InitOSKeymap(_THIS)
{
	int i;

	/* Initialize the Flash key translation table */
	for ( i=0; i<SDL_arraysize(keymap); ++i )
		keymap[i] = SDLK_UNKNOWN;
	
	keymap[SCANCODE_BACKSPACE] = SDLK_BACKSPACE;
	keymap[SCANCODE_TAB] = SDLK_TAB;
	keymap[SCANCODE_ENTER] = SDLK_RETURN;
	keymap[SCANCODE_SHIFT] = SDLK_LSHIFT;
	keymap[SCANCODE_CONTROL] = SDLK_LCTRL;
	keymap[SCANCODE_BREAK] = SDLK_BREAK;
	keymap[SCANCODE_CAPSLOCK] = SDLK_CAPSLOCK;
	keymap[SCANCODE_ESCAPE] = SDLK_ESCAPE;
	keymap[SCANCODE_SPACE] = SDLK_SPACE;
	keymap[SCANCODE_PAGEUP] = SDLK_PAGEUP;
	keymap[SCANCODE_PAGEDOWN] = SDLK_PAGEDOWN;
	keymap[SCANCODE_END] = SDLK_END;
	keymap[SCANCODE_HOME] = SDLK_HOME;
	keymap[SCANCODE_INSERT] = SDLK_INSERT;
	keymap[SCANCODE_DELETE] = SDLK_DELETE;
	
	keymap[SCANCODE_0] = SDLK_0;
	keymap[SCANCODE_1] = SDLK_1;
	keymap[SCANCODE_2] = SDLK_2;
	keymap[SCANCODE_3] = SDLK_3;
	keymap[SCANCODE_4] = SDLK_4;
	keymap[SCANCODE_5] = SDLK_5;
	keymap[SCANCODE_6] = SDLK_6;
	keymap[SCANCODE_7] = SDLK_7;
	keymap[SCANCODE_8] = SDLK_8;
	keymap[SCANCODE_9] = SDLK_9;
	
	keymap[SCANCODE_A] = SDLK_a;
	keymap[SCANCODE_B] = SDLK_b;
	keymap[SCANCODE_C] = SDLK_c;
	keymap[SCANCODE_D] = SDLK_d;
	keymap[SCANCODE_E] = SDLK_e;
	keymap[SCANCODE_F] = SDLK_f;
	keymap[SCANCODE_G] = SDLK_g;
	keymap[SCANCODE_H] = SDLK_h;
	keymap[SCANCODE_I] = SDLK_i;
	keymap[SCANCODE_J] = SDLK_j;
	keymap[SCANCODE_K] = SDLK_k;
	keymap[SCANCODE_L] = SDLK_l;
	keymap[SCANCODE_M] = SDLK_m;
	keymap[SCANCODE_N] = SDLK_n;
	keymap[SCANCODE_O] = SDLK_o;
	keymap[SCANCODE_P] = SDLK_p;
	keymap[SCANCODE_Q] = SDLK_q;
	keymap[SCANCODE_R] = SDLK_r;
	keymap[SCANCODE_S] = SDLK_s;
	keymap[SCANCODE_T] = SDLK_t;
	keymap[SCANCODE_U] = SDLK_u;
	keymap[SCANCODE_V] = SDLK_v;
	keymap[SCANCODE_W] = SDLK_w;
	keymap[SCANCODE_X] = SDLK_x;
	keymap[SCANCODE_Y] = SDLK_y;
	keymap[SCANCODE_Z] = SDLK_z;
	
	keymap[SCANCODE_KEYPAD0] = SDLK_KP0;
	keymap[SCANCODE_KEYPAD1] = SDLK_KP1;
	keymap[SCANCODE_KEYPAD2] = SDLK_KP2;
	keymap[SCANCODE_KEYPAD3] = SDLK_KP3;
	keymap[SCANCODE_KEYPAD4] = SDLK_KP4;
	keymap[SCANCODE_KEYPAD5] = SDLK_KP5;
	keymap[SCANCODE_KEYPAD6] = SDLK_KP6;
	keymap[SCANCODE_KEYPAD7] = SDLK_KP7;
	keymap[SCANCODE_KEYPAD8] = SDLK_KP8;
	keymap[SCANCODE_KEYPAD9] = SDLK_KP9;
	keymap[SCANCODE_KEYPADMULTIPLY] = SDLK_KP_MULTIPLY;
	keymap[SCANCODE_KEYPADPLUS] = SDLK_KP_PLUS;
	keymap[SCANCODE_KEYPADMINUS] = SDLK_KP_MINUS;
	keymap[SCANCODE_KEYPADPERIOD] = SDLK_KP_PERIOD;
	keymap[SCANCODE_KEYPADDIVIDE] = SDLK_KP_DIVIDE;
	
	keymap[SCANCODE_F1] = SDLK_F1;
	keymap[SCANCODE_F2] = SDLK_F2;
	keymap[SCANCODE_F3] = SDLK_F3;
	keymap[SCANCODE_F4] = SDLK_F4;
	keymap[SCANCODE_F5] = SDLK_F5;
	keymap[SCANCODE_F6] = SDLK_F6;
	keymap[SCANCODE_F7] = SDLK_F7;
	keymap[SCANCODE_F8] = SDLK_F8;
	keymap[SCANCODE_F9] = SDLK_F9;
	//keymap[SCANCODE_F10 -- Reserved
	keymap[SCANCODE_F11] = SDLK_F11;
	keymap[SCANCODE_F12] = SDLK_F12;
	
	keymap[SCANCODE_NUMLOCK] = SDLK_NUMLOCK;
	keymap[SCANCODE_SCROLLLOCK] = SDLK_SCROLLOCK;
	
	keymap[SCANCODE_SEMICOLON] = SDLK_SEMICOLON;
	keymap[SCANCODE_EQUAL] = SDLK_EQUALS;
	keymap[SCANCODE_COMMA] = SDLK_COMMA;
	keymap[SCANCODE_MINUS] = SDLK_MINUS;
	keymap[SCANCODE_PERIOD] = SDLK_PERIOD;
	keymap[SCANCODE_SLASH] = SDLK_SLASH;
	
	keymap[SCANCODE_BRACKET_LEFT] = SDLK_LEFTBRACKET;
	keymap[SCANCODE_BACKSLASH] = SDLK_QUOTEDBL;
	keymap[SCANCODE_BRACKET_RIGHT] = SDLK_RIGHTBRACKET;
	keymap[SCANCODE_APOSTROPHE] = SDLK_QUOTE;
}

static SDL_keysym *TranslateKey(int scancode, SDL_keysym *keysym)
{
	/* Set the keysym information */
	keysym->scancode = scancode;
	keysym->sym = keymap[scancode];
	keysym->mod = KMOD_NONE;
	keysym->unicode = 0;
	return(keysym);
}
