/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

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
#include <map>
namespace NSG
{
    class SDLWindow : public Window
    {
    public:
        SDLWindow(const std::string& name, WindowFlags flags);
        SDLWindow(const std::string& name, int x, int y, int width, int height, WindowFlags flags);
        ~SDLWindow();
        void SwapWindowBuffers() override;
        void Destroy() override;
        static void HandleEvents();
		void EnterBackground() override;
		void EnterForeground() override;
		void RestoreContext();
    private:
        void Show() override;
        void Hide() override;
        void Raise() override;
        static JoystickAxis ConvertAxis(int axis);
        void OpenJoystick(int index);
        void CloseJoystick(int index);
        void OpenJoysticks();
        static SDLWindow* GetWindowFromID(uint32_t windowID);
        static SDLWindow* GetCurrentWindow();
        void Initialize(int x, int y, int width, int height, WindowFlags flags);
        void Close() override;
        void ViewChanged(int width, int height) override;
        uint32_t windowID_;
        struct JoystickState
        {
            int deviceIndex;
            void* joystick_;
            void* pad_;
            int instanceID_;
            JoystickState() : deviceIndex(-1), joystick_(nullptr), pad_(nullptr), instanceID_(-1){}; 
        };
        std::map<int, JoystickState> joysticks_;
        int flags_;
    };
}
#endif