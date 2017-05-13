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
#if defined(IS_TARGET_LINUX)
#include "Types.h"
#include "Window.h"
#include <X11/Xlib.h>
#include <string>
namespace NSG {
class LinuxX11Window : public Window {
public:
    LinuxX11Window(const std::string& name, WindowFlags flags);
    LinuxX11Window(const std::string& name, int x, int y, int width, int height,
                   WindowFlags flags);
    ~LinuxX11Window();
    void Destroy() override;
    virtual void CreateContext() = 0;
    virtual void DestroyContext() = 0;
    void HandleEvents() override;

protected:
    void Initialize(int x, int y, int width, int height, WindowFlags flags);
    void Close() override;
    static Display* display_;
    ::Window hwnd_;

private:
    void Show() override;
    void Hide() override;
    void Raise() override;
    void SetupImgui() override;
    void BeginImguiRender() override;
    int flags_;
    int keyModifier_;
    static XIM im_;
    static XIC ic_;
};
}
#endif
