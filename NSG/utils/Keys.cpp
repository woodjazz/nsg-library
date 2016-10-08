/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2016 Néstor Silveira Gorski

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
#include "Keys.h"

namespace NSG
{
    int MapKey(int key)
    {
        switch (key)
        {
                #if defined(SDL) || defined(EMSCRIPTEN)
            case SDLK_ESCAPE:
                return NSG_KEY_ESC;
            case SDLK_SPACE:
                return NSG_KEY_SPACE;
            case SDLK_TAB:
                return NSG_KEY_TAB;
            case SDLK_BACKSPACE:
                return NSG_KEY_BACKSPACE;
            case SDLK_RETURN:
                return NSG_KEY_ENTER;
            case SDLK_DELETE:
                return NSG_KEY_DELETE;
            case SDL_SCANCODE_RIGHT:
                return NSG_KEY_RIGHT;
            case SDL_SCANCODE_LEFT:
                return NSG_KEY_LEFT;
            case SDL_SCANCODE_UP:
                return NSG_KEY_UP;
            case SDL_SCANCODE_DOWN:
                return NSG_KEY_DOWN;
            case SDLK_HOME:
                return NSG_KEY_HOME;
            case SDLK_END:
                return NSG_KEY_END;
            case SDL_SCANCODE_PAGEUP:
                return NSG_KEY_PAGEUP;
            case SDL_SCANCODE_PAGEDOWN:
                return NSG_KEY_PAGEDOWN;
                #elif defined(IS_TARGET_WINDOWS)
            case VK_ESCAPE:
                return NSG_KEY_ESC;
            case VK_SPACE:
                return NSG_KEY_SPACE;
            case VK_TAB:
                return NSG_KEY_TAB;
            case VK_BACK:
                return NSG_KEY_BACKSPACE;
            case VK_RETURN:
                return NSG_KEY_ENTER;
            case VK_DELETE:
                return NSG_KEY_DELETE;
            case VK_RIGHT:
                return NSG_KEY_RIGHT;
            case VK_LEFT:
                return NSG_KEY_LEFT;
            case VK_UP:
                return NSG_KEY_UP;
            case VK_DOWN:
                return NSG_KEY_DOWN;
            case VK_HOME:
                return NSG_KEY_HOME;
            case VK_END:
                return NSG_KEY_END;
            case VK_PRIOR:
                return NSG_KEY_PAGEUP;
            case VK_NEXT:
                return NSG_KEY_PAGEDOWN;
                #elif defined(IS_TARGET_LINUX)
            case XK_Escape:
                return NSG_KEY_ESC;
            case XK_space:
                return NSG_KEY_SPACE;
            case XK_Tab:
                return NSG_KEY_TAB;
            case XK_BackSpace:
                return NSG_KEY_BACKSPACE;
            case XK_Return:
                return NSG_KEY_ENTER;
            case XK_Delete:
                return NSG_KEY_DELETE;
            case XK_Right:
                return NSG_KEY_RIGHT;
            case XK_Left:
                return NSG_KEY_LEFT;
            case XK_Up:
                return NSG_KEY_UP;
            case XK_Down:
                return NSG_KEY_DOWN;
            case XK_Home:
                return NSG_KEY_HOME;
            case XK_KP_End:
                return NSG_KEY_END;
            case XK_Page_Up:
                return NSG_KEY_PAGEUP;
            case XK_Page_Down:
                return NSG_KEY_PAGEDOWN;
                #else
#error("Unknown platform!!!")
                #endif
            default:
                return key;
        }
    }
}