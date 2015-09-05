/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2015 Néstor Silveira Gorski

-------------------------------------------------------------------------------
This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/
#pragma once

#define     NSG_KEY_RELEASE     0
#define     NSG_KEY_PRESS       1
#define     NSG_KEY_REPEAT      2

#if defined(SDL)
#include "SDL_keycode.h"
#include "SDL_mouse.h"

#define     NSG_KEY_MOD_SHIFT       SDLK_LSHIFT
#define     NSG_KEY_MOD_CONTROL     SDLK_LCTRL
#define     NSG_KEY_MOD_ALT         SDLK_LALT

#define     NSG_KEY_SPACE   	SDLK_SPACE
#define     NSG_KEY_TAB         SDLK_TAB
#define     NSG_KEY_BACKSPACE   SDLK_BACKSPACE
#define     NSG_KEY_ENTER       SDLK_RETURN
#define     NSG_KEY_DELETE      SDLK_DELETE
#define     NSG_KEY_RIGHT       SDL_SCANCODE_RIGHT
#define     NSG_KEY_LEFT        SDL_SCANCODE_LEFT
#define     NSG_KEY_UP        	SDL_SCANCODE_UP
#define     NSG_KEY_DOWN        SDL_SCANCODE_DOWN
#define     NSG_KEY_HOME        SDLK_HOME
#define     NSG_KEY_END         SDLK_END

#define     NSG_KEY_A         SDLK_a
#define     NSG_KEY_C         SDLK_c
#define     NSG_KEY_D         SDLK_d
#define     NSG_KEY_E         SDLK_e
#define     NSG_KEY_F         SDLK_f
#define     NSG_KEY_G         SDLK_g
#define     NSG_KEY_M         SDLK_m
#define     NSG_KEY_P         SDLK_p
#define     NSG_KEY_Q         SDLK_q
#define     NSG_KEY_R         SDLK_r
#define     NSG_KEY_S         SDLK_s
#define     NSG_KEY_W         SDLK_w
#define     NSG_KEY_Z         SDLK_z
#define     NSG_KEY_LALT      SDL_SCANCODE_LALT//SDLK_LALT
#define     NSG_KEY_LSHIFT    SDL_SCANCODE_LSHIFT//SDLK_LSHIFT

#define     NSG_KEY_0		SDLK_0
#define     NSG_KEY_1		SDLK_1
#define     NSG_KEY_2		SDLK_2
#define     NSG_KEY_3		SDLK_3
#define     NSG_KEY_4		SDLK_4
#define     NSG_KEY_5		SDLK_5
#define     NSG_KEY_6		SDLK_6
#define     NSG_KEY_7		SDLK_7
#define     NSG_KEY_8		SDLK_8
#define     NSG_KEY_9		SDLK_9


#define     NSG_BUTTON_LEFT     SDL_BUTTON_LEFT
#define     NSG_BUTTON_MIDDLE   SDL_BUTTON_MIDDLE
#define     NSG_BUTTON_RIGHT    SDL_BUTTON_RIGHT

#else

#error("Unknown platform!!!")

#endif
