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
#if defined(IS_TARGET_LINUX) && defined(EGL)
#include "LinuxX11EGLWindow.h"
#include "Check.h"
#include "Engine.h"
#include "Log.h"
#include <EGL/egl.h>
#include <X11/Xutil.h>

namespace NSG {
static EGLContext eglContext = nullptr;
static EGLSurface eglSurface = EGL_NO_SURFACE;

LinuxX11EGLWindow::LinuxX11EGLWindow(const std::string& name, WindowFlags flags)
    : LinuxX11Window(name, flags) {
    auto& conf = Engine::GetPtr()->GetAppConfiguration();
    Initialize(conf.x_, conf.y_, conf.width_, conf.height_, flags);
    LOGI("Linux X11 EGL Window %s created.", name_.c_str());
}

LinuxX11EGLWindow::LinuxX11EGLWindow(const std::string& name, int x, int y,
                                     int width, int height, WindowFlags flags)
    : LinuxX11Window(name, x, y, width, height, flags) {
    Initialize(x, y, width, height, flags);
    LOGI("Linux X11 EGL Window %s created.", name_.c_str());
}

LinuxX11EGLWindow::~LinuxX11EGLWindow() { Close(); }

void LinuxX11EGLWindow::CreateContext() {
    auto display =
        eglGetDisplay((EGLNativeDisplayType)LinuxX11Window::display_);
    CHECK_ASSERT(display != EGL_NO_DISPLAY && "Failed to create display");

    EGLint major = 0;
    EGLint minor = 0;
    EGLBoolean success = eglInitialize(display, &major, &minor);
    CHECK_CONDITION(success && "Failed to query ELG version");
    CHECK_CONDITION(((major == 1 && minor >= 2) || major > 1) &&
                    "EGL version is not >=1.2");
    EGLint count = 0;
    eglGetConfigs(display, NULL, 0, &count);
    LOGI("EGL has %d configs", count);
    std::vector<EGLConfig> configs(count);

    const EGLint attribs[] = {EGL_SURFACE_TYPE,
                              EGL_WINDOW_BIT,
                              EGL_RENDERABLE_TYPE,
                              EGL_OPENGL_ES2_BIT,
                              EGL_STENCIL_SIZE,
                              8,
                              EGL_BLUE_SIZE,
                              8,
                              EGL_GREEN_SIZE,
                              8,
                              EGL_RED_SIZE,
                              8,
                              EGL_ALPHA_SIZE,
                              8,
                              EGL_DEPTH_SIZE,
                              24,
                              EGL_NONE};
    EGLint n = 0;
    EGLint value = 0;
    eglChooseConfig(display, attribs, &configs[0], count, &n);
    CHECK_CONDITION(n > 0 && "EGL configuration not found!!!");
    EGLConfig egl_bestConf = configs[0];
    auto nAttrs = sizeof(attribs) / sizeof(EGLint) - 1;
    for (int i = 0; i < n; i++) {
        for (int attr = 4; attr < nAttrs; attr += 2) {
            eglGetConfigAttrib(display, configs[i], attribs[attr], &value);
            if (value >= attribs[attr + 1])
                egl_bestConf = configs[i];
        }
    }
    if (Window::mainWindow_) {
        isMainWindow_ = false;
        // Do not create a new context. Instead, share the main window's
        // context.
        SetContext();
    } else {
        eglSurface = eglCreateWindowSurface(
            display, egl_bestConf, (EGLNativeWindowType)hwnd_, nullptr);
        CHECK_CONDITION(eglSurface != EGL_NO_SURFACE);
        EGLint context_attribs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
        eglContext = eglCreateContext(display, egl_bestConf, EGL_NO_CONTEXT,
                                      context_attribs);
        CHECK_CONDITION(eglContext && "Failed to create EGL context!!!");
        SetContext();
        Window::SetMainWindow(this);
    }
}

void LinuxX11EGLWindow::SetContext() {
    auto display =
        eglGetDisplay((EGLNativeDisplayType)LinuxX11Window::display_);
    CHECK_CONDITION(EGL_TRUE == eglMakeCurrent(display, eglSurface, eglSurface,
                                               eglContext));
}

void LinuxX11EGLWindow::DestroyContext() {
    auto display =
        eglGetDisplay((EGLNativeDisplayType)LinuxX11Window::display_);
    auto context = eglGetCurrentContext();
    auto surface = eglGetCurrentSurface(EGL_DRAW);
    eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    if (context != EGL_NO_CONTEXT)
        eglDestroyContext(display, context);
    if (surface != EGL_NO_SURFACE)
        eglDestroySurface(display, surface);
    eglTerminate(display);
}

void LinuxX11EGLWindow::SwapWindowBuffers() {
    auto display =
        eglGetDisplay((EGLNativeDisplayType)LinuxX11Window::display_);
    auto surface = eglGetCurrentSurface(EGL_DRAW);
    eglSwapBuffers(display, surface);
}
}
#endif
