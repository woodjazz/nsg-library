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
#if SDL
#include "Types.h"
#include "Window.h"
#include <string>
namespace NSG
{
    class SDLWindow : public Window
    {
    public:
        SDLWindow(const std::string& name);
        SDLWindow(const std::string& name, int x, int y, int width, int height);
        ~SDLWindow();
        void RenderFrame() override;
        void Destroy() override;
        void HandleEvents() override;
        int Run() override;
		void EnterBackground() override;
		void EnterForeground() override;
		void RestoreContext();
    private:
        void OpenJoystick(int index);
        void OpenJoysticks();
        SDLWindow* GetWindowFromID(uint32_t windowID) const;
        SDLWindow* GetCurrentWindow() const;
        void Initialize(int x, int y, int width, int height);
        void Close() override;
        void ViewChanged(int width, int height) override;
        uint32_t windowID_;
    };
}
#endif