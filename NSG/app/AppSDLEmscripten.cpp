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
#if EMSCRIPTEN
#undef main
#include "App.h"
#include "Tick.h"
#include "Keys.h"
#include "Log.h"
#include "UTF8String.h"
#include <memory>
#include <string>
#include <locale>
#ifndef __GNUC__
#include <codecvt>
#endif
#include <assert.h>
#include <SDL/SDL.h>
#include <emscripten.h>

NSG::PInternalApp s_pApp = nullptr;

namespace NSG
{
    const int WIDTH = 320;
    const int HEIGHT = 200;

    static int width = WIDTH;
    static int height = HEIGHT;

    void RunOneFrame()
    {
        bool quit = false;

        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_WINDOWEVENT)
            {
                switch (event.window.event)
                {
                case SDL_WINDOWEVENT_RESIZED:
                    width = event.window.data1;
                    height = event.window.data2;
                    s_pApp->ViewChanged(width, height);
                    break;
                default:
                    break;
                }
            }
            else if (event.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                int key = event.key.keysym.sym;
                int scancode = event.key.keysym.scancode;
                int action = NSG_KEY_PRESS;
                int modifier = event.key.keysym.mod;
                s_pApp->OnKey(key, action, modifier);
            }
            else if (event.type == SDL_KEYUP)
            {
                int key = event.key.keysym.sym;
                int scancode = event.key.keysym.scancode;
                int action = NSG_KEY_RELEASE;
                int modifier = event.key.keysym.mod;
                s_pApp->OnKey(key, action, modifier);
            }

            else if (event.type == SDL_TEXTINPUT)
            {
                UTF8String utf8(event.text.text);
                unsigned unicode = utf8.AtUTF8(0);
                if (unicode)
                {
                    s_pApp->OnChar(unicode);
                }
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                double x = event.button.x;
                double y = event.button.y;
                s_pApp->OnMouseDown((float)(-1 + 2 * x / width), (float)(1 + -2 * y / height));
            }
            else if (event.type == SDL_MOUSEBUTTONUP)
            {
                double x = event.button.x;
                double y = event.button.y;
                s_pApp->OnMouseUp((float)(-1 + 2 * x / width), (float)(1 + -2 * y / height));
            }
            else if (event.type == SDL_MOUSEMOTION)
            {
                if (width > 0 && height > 0)
                {
                    double x = event.motion.x;
                    double y = event.motion.y;
                    s_pApp->OnMouseMove((float)(-1 + 2 * x / width), (float)(1 + -2 * y / height));
                }
            }
            else if (event.type == SDL_MOUSEWHEEL)
            {
                if (width > 0 && height > 0)
                {
                    const float FACTOR = 15;
                    float x = FACTOR * event.wheel.x;
                    float y = FACTOR * event.wheel.y;
                    float screenX = x / (float)width;
                    float screenY = y / (float)height;
                    s_pApp->OnMouseWheel(screenX, screenY);
                }
            }
        }

        s_pApp->RenderFrame();

        SDL_GL_SwapBuffers();

        if (quit || s_pApp->ShallExit())
        {
            s_pApp = nullptr;
            SDL_Quit();
        }
    }

    bool CreateModule(App* pApp)
    {
        s_pApp = PInternalApp(new InternalApp(pApp));

        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_NOPARACHUTE) != 0)
            //if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) != 0)
        {
            TRACE_LOG("SDL_Init Error: " << SDL_GetError() << std::endl);
            return false;
        }

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

        /* Turn on double buffering with a 24bit Z buffer.
         * You may need to change this to 16 or 32 for your system */
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        SDL_Surface* screen = SDL_SetVideoMode( WIDTH, HEIGHT, 16, SDL_OPENGL);

        if (!screen)
        {
            TRACE_LOG("Failed to set screen video mode \n");
            return false;
        }

        s_pApp->ViewChanged(width, height);

        SDL_StartTextInput();

        emscripten_set_main_loop(RunOneFrame, 60, 1);

        return true;
    }
}

#endif