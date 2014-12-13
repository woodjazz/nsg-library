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
#include "Window.h"
#include "App.h"
#include "AppConfiguration.h"
#include "SignalSlots.h"
#include "TextMesh.h"
#include "Scene.h"
#include "Graphics.h"
#include "Music.h"

#if EMSCRIPTEN
#include <emscripten.h>
#endif

namespace NSG
{
    Window::Window(const std::string& name)
        : Tick(AppConfiguration::this_->fps_),
          signalViewChanged_(new Signal<int, int>()),
          signalMouseMoved_(new Signal<float, float>()),
          signalMouseDown_(new Signal<int, float, float>()),
          signalMouseUp_(new Signal<int, float, float>()),
          signalMouseWheel_(new Signal<float, float>()),
          signalKey_(new Signal<int, int, int>()),
          signalChar_(new Signal<unsigned int>()),
          signalMultiGesture_(new Signal<int, float, float, float, float, int>()),
          signalUpdate_(new Signal<float>()),
          signalRender_(new Signal<>()),
          signalDropFile_(new Signal<const std::string&>()),
          name_(name),
          app_(App::this_),
          isClosed_(false),
          minimized_(false),
		  isMainWindow_(true)
    {
        TRACE_PRINTF("Window %s created\n", name_.c_str());
    }

    Window::~Window()
    {
        TRACE_PRINTF("Window %s terminated\n", name_.c_str());
    }

    void Window::Close()
    {
        TRACE_PRINTF("Closing %s window...", name_.c_str());

		if (app_->GetMainWindow() == this) 
		{
			// destroy other windows
			auto windows = app_->GetWindows();
			for (auto& obj : windows)
			{
				PWindow window = obj.lock();
				if (window && window.get() != this)
					window->Destroy();
			}
		}
		Destroy();

        #if EMSCRIPTEN
        {
            emscripten_run_script("setTimeout(function() { window.close() }, 2000)");
        }
        #endif
    }

    float Window::GetDeltaTime() const
    {
        return deltaTime_;
    }

    void Window::InitializeTicks()
    {
        app_->InitializeGraphics();
    }

    void Window::BeginTicks()
    {
    }

    void Window::DoTick(float delta)
    {
        deltaTime_ = delta;
        signalUpdate_->Run(deltaTime_);
    }

    void Window::EndTicks()
    {
        Graphics::this_->ClearAllBuffers();
        signalRender_->Run();
        UniformsUpdate::ClearAllUpdates();
    }

    void Window::SetSize(int width, int height)
    {
        width_ = width;
        height_ = height;
    }

    void Window::ViewChanged(int width, int height)
    {
        SetSize(width, height);
		if (Graphics::this_->GetWindow() == this)
			Graphics::this_->SetViewport(GetViewport(), true);
        signalViewChanged_->Run(width, height);
    }

    void Window::OnMouseMove(float x, float y)
    {
        signalMouseMoved_->Run(x, y);
    }

    void Window::OnMouseDown(int button, float x, float y)
    {
        signalMouseDown_->Run(button, x, y);
    }

    void Window::OnMouseUp(int button, float x, float y)
    {
        signalMouseUp_->Run(button, x, y);
    }

    void Window::OnMultiGesture(int timestamp, float x, float y, float dTheta, float dDist, int numFingers)
    {
        signalMultiGesture_->Run(timestamp, x, y, dTheta, dDist, numFingers);
    }

    void Window::OnMouseWheel(float x, float y)
    {
        signalMouseWheel_->Run(x, y);
    }

    void Window::OnKey(int key, int action, int modifier)
    {
        signalKey_->Run(key, action, modifier);
    }

    void Window::OnChar(unsigned int character)
    {
        signalChar_->Run(character);
    }

    void Window::EnterBackground()
    {
        minimized_ = true;

		if (app_->GetMainWindow() == this)
        {
			app_->InvalidateObjects();
            if (Music::this_ && AppConfiguration::this_->pauseMusicOnBackground_)
                Music::this_->Pause();
        }
    }

    void Window::EnterForeground()
    {
        if (app_->GetMainWindow() == this)
        {
            if (Music::this_ && AppConfiguration::this_->pauseMusicOnBackground_)
                Music::this_->Resume();
        }

        minimized_ = false;
    }

    void Window::DropFile(const std::string& filePath)
    {
        signalDropFile_->Run(filePath);
    }

    Recti Window::GetViewport() const
    {
        return Recti(0, 0, width_, height_);
    }
}