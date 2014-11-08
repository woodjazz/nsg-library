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

#if defined(NACL)

#include "ppapi/c/ppb_input_event.h"
#define     NSG_KEY_MOD_SHIFT       PP_INPUTEVENT_MODIFIER_SHIFTKEY
#define     NSG_KEY_MOD_CONTROL     PP_INPUTEVENT_MODIFIER_CONTROLKEY
#define     NSG_KEY_MOD_ALT         PP_INPUTEVENT_MODIFIER_ALTKEY

// Same as Windows (Virtual-key Codes)
// http://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx
#define     NSG_KEY_TAB         0x09
#define     NSG_KEY_BACKSPACE   0x08
#define     NSG_KEY_ENTER       0x0D
#define     NSG_KEY_DELETE      0x2E
#define     NSG_KEY_RIGHT       0x27
#define     NSG_KEY_LEFT        0x25
#define     NSG_KEY_HOME        0x24
#define     NSG_KEY_END         0x23

#define     NSG_KEY_A         0x41
#define     NSG_KEY_C         0x43
#define     NSG_KEY_D         0x44
#define     NSG_KEY_E         0x45
#define     NSG_KEY_F         0x46
#define     NSG_KEY_W         0x57
#define     NSG_KEY_S         0x53
#define     NSG_KEY_Q         0x51
#define     NSG_KEY_LALT      0x12
#define     NSG_KEY_LSHIFT    0xA0


#define     NSG_BUTTON_LEFT     PP_INPUTEVENT_MOUSEBUTTON_LEFT
#define     NSG_BUTTON_MIDDLE   PP_INPUTEVENT_MOUSEBUTTON_MIDDLE
#define     NSG_BUTTON_RIGHT    PP_INPUTEVENT_MOUSEBUTTON_RIGHT


#elif defined(SDL)
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
#define     NSG_KEY_LSHIFT    SDLK_LSHIFT


#define     NSG_BUTTON_LEFT     SDL_BUTTON_LEFT
#define     NSG_BUTTON_MIDDLE   SDL_BUTTON_MIDDLE
#define     NSG_BUTTON_RIGHT    SDL_BUTTON_RIGHT

#else

#error("Unknown platform!!!")

#endif
