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
#include "Util.h"
#include <string>
#include <vector>
namespace NSG
{
    struct IRender
    {
        virtual void Render() = 0;
    };

    class Window : public std::enable_shared_from_this<Window>
    {
    public:
        static PWindow CreateExternal(const std::string& name = GetUniqueName("ExternalWindow"));
        static PWindow Create(const std::string& name = GetUniqueName("Window"), WindowFlags flags = (int)WindowFlag::SHOWN);
        static PWindow Create(const std::string& name, int x, int y, int width, int height, WindowFlags flags = (int)WindowFlag::SHOWN);
        virtual ~Window();
        virtual void ViewChanged(int width, int height);
        virtual void Show() {}
        virtual void Hide() {}
        virtual void Raise() {}
        void OnMouseMove(int x, int y);
        void OnMouseWheel(float x, float y);
        void OnMouseDown(int button, int x, int y);
        void OnMouseUp(int button, int x, int y);
        void OnMultiGesture(int timestamp, float x, float y, float dTheta, float dDist, int numFingers);
        void OnKey(int key, int action, int modifier);
        void OnChar(unsigned int character);
        void OnText(const std::string& text);
        void OnJoystickDown(int joystickID, JoystickButton button);
        void OnJoystickUp(int joystickID, JoystickButton button);
        void OnJoystickAxisMotion(int joystickID, JoystickAxis axis, float position);
        void RenderFrame();
        virtual void SwapWindowBuffers() {}
        virtual void EnterBackground();
        virtual void EnterForeground();
        void DropFile(const std::string& filePath);
        int GetWidth() const { return width_; }
        int GetHeight() const { return height_; }
        virtual void Close();
        bool IsClosed() const { return isClosed_; }
        bool IsMinimized() const { return minimized_; }
        static bool AreAllWindowsMinimized();
        virtual void Destroy() {}
        Recti GetViewport() const;
        const std::string& GetName() const { return name_; }
        bool HasFilters() const { return !filters_.empty() && filtersEnabled_; }
        void EnableFilters(bool enable);
        FrameBuffer* GetFrameBuffer() const { return frameBuffer_.get(); }
        FrameBuffer* GetFilterFrameBuffer() const { return filterFrameBuffer_.get(); }
        void SetScene(PScene scene);
        PWeakScene GetScene() const { return scene_; }
        void ShowMap(PTexture texture);
        static bool AllowWindowCreation();
        static void NotifyOneWindow2Remove();
        static std::vector<PWeakWindow>& GetWindows() { return windows_; }
        static void SetMainWindow(Window* window);
        static void AddWindow(PWindow window);
        static void UpdateScenes(float delta);
        static bool RenderWindows();
        static Window* GetMainWindow() { return mainWindow_; }
        static void HandleEvents();
        SignalSizeChanged::PSignal SigSizeChanged() { return signalViewChanged_; }
        SignalFloatFloat::PSignal SigFloatFloat() { return signalFloatFloat_; }
        SignalMouseMoved::PSignal SigMouseMoved() { return signalMouseMoved_; }
        SignalMouseButton::PSignal SigMouseDown() { return signalMouseDown_; }
        SignalMouseButtonInt::PSignal SigMouseDownInt() { return signalMouseDownInt_; }
        SignalMouseButton::PSignal SigMouseUp() { return signalMouseUp_; }
        SignalMouseButtonInt::PSignal SigMouseUpInt() { return signalMouseUpInt_; }
        SignalFloatFloat::PSignal SigMouseWheel() { return signalMouseWheel_; }
        SignalKey::PSignal SigKey() { return signalKey_; }
        SignalUnsigned::PSignal SigUnsigned() { return signalUnsigned_; }
        SignalText::PSignal SigText() { return signalText_; }
        SignalMultiGesture::PSignal SigMultiGesture() { return signalMultiGesture_; }
        SignalDropFile::PSignal SigDropFile() { return signalDropFile_; }
        SignalJoystickButton::PSignal SigJoystickDown() { return signalJoystickDown_; }
        SignalJoystickButton::PSignal SigJoystickUp() { return signalJoystickUp_; }
        SignalJoystickAxisMotion::PSignal SigJoystickAxisMotion() { return signalJoystickAxisMotion_; }
        SignalEmpty::PSignal SigDrawIMGUI() { return signalDrawIMGUI_; }
        SignalTouchFinger::PSignal SigTouchFinger() { return signalTouchFinger_; }
        PixelFormat GetPixelFormat() const { return pixelFormat_; }
        void SetPixelFormat(PixelFormat value) { pixelFormat_ = value; }
        void RenderFilters();
        virtual void SetupImgui();
        virtual void BeginImguiRender() {}
        virtual void SetContext() {}
        void SetRender(IRender* render);
        void RemoveRender(IRender* render);
        PShowTexture GetShowMap() const { return showMap_; }
        void AddFilter(PMaterial filter);
        void RemoveFilter(PMaterial filter);
        bool UseFrameRender();
        void ShowMap();
        void OnReady();
    protected:
        Window(const std::string& name);
        void SetSize(int width, int height);
        std::string name_;
        bool isClosed_;
        bool minimized_;
        bool isMainWindow_;
        int width_;
        int height_;
        bool filtersEnabled_;
        PWeakScene scene_; //scene to render in this window
        static std::vector<PWeakWindow> windows_;
        static Window* mainWindow_;
        PRenderingContext graphics_;
        PRenderer renderer_;
    private:
        void CreateFrameBuffer();
		void OnMouseMove(float x, float y);
		void OnMouseDown(int button, float x, float y);
		void OnMouseUp(int button, float x, float y);
        bool BeginFrameRender();
        std::vector<PWeakMaterial> filters_;
        PFrameBuffer frameBuffer_;
        PFrameBuffer filterFrameBuffer_;
        PShowTexture showMap_;
        SignalSizeChanged::PSignal signalViewChanged_;
        SignalFloatFloat::PSignal signalFloatFloat_;
        SignalMouseMoved::PSignal signalMouseMoved_;
        SignalMouseButton::PSignal signalMouseDown_;
        SignalMouseButtonInt::PSignal signalMouseDownInt_;
        SignalMouseButton::PSignal signalMouseUp_;
        SignalMouseButtonInt::PSignal signalMouseUpInt_;
        SignalFloatFloat::PSignal signalMouseWheel_;
        SignalKey::PSignal signalKey_;
        SignalUnsigned::PSignal signalUnsigned_;
        SignalText::PSignal signalText_;
        SignalMultiGesture::PSignal signalMultiGesture_;
        SignalDropFile::PSignal signalDropFile_;
        SignalJoystickButton::PSignal signalJoystickDown_;
        SignalJoystickButton::PSignal signalJoystickUp_;
        SignalJoystickAxisMotion::PSignal signalJoystickAxisMotion_;
        SignalEmpty::PSignal signalDrawIMGUI_;
        SignalTouchFinger::PSignal signalTouchFinger_;
        PTexture showTexture_;
        static int nWindows2Remove_;
        PixelFormat pixelFormat_;
        IRender* render_;
        PGUI gui_;
    };
}
