/*
   -------------------------------------------------------------------------------
   This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2017 Néstor Silveira Gorski

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
#if defined(IS_TARGET_LINUX)
#include "LinuxX11GLXWindow.h"
#include "Check.h"
#include "Engine.h"
#include "Log.h"
#include <GL/glx.h>

namespace NSG {

LinuxX11GLXWindow::LinuxX11GLXWindow(const std::string& name, WindowFlags flags)
    : LinuxX11Window(name, flags) {
    auto& conf = Engine::GetPtr()->GetAppConfiguration();
    Initialize(conf.x_, conf.y_, conf.width_, conf.height_, flags);
    LOGI("Linux X11 GLX Window %s created.", name_.c_str());
}

LinuxX11GLXWindow::LinuxX11GLXWindow(const std::string& name, int x, int y,
                                     int width, int height, WindowFlags flags)
    : LinuxX11Window(name, x, y, width, height, flags) {
    Initialize(x, y, width, height, flags);
    LOGI("Linux X11 GLX Window %s created.", name_.c_str());
}

LinuxX11GLXWindow::~LinuxX11GLXWindow() { Close(); }

void LinuxX11GLXWindow::CreateContext() {
    int32_t screen = DefaultScreen(LinuxX11Window::display_);
    int major, minor;
    bool version = glXQueryVersion(LinuxX11Window::display_, &major, &minor);
    CHECK_CONDITION(version && "Failed to query GLX version");
    CHECK_CONDITION(((major == 1 && minor >= 2) || major > 1) &&
                    "GLX version is not >=1.2");

    const int attrsGlx[] = {
        GLX_RENDER_TYPE, GLX_RGBA_BIT, GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_DOUBLEBUFFER, true, GLX_RED_SIZE, 8, GLX_BLUE_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        // GLX_ALPHA_SIZE, 8,
        GLX_DEPTH_SIZE, 24, GLX_STENCIL_SIZE, 8, None,
    };

    // GLXFBConfig bestConfig = nullptr;

    bool valid = true;
    XVisualInfo* visualInfo = nullptr;
    int numConfigs = 0;
    auto configs = glXChooseFBConfig(LinuxX11Window::display_, screen, attrsGlx,
                                     &numConfigs);
    for (int i = 0; i < numConfigs; ++i) {
        visualInfo =
            glXGetVisualFromFBConfig(LinuxX11Window::display_, configs[i]);
        if (visualInfo) {
            auto n = sizeof(attrsGlx) / sizeof(int) - 1;
            for (uint32_t attr = 6; attr < n; attr += 2) {
                int value = 0;
                glXGetFBConfigAttrib(LinuxX11Window::display_, configs[i],
                                     attrsGlx[attr], &value);
                if (value < attrsGlx[attr + 1]) {
                    valid = false;
                    break;
                }
            }
            if (valid) {
                // bestConfig = configs[i];
                break;
            }
        }
        XFree(visualInfo);
    }
    XFree(configs);

    CHECK_CONDITION(visualInfo &&
                    "Failed to find a suitable X11 display configuration.");

    if (Window::mainWindow_) {
        isMainWindow_ = false;
        // Do not create a new context. Instead, share the main window's
        // context.
        SetContext();
    } else {
        LOGI("Create GL 2.1 context.");
        auto context =
            glXCreateContext(LinuxX11Window::display_, visualInfo, 0, GL_TRUE);
        CHECK_CONDITION(context && "Failed to create GL 2.1 context.");
        glXMakeCurrent(LinuxX11Window::display_, hwnd_, context);
        Window::SetMainWindow(this);
    }

    auto glXSwapIntervalEXT = (PFNGLXSWAPINTERVALEXTPROC)glXGetProcAddress(
        (const GLubyte*)"glXSwapIntervalEXT");
    if (glXSwapIntervalEXT) {
        LOGI("Using glXSwapIntervalEXT.");
        glXSwapIntervalEXT(LinuxX11Window::display_, hwnd_, 0);
    } else {
        auto glXSwapIntervalMESA =
            (PFNGLXSWAPINTERVALMESAPROC)glXGetProcAddress(
                (const GLubyte*)"glXSwapIntervalMESA");
        if (glXSwapIntervalMESA) {
            LOGI("Using glXSwapIntervalMESA.");
            glXSwapIntervalMESA(0);
        } else {
            auto glXSwapIntervalSGI =
                (PFNGLXSWAPINTERVALSGIPROC)glXGetProcAddress(
                    (const GLubyte*)"glXSwapIntervalSGI");
            if (glXSwapIntervalSGI) {
                LOGI("Using glXSwapIntervalSGI.");
                glXSwapIntervalSGI(0);
            }
        }
    }
}

void LinuxX11GLXWindow::SetContext() {
    auto context = glXGetCurrentContext();
    glXMakeCurrent(LinuxX11Window::display_, hwnd_, context);
}

void LinuxX11GLXWindow::DestroyContext() {
    auto context = glXGetCurrentContext();
    glXDestroyContext(LinuxX11Window::display_, context);
}

void LinuxX11GLXWindow::SwapWindowBuffers() {
    glXSwapBuffers(LinuxX11Window::display_, hwnd_);
}
}
#endif
