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
#include "SDLWindow.h"
#include "SDL.h"
#undef main
#include "App.h"
#include "Graphics.h"
#include "Tick.h"
#include "Keys.h"
#include "Log.h"
#include "UTF8String.h"
#include "AppConfiguration.h"
#include <memory>
#include <string>
#include <locale>
#include <thread>
#ifndef __GNUC__
#include <codecvt>
#endif
#if EMSCRIPTEN
#include <emscripten.h>
#endif
namespace NSG
{
    SDLWindow::SDLWindow(const std::string& name, int x, int y, int width, int height)
        : Window(name)
    {
        if (SDL_InitSubSystem(SDL_INIT_VIDEO))
            TRACE_LOG("SDL_INIT_VIDEO Error: " << SDL_GetError() << std::endl);

        SetSize(width, height);

        #if !defined(EMSCRIPTEN)
        {
            win_ = nullptr;
            context_ = nullptr;
        }
        #endif

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

        #if EMSCRIPTEN
        {
            if (!SDL_SetVideoMode(width, height, 32, SDL_OPENGL | SDL_RESIZABLE))
            {
                TRACE_LOG("Failed to set screen video mode \n");
                return;
            }
            isMainWindow_ = true;
            app_->SetMainWindow(this);
        }
        #else
        {
            Uint32 flags;
            #if IOS || ANDROID
            {
                flags = SDL_WINDOW_FULLSCREEN | SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
            }
            #else
            {
                flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
            }
            #endif

            win_ = SDL_CreateWindow(name.c_str(), x, y, width, height, flags);

            if (win_ == nullptr)
            {
                TRACE_LOG("SDL_CreateWindow Error: " << SDL_GetError() << std::endl);
                return;
            }

            if (app_->GetMainWindow())
            {
                isMainWindow_ = false;
                // Do not create a new context. Instead, share the main window's context.
                context_ = app_->GetMainWindow()->GetSDLContext();
                SDL_GL_MakeCurrent(win_, context_);
            }
            else
            {
                context_ = SDL_GL_CreateContext(win_);
                app_->SetMainWindow(this);
            }
            SDL_GL_SetSwapInterval(1);
            SDL_GetWindowSize(win_, &width, &height);
        }
        #endif

        SetSize(width, height);

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

        #if defined(IS_WINDOWS) || defined(IS_LINUX)
        {
            glewExperimental = true; // Needed for core profile. Solves issue with glGenVertexArrays

            GLenum err = glewInit();
            if (err != GLEW_OK)
            {
                TRACE_LOG("Failed to initialize GLEW with error = " << glewGetErrorString(err));
                return;
            }

            if (!GLEW_VERSION_2_0)
            {
                TRACE_LOG("No support for OpenGL 2.0 found\n");
                return;
            }

            if (!GLEW_EXT_framebuffer_object || !GLEW_EXT_packed_depth_stencil)
            {
                TRACE_LOG("EXT_framebuffer_object and EXT_packed_depth_stencil OpenGL extensions are required");
                return;
            }
        }
        #endif

        Tick::Initialize();
    }

    SDLWindow::~SDLWindow()
    {
        Destroy();
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
    }

    void SDLWindow::Destroy()
    {
        #if !defined(EMSCRIPTEN)
        if (!isClosed_)
        {
            isClosed_ = true;
            app_->NotifyOneWindow2Remove();
            if (isMainWindow_)
            {
                SDL_GL_DeleteContext(context_);
                app_->SetMainWindow(nullptr);
            }
            context_ = nullptr;
            CHECK_ASSERT(win_, __FILE__, __LINE__);
            SDL_DestroyWindow(win_);
            win_ = nullptr;
        }
        #endif
    }

    void SDLWindow::RenderFrame()
    {
        Graphics::this_->SetWindow(this);
        PerformTicks();
        #ifndef EMSCRIPTEN
        {
            SDL_GL_SwapWindow(win_);
        }
        #endif
    }

    void SDLWindow::ViewChanged(int width, int height)
    {
        if (width_ != width || height_ != height)
        {
            TRACE_LOG("ViewChanged: " << width << "," << height);
            #if EMSCRIPTEN
            SDL_SetVideoMode(width, height, 32, SDL_OPENGL | SDL_RESIZABLE);
            #endif
            Window::ViewChanged(width, height);
        }
    }
}
#endif