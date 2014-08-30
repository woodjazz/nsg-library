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
#if SDL || EMSCRIPTEN
#include "SDL.h"
#undef main
#include "App.h"
#include "Tick.h"
#include "Keys.h"
#include "Log.h"
#include "UTF8String.h"
#include "AppConfiguration.h"
#include <memory>
#include <string>
#include <locale>
#ifndef __GNUC__
#include <codecvt>
#endif
#if EMSCRIPTEN
#include <emscripten.h>
static bool ems_terminating = false;
#endif

namespace NSG
{
    NSG::PInternalApp app = nullptr;
    static bool quit = false;
    static int width = 0;
    static int height = 0;
    static bool minimized = false;
    static SDL_Window* win = nullptr;

    static void AppEnterBackground()
    {
        if (!minimized)
        {
            minimized = true;
            app->InvalidateGPUContext();
            app->ReleaseResourcesFromMemory();
        }
    }

    static void AppEnterForeground()
    {
        minimized = false;
    }

    static void RenderFrame(void* data = nullptr)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_WINDOWEVENT)
            {
                switch (event.window.event)
                {
                case SDL_WINDOWEVENT_MINIMIZED:
                    AppEnterBackground();
                    break;

#if !EMSCRIPTEN
                case SDL_WINDOWEVENT_RESIZED:
                case SDL_WINDOWEVENT_RESTORED:
                {
                    AppEnterForeground();
                    SDL_GetWindowSize(win, &width, &height);
                    app->ViewChanged(width, height);
                    break;
                }
#endif
                default:
                    break;
                }
            }
#if EMSCRIPTEN
            else if (event.type == SDL_VIDEORESIZE)
            {
                SDL_ResizeEvent* r = (SDL_ResizeEvent*)&event;
                width = r->w;
                height = r->h;
                app->ViewChanged(width, height);
            }
#else            
            else if (event.type == SDL_APP_DIDENTERBACKGROUND)
            {
                AppEnterBackground();
            }
            else if (event.type == SDL_APP_DIDENTERFOREGROUND)
            {
                AppEnterForeground();
            }

#endif
            else if (event.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                int key = event.key.keysym.sym;
                //int scancode = event.key.keysym.scancode;
                int action = NSG_KEY_PRESS;
                int modifier = event.key.keysym.mod;
                app->OnKey(key, action, modifier);
            }
            else if (event.type == SDL_KEYUP)
            {
                int key = event.key.keysym.sym;
                //int scancode = event.key.keysym.scancode;
                int action = NSG_KEY_RELEASE;
                int modifier = event.key.keysym.mod;
                app->OnKey(key, action, modifier);
            }

            else if (event.type == SDL_TEXTINPUT)
            {
#ifndef __GNUC__
                std::string utf8(event.text.text);
                std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> utf16conv;
                std::u16string utf16 = utf16conv.from_bytes(utf8);
                for (char16_t c : utf16)
                {
                    app->OnChar((unsigned int)c);
                }
#else
                UTF8String utf8(event.text.text);
                unsigned unicode = utf8.AtUTF8(0);
                if (unicode)
                {
                    app->OnChar(unicode);
                }
#endif
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                double x = event.button.x;
                double y = event.button.y;
                app->OnMouseDown((float)(-1 + 2 * x / width), (float)(1 + -2 * y / height));
            }
            else if (event.type == SDL_MOUSEBUTTONUP)
            {
                double x = event.button.x;
                double y = event.button.y;
                app->OnMouseUp((float)(-1 + 2 * x / width), (float)(1 + -2 * y / height));
            }
            else if (event.type == SDL_MOUSEMOTION)
            {
                if (width > 0 && height > 0)
                {
                    double x = event.motion.x;
                    double y = event.motion.y;
                    app->OnMouseMove((float)(-1 + 2 * x / width), (float)(1 + -2 * y / height));
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
                    app->OnMouseWheel(screenX, screenY);
                }
            }
            else if (event.type == SDL_FINGERDOWN)
            {
                double x = event.tfinger.x;
                double y = event.tfinger.y;
                app->OnMouseDown((float)(-1 + 2 * x), (float)(1 + -2 * y));
            }
            else if (event.type == SDL_FINGERUP)
            {
                double x = event.tfinger.x;
                double y = event.tfinger.y;
                app->OnMouseUp((float)(-1 + 2 * x), (float)(1 + -2 * y));
            }
            else if (event.type == SDL_FINGERMOTION)
            {
                double x = event.tfinger.x;
                double y = event.tfinger.y;
                app->OnMouseMove((float)(-1 + 2 * x), (float)(1 + -2 * y));
            }
        }

#ifndef IOS
        if (quit)
        {
#if EMSCRIPTEN
            if (!ems_terminating)
            {
                TRACE_LOG("App terminating...");
                ems_terminating = true;
                emscripten_run_script("setTimeout(function() { window.close() }, 2000)");
            }
#else
            TRACE_LOG("App terminating...");
#endif
        }
        else if (app->ShallExit())
        {
            TRACE_LOG("App's logic forced exit!");
            quit = true;
        }
        else if (!minimized)
        {
            app->RenderFrame();
        }
#else
        if (!minimized)
        {
            app->RenderFrame();
            SDL_GL_SwapWindow(win);
            if (app->ShallExit())
            {
                SDL_Quit();
                exit(0); //force quit on IOS
            }
        }
#endif
    }

    bool CreateModule(App* pApp)
    {
        // Since android can keep the status of global/static variables between runs then variables need to be initializated here again/
        quit = false;
        width = 0;
        height = 0;
        minimized = false;

        app = PInternalApp(new InternalApp(pApp));

        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_NOPARACHUTE))
        {
            TRACE_LOG("SDL_Init Error: " << SDL_GetError() << std::endl);
            return false;
        }

        const int DOUBLE_BUFFER = 1;
        const int DEPTH_SIZE = 24;
        const int RED_SIZE = 8;
        const int GREEN_SIZE = 8;
        const int BLUE_SIZE = 8;
        const int ALPHA_SIZE = 8;
        const int STENCIL_SIZE = 8;
        const int CONTEXT_MAJOR_VERSION = 2;
        const int CONTEXT_MINOR_VERSION = 0;

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, CONTEXT_MAJOR_VERSION);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, CONTEXT_MINOR_VERSION);

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, DOUBLE_BUFFER);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, DEPTH_SIZE);
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, RED_SIZE);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, GREEN_SIZE);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, BLUE_SIZE);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, ALPHA_SIZE);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, STENCIL_SIZE);

#if IOS || ANDROID
        Uint32 flags = SDL_WINDOW_FULLSCREEN | SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
#else
        Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
#endif
        width = AppConfiguration::this_->width_;
        height = AppConfiguration::this_->height_;

#if EMSCRIPTEN

        SDL_Surface* screen = SDL_SetVideoMode(width, height, 32, SDL_OPENGL);
        if (!screen)
        {
            TRACE_LOG("Failed to set screen video mode \n");
            return false;
        }

#else
        win = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

        if (win == nullptr)
        {
            TRACE_LOG("SDL_CreateWindow Error: " << SDL_GetError() << std::endl);
            return false;
        }

        //SDL_GLContext maincontext =
        SDL_GL_CreateContext(win);

        SDL_GetWindowSize(win, &width, &height);

        SDL_GL_SetSwapInterval(AppConfiguration::this_->vertical_sync_ ? 1 : 0);
#endif

        int value = 0;
        SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &value);
        CHECK_ASSERT(value >= CONTEXT_MAJOR_VERSION, __FILE__, __LINE__);
        SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &value);
        CHECK_ASSERT(value >= CONTEXT_MINOR_VERSION, __FILE__, __LINE__);
        SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER, &value);
        CHECK_ASSERT(value == DOUBLE_BUFFER, __FILE__, __LINE__);
        SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &value);
        CHECK_ASSERT(value == DEPTH_SIZE, __FILE__, __LINE__);
        SDL_GL_GetAttribute(SDL_GL_RED_SIZE, &value);
        CHECK_ASSERT(value == RED_SIZE, __FILE__, __LINE__);
        SDL_GL_GetAttribute(SDL_GL_GREEN_SIZE, &value);
        CHECK_ASSERT(value == GREEN_SIZE, __FILE__, __LINE__);
        SDL_GL_GetAttribute(SDL_GL_BLUE_SIZE, &value);
        CHECK_ASSERT(value == BLUE_SIZE, __FILE__, __LINE__);
        SDL_GL_GetAttribute(SDL_GL_ALPHA_SIZE, &value);
        CHECK_ASSERT(value == ALPHA_SIZE, __FILE__, __LINE__);
        SDL_GL_GetAttribute(SDL_GL_STENCIL_SIZE, &value);
        CHECK_ASSERT(value == STENCIL_SIZE, __FILE__, __LINE__);

#ifndef GL_ES_VERSION_2_0

        glewExperimental = true; // Needed for core profile. Solves issue with glGenVertexArrays

        GLenum err = glewInit();
        if (err != GLEW_OK)
        {
            TRACE_LOG("Failed to initialize GLEW with error = " << glewGetErrorString(err));
            return false;
        }

        if (!GLEW_VERSION_2_0)
        {
            TRACE_LOG("No support for OpenGL 2.0 found\n");
            return false;
        }

        if (!GLEW_EXT_framebuffer_object || !GLEW_EXT_packed_depth_stencil)
        {
            TRACE_LOG("EXT_framebuffer_object and EXT_packed_depth_stencil OpenGL extensions are required");
            return false;
        }
#endif
		app->SetViewSize(width, height);
        app->Initialize();
        

#if IOS
        SDL_iPhoneSetAnimationCallback(win, 1, &RenderFrame, nullptr);
#elif EMSCRIPTEN
        SDL_StartTextInput();
        emscripten_set_main_loop_arg(&RenderFrame, screen, 0, 1);
        emscripten_run_script("setTimeout(function() { window.close() }, 2000)");
#else
        while (!quit)
        {
            RenderFrame();
            SDL_GL_SwapWindow(win);
        }
        app = nullptr;
        SDL_Quit();
#endif

        return true;
    }
}
#endif