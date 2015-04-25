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
#include "Engine.h"
#include "UniformsUpdate.h"
#include "Graphics.h"
#include "Window.h"
#if EMSCRIPTEN
#include "SDL.h"
#include <emscripten.h>
#include <html5.h>
#endif

namespace NSG
{
    SignalEngine::PSignal Engine::signalCreated_(new SignalEngine);
    AppConfiguration Engine::conf_;
    Engine* Engine::this_ = nullptr;

    Engine::Engine()
        : Tick(Engine::conf_.fps_),
          signalBeginFrame_(new SignalEmpty()),
          signalUpdate_(new SignalUpdate),
          deltaTime_(0)
    {
        CHECK_ASSERT(!Engine::this_, __FILE__, __LINE__);
        Engine::this_ = this;
    }

    Engine::~Engine()
    {
        CHECK_ASSERT(Engine::this_, __FILE__, __LINE__);
        Engine::this_ = nullptr;
    }

    void Engine::InitializeTicks()
    {
        Engine::signalCreated_->Run(this);
    }

    void Engine::BeginTicks()
    {
        Window::HandleEvents();
    }

    void Engine::DoTick(float delta)
    {
        deltaTime_ = delta;
        Window::UpdateScenes(delta);
        signalUpdate_->Run(deltaTime_);
    }

    void Engine::EndTicks()
    {
        RenderFrame();
    }

    bool Engine::RenderFrame()
    {
        if (Graphics::this_->BeginFrameRender())
        {
            signalBeginFrame_->Run();
            Window::RenderWindows();
            UniformsUpdate::ClearAllUpdates();
            Graphics::this_->EndFrameRender();
            return true;
        }
        return false;
    }

    bool Engine::RunFrame()
    {
        Engine::this_->PerformTicks();
        return Window::GetMainWindow() != nullptr;
    }

    int Engine::Run()
    {
        Tick::Initialize();
        #if EMSCRIPTEN
        SDL_StartTextInput();
        emscripten_set_main_loop([]() {Engine::RunFrame();}, 0, 1);
        emscripten_run_script("setTimeout(function() { window.close() }, 2000)");
        #else
        while (Engine::RunFrame());
        #endif
        return 0;
    }

}