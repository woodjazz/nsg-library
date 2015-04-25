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
#include "MapAndVector.h"
#include "Util.h"
#include <string>
#include <mutex>
struct SDL_Window;
namespace NSG
{
    class Window : public std::enable_shared_from_this<Window>
    {
    public:
		static PWindow Create(const std::string& name = GetUniqueName("Window"), WindowFlags flags = (int)WindowFlag::SHOWN);
        static PWindow Create(const std::string& name, int x, int y, int width, int height, WindowFlags flags = (int)WindowFlag::SHOWN);
        virtual ~Window();
        float GetDeltaTime() const;
        virtual void ViewChanged(int width, int height);
        virtual void Show() = 0;
        virtual void Hide() = 0;
        virtual void Raise() = 0;
        void OnMouseMove(float x, float y);
        void OnMouseWheel(float x, float y);
        void OnMouseDown(int button, float x, float y);
        void OnMouseUp(int button, float x, float y);
        void OnMultiGesture(int timestamp, float x, float y, float dTheta, float dDist, int numFingers);
        void OnKey(int key, int action, int modifier);
        void OnChar(unsigned int character);
        void OnJoystickDown(int joystickID, unsigned button);
        void OnJoystickUp(int joystickID, unsigned button);
        void OnJoystickAxisMotion(int joystickID, JoystickAxis axis, float position);
		virtual void RenderFrame() = 0;
        virtual void EnterBackground();
        virtual void EnterForeground();
        void InvalidateContext();
        void DropFile(const std::string& filePath);
        int GetWidth() const { return width_; }
        int GetHeight() const { return height_; }
        virtual void Close();
		bool IsClosed() const { return isClosed_; }
		bool IsMinimized() const { return minimized_; }
        virtual void Destroy() {}
        Recti GetViewport() const;
        const std::string& GetName() const { return name_; }
        PFilter AddBlurFilter();
        PFilter AddBlendFilter();
        PFilter AddWaveFilter();
        PFilter AddUserFilter(PResource fragmentShader);
        const std::vector<PFilter>& GetFilters() const { return filters_; }
        bool HasFilters() const { return !filters_.empty() && filtersEnabled_; }
        void EnableFilters(bool enable);
        bool BeginFrameRender();
        void EndFrameRender();
        PFrameBuffer GetFrameBuffer() const { return frameBuffer_; }
        void SetScene(Scene* scene);
        Scene* GetScene() const { return scene_; }
        static bool AllowWindowCreation();
		static void NotifyOneWindow2Remove();
        static std::vector<PWeakWindow>& GetWindows() { return windows_; }
        static void SetMainWindow(Window* window);
        static void AddWindow(PWindow window);
        static void UpdateScenes(float delta);
		static bool RenderWindows();
        static Window* GetMainWindow() { return mainWindow_; }
        static void HandleEvents();

        static SignalWindow::PSignal signalWindowCreated_;
        SignalViewChanged::PSignal signalViewChanged_;
        SignalMouseMoved::PSignal signalMouseMoved_;
        SignalMouseDown::PSignal signalMouseDown_;
        SignalMouseUp::PSignal signalMouseUp_;
        SignalMouseWheel::PSignal signalMouseWheel_;
        SignalKey::PSignal signalKey_;
        SignalChar::PSignal signalChar_;
        SignalMultiGesture::PSignal signalMultiGesture_;
        SignalDropFile::PSignal signalDropFile_;
        SignalJoystickDown::PSignal signalJoystickDown_;
        SignalJoystickUp::PSignal signalJoystickUp_;
        SignalJoystickAxisMotion::PSignal signalJoystickAxisMotion_;
    protected:
        Window(const std::string& name);
        void SetSize(int width, int height);
        void OnCreated();
        std::string name_;
        bool isClosed_;
        bool minimized_;
        bool isMainWindow_;
        int width_;
        int height_;
        bool filtersEnabled_;
        Scene* scene_; //scene to render in this window
		static std::vector<PWeakWindow> windows_;
        static Window* mainWindow_;
    private:
        void AddFilter(PFilter filter);
        void CreateFrameBuffer();
        std::vector<PFilter> filters_;
        PFrameBuffer frameBuffer_;
        PShowTexture showFrameBuffer_;
        static int nWindows2Remove_;
        static PGraphics graphics_;
        static std::once_flag onceFlag_;
    };
}