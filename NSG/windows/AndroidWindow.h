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
#pragma once
#if defined(IS_TARGET_ANDROID)
#include "GLIncludes.h"
#include "Window.h"
#include <android/sensor.h>
#include <android_native_app_glue.h>
#include <string>

namespace NSG {
class AndroidWindow : public Window {
public:
    AndroidWindow(const std::string& name, WindowFlags flags);
    AndroidWindow(const std::string& name, int x, int y, int width, int height,
                  WindowFlags flags);
    ~AndroidWindow();
    void SwapWindowBuffers() override;
    void Destroy() override;
    void HandleEvents() override;

private:
    bool IsValid() override;
    void SetContext() override;
    void Show() override;
    void Hide() override;
    void Raise() override;
    void SetupImgui() override;
    void BeginImguiRender() override;
    void Initialize(int x, int y, int width, int height, WindowFlags flags);
    void Close() override;
    void InitDisplay();
    void DestroyDisplay();
    void RestoreContext();
    static int32_t HandleInput(struct android_app* app, AInputEvent* event);
    static void HandleCmd(struct android_app* app, int32_t cmd);
    int flags_;
    ASensorManager* sensorManager_;
    const ASensor* accelerometerSensor_;
    ASensorEventQueue* sensorEventQueue_;
    EGLDisplay display_;
    EGLSurface surface_;
    EGLContext eglContext_;
};
}
#endif
