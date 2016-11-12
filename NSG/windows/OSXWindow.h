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
#pragma once
#if defined(IS_TARGET_OSX)
#include "Types.h"
#include "Window.h"
#include <string>
#import <Cocoa/Cocoa.h>

namespace NSG
{
    class OSXWindow : public Window
    {
    public:
        OSXWindow(const std::string& name, WindowFlags flags);
        OSXWindow(const std::string& name, int x, int y, int width, int height, WindowFlags flags);
        ~OSXWindow();
        void SwapWindowBuffers() override;
        void Destroy() override;
        void HandleEvents() override;
        NSWindow* GetNSWindow() const { return window_; }
        void Close() override;        
        void GetMousePos(int& outX, int& outY) const;        
    private:
        void CreateContext();
        void SetContext() override;
        void Show() override;
        void Hide() override;
        void Raise() override;
        void SetupImgui() override;
        void BeginImguiRender() override;
        void Initialize(int x, int y, int width, int height, WindowFlags flags);
        int flags_;
        int32_t style_;
        NSWindow* window_;
        NSRect windowFrame_;
        static NSOpenGLView* view_;
        static NSOpenGLContext* context_;        
    };
}
#endif
