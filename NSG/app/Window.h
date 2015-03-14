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
#include "Tick.h"
#include "MapAndVector.h"
#include <string>
struct SDL_Window;
namespace NSG
{
    class Window : public Tick, public std::enable_shared_from_this<Window>
    {
    public:
        Window(const std::string& name);
        virtual ~Window();
        float GetDeltaTime() const;
        void InitializeTicks() override;
        void BeginTicks() override;
        void DoTick(float delta) override;
        void EndTicks() override;
        virtual void ViewChanged(int width, int height);
        void OnMouseMove(float x, float y);
        void OnMouseWheel(float x, float y);
        void OnMouseDown(int button, float x, float y);
        void OnMouseUp(int button, float x, float y);
        void OnMultiGesture(int timestamp, float x, float y, float dTheta, float dDist, int numFingers);
        void OnKey(int key, int action, int modifier);
        void OnChar(unsigned int character);
		virtual void RenderFrame() = 0;
        void EnterBackground();
        void EnterForeground();
        void InvalidateContext();
        void DropFile(const std::string& filePath);
        #if !defined(EMSCRIPTEN)
        virtual SDL_Window* GetSDLWindow() const { return nullptr; }
		virtual SDL_GLContext GetSDLContext() const { return nullptr;  }
        #endif
        int GetWidth() const { return width_; }
        int GetHeight() const { return height_; }
        void Close();
		bool IsClosed() const { return isClosed_; }
		bool IsMinimized() const { return minimized_; }
        virtual void Destroy() = 0;
        Recti GetViewport() const;
        const std::string& GetName() const { return name_; }

        SignalViewChanged::PSignal signalViewChanged_;
        SignalMouseMoved::PSignal signalMouseMoved_;
        SignalMouseDown::PSignal signalMouseDown_;
        SignalMouseUp::PSignal signalMouseUp_;
        SignalMouseWheel::PSignal signalMouseWheel_;
        SignalKey::PSignal signalKey_;
        SignalChar::PSignal signalChar_;
        SignalMultiGesture::PSignal signalMultiGesture_;
        SignalUpdate::PSignal signalUpdate_;
        SignalRender::PSignal signalRender_;
        SignalDropFile::PSignal signalDropFile_;
    protected:
        void SetSize(int width, int height);
        std::string name_;
        float deltaTime_; // Fixed time in seconds (1/AppConfiguration::fps_)
        App* app_;
        bool isClosed_;
        bool minimized_;
		bool isMainWindow_;
        int width_;
        int height_;
    };
}