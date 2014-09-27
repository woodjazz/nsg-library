/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://nsg-library.googlecode.com/

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

#if !defined(SDL)
#define     NSG_KEY_MOD_SHIFT       0x0001
#define     NSG_KEY_MOD_CONTROL     0x0002
#define     NSG_KEY_MOD_ALT         0x0004

#define     NSG_KEY_TAB         258
#define     NSG_KEY_BACKSPACE   259
#define     NSG_KEY_ENTER       257
#define     NSG_KEY_DELETE      261
#define     NSG_KEY_RIGHT       262
#define     NSG_KEY_LEFT        263
#define     NSG_KEY_HOME        268
#define     NSG_KEY_END         269

#else
#include "SDL_keycode.h"
#include "SDL_mouse.h"

#define     NSG_KEY_MOD_SHIFT       SDLK_LSHIFT
#define     NSG_KEY_MOD_CONTROL     SDLK_LCTRL
#define     NSG_KEY_MOD_ALT         SDLK_LALT

#define     NSG_KEY_TAB         SDLK_TAB
#define     NSG_KEY_BACKSPACE   SDLK_BACKSPACE
#define     NSG_KEY_ENTER       SDLK_RETURN
#define     NSG_KEY_DELETE      SDLK_DELETE
#define     NSG_KEY_RIGHT       SDLK_RIGHT
#define     NSG_KEY_LEFT        SDLK_LEFT
#define     NSG_KEY_HOME        SDLK_HOME
#define     NSG_KEY_END         SDLK_END

#define     NSG_KEY_A         SDLK_a
#define     NSG_KEY_C         SDLK_c
#define     NSG_KEY_D         SDLK_d
#define     NSG_KEY_E         SDLK_e
#define     NSG_KEY_F         SDLK_f
#define     NSG_KEY_W         SDLK_w
#define     NSG_KEY_S         SDLK_s
#define     NSG_KEY_Q         SDLK_q
#define     NSG_KEY_LALT      SDLK_LALT


#define     NSG_BUTTON_LEFT     SDL_BUTTON_LEFT
#define     NSG_BUTTON_MIDDLE   SDL_BUTTON_MIDDLE
#define     NSG_BUTTON_RIGHT    SDL_BUTTON_RIGHT



#endif
