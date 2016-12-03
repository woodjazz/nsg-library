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
#if EMSCRIPTEN
#include "EmscriptenWindow.h"
#include "SDL.h" //comes from emscripten
#undef main
#include "Engine.h"
#include "RenderingContext.h"
#include "Tick.h"
#include "Keys.h"
#include "Log.h"
#include "Check.h"
#include "UTF8String.h"
#include "AppConfiguration.h"
#include "Object.h"
#include "Scene.h"
#include "imgui.h"
#include "Maths.h"
#include <memory>
#include <string>
#include <locale>
#include <thread>
#include <mutex>
#ifndef __GNUC__
#include <codecvt>
#endif
#include <emscripten.h>
#include <html5.h>

namespace NSG
{
    static EM_BOOL EmscriptenResizeCallback(int eventType, const EmscriptenUiEvent* keyEvent, void* userData)
    {
        using namespace NSG;
        Window* window = Window::GetMainWindow();
        if (window)
            window->ViewChanged(keyEvent->windowInnerWidth, keyEvent->windowInnerHeight);
        return false;
    }
    static EM_BOOL EmscripteGamepadCallback(int eventType, const EmscriptenGamepadEvent* gamepadEvent, void* userData)
    {
        return false;
    }

    const char* InternalPointer = "InternalPointer";

    EmscriptenWindow::EmscriptenWindow(const std::string& name, WindowFlags flags)
        : Window(name),
          flags_(0)
    {
        const AppConfiguration& conf = Engine::GetPtr()->GetAppConfiguration();
        Initialize(conf.x_, conf.y_, conf.width_, conf.height_, flags);
        LOGI("Window %s created.", name_.c_str());
    }

    EmscriptenWindow::EmscriptenWindow(const std::string& name, int x, int y, int width, int height, WindowFlags flags)
        : Window(name),
          flags_(0)
    {
        Initialize(x, y, width, height, flags);
        LOGI("Window %s created.", name_.c_str());
    }

    EmscriptenWindow::~EmscriptenWindow()
    {
        Close();
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
    }

    void EmscriptenWindow::OpenJoystick(int deviceIndex)
    {
        JoystickState state;
        state.deviceIndex = deviceIndex;
        state.joystick_ = SDL_JoystickOpen(deviceIndex);
        if (!state.joystick_)
        {
            LOGW("Cannot open joystick number: %d", deviceIndex);
        }
        state.instanceID_ = deviceIndex;
        LOGI("Joystick number: %d has been added.", deviceIndex);
        joysticks_[state.instanceID_] = state;
    }

    void EmscriptenWindow::CloseJoystick(int deviceIndex)
    {
        for (auto it : joysticks_)
        {
            auto& state = it.second;
            if (state.deviceIndex == deviceIndex)
            {
                SDL_JoystickClose((SDL_Joystick*)state.joystick_);
                joysticks_.erase(state.instanceID_);
                LOGI("Joystick number: %d has been removed.", deviceIndex);
                break;
            }
        }
    }

    void EmscriptenWindow::OpenJoysticks()
    {
        int size = SDL_NumJoysticks();
        for (int i = 0; i < size; ++i)
            OpenJoystick(i);
    }

    void EmscriptenWindow::Initialize(int x, int y, int width, int height, WindowFlags flags)
    {
        static std::once_flag onceFlag_;
        std::call_once(onceFlag_, [&]()
        {
            int flags = 0;
            CHECK_CONDITION(0 == SDL_Init(flags));
            OpenJoysticks();
        });

        flags_ = SDL_INIT_JOYSTICK | SDL_INIT_VIDEO;

        CHECK_CONDITION(0 == SDL_InitSubSystem(flags_));

        SetSize(width, height);

        const int DOUBLE_BUFFER = 1;
        const int MIN_DEPTH_SIZE = 24;
        const int MAX_DEPTH_SIZE = 24;
        const int RED_SIZE = 8;
        const int GREEN_SIZE = 8;
        const int BLUE_SIZE = 8;
        const int ALPHA_SIZE = 8;
        const int STENCIL_SIZE = 8;
        const int CONTEXT_MAJOR_VERSION = 2;
        const int CONTEXT_MINOR_VERSION = 0;

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, CONTEXT_MAJOR_VERSION);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, CONTEXT_MINOR_VERSION);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, DOUBLE_BUFFER);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, MAX_DEPTH_SIZE);
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, RED_SIZE);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, GREEN_SIZE);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, BLUE_SIZE);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, ALPHA_SIZE);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, STENCIL_SIZE);

        CHECK_CONDITION( nullptr != SDL_SetVideoMode(width, height, 32, SDL_OPENGL | SDL_RESIZABLE));
        isMainWindow_ = true;
        Window::SetMainWindow(this);
        emscripten_set_resize_callback(nullptr, nullptr, false, EmscriptenResizeCallback);
        emscripten_set_gamepadconnected_callback(nullptr, false, EmscripteGamepadCallback);
        emscripten_set_gamepaddisconnected_callback(nullptr, false, EmscripteGamepadCallback);

        SetSize(width, height);

        if (isMainWindow_)
        {
            int value = 0;
            SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &value);
            LOGI("CONTEXT_MAJOR_VERSION=%d", value);
            CHECK_ASSERT(value >= CONTEXT_MAJOR_VERSION);
            SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &value);
            LOGI("GL_CONTEXT_MINOR_VERSION=%d", value);
            CHECK_ASSERT(value >= CONTEXT_MINOR_VERSION);
            SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER, &value);
            LOGI("GL_DOUBLEBUFFER=%d", value);
            CHECK_ASSERT(value == DOUBLE_BUFFER);
            SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &value);
            LOGI("GL_DEPTH_SIZE=%d", value);
            CHECK_ASSERT(value >= MIN_DEPTH_SIZE && value <= MAX_DEPTH_SIZE);
            SDL_GL_GetAttribute(SDL_GL_RED_SIZE, &value);
            LOGI("GL_RED_SIZE=%d", value);
            CHECK_ASSERT(value == RED_SIZE);
            SDL_GL_GetAttribute(SDL_GL_GREEN_SIZE, &value);
            LOGI("GL_GREEN_SIZE=%d", value);
            CHECK_ASSERT(value == GREEN_SIZE);
            SDL_GL_GetAttribute(SDL_GL_BLUE_SIZE, &value);
            LOGI("GL_BLUE_SIZE=%d", value);
            CHECK_ASSERT(value == BLUE_SIZE);
            SDL_GL_GetAttribute(SDL_GL_ALPHA_SIZE, &value);
            LOGI("GL_ALPHA_SIZE=%d", value);
            CHECK_ASSERT(value == ALPHA_SIZE);
            SDL_GL_GetAttribute(SDL_GL_STENCIL_SIZE, &value);
            LOGI("GL_STENCIL_SIZE=%d", value);
            CHECK_ASSERT(value == STENCIL_SIZE);
        }
    }

    void EmscriptenWindow::Close()
    {
        Window::Close();
        SDL_QuitSubSystem(flags_);
    }

    void EmscriptenWindow::ViewChanged(int width, int height)
    {
        if (width_ != width || height_ != height)
        {
            CHECK_CONDITION( nullptr != SDL_SetVideoMode(width, height, 32, SDL_OPENGL | SDL_RESIZABLE));
            //emscripten_set_canvas_size(width, height);
            Window::ViewChanged(width, height);
        }
    }

    void EmscriptenWindow::EnterBackground()
    {
        Window::EnterBackground();
        Object::InvalidateAll();
    }

    EmscriptenWindow* EmscriptenWindow::GetWindowFromID(uint32_t windowID)
    {
        return static_cast<EmscriptenWindow*>(Window::mainWindow_);
    }

    EmscriptenWindow* EmscriptenWindow::GetCurrentWindow()
    {
        return static_cast<EmscriptenWindow*>(Window::mainWindow_);
    }

    JoystickAxis EmscriptenWindow::ConvertAxis(int axis)
    {
        if (axis >= (int)JoystickAxis::FIRST && axis < (int)JoystickAxis::LAST)
            return (JoystickAxis)axis;
        else
        {
            LOGW("Unknown joystick axis: %d", axis);
            return JoystickAxis::UNKNOWN;
        }
    }

    JoystickButton EmscriptenWindow::ConvertButton(int button)
    {
        if (button >= (int)JoystickButton::FIRST && button < (int)JoystickButton::LAST)
            return (JoystickButton)button;
        else
        {
            LOGW("Unknown joystick button: %d", button);
            return JoystickButton::UNKNOWN;
        }
    }

    void EmscriptenWindow::HandleGamepad()
    {
        EmscriptenGamepadEvent gamepadState;
        if (EMSCRIPTEN_RESULT_SUCCESS == emscripten_get_gamepad_status(0, &gamepadState))
        {
            static EmscriptenGamepadEvent prevGamepadState = gamepadState;
            EmscriptenWindow* window = static_cast<EmscriptenWindow*>(Window::mainWindow_);
            if (gamepadState.connected && window)
            {
                const auto PRECISION_ERROR = 0.15;
                for (int i = 0; i < gamepadState.numAxes; i++)
                {
                    if (std::abs(gamepadState.axis[i]) < PRECISION_ERROR)
                        gamepadState.axis[i] = 0;
                    window->OnJoystickAxisMotion(gamepadState.index, (JoystickAxis)i, gamepadState.axis[i]);
                }
                for (int i = 0; i < gamepadState.numButtons; i++)
                {
                    if (gamepadState.digitalButton[i] != prevGamepadState.digitalButton[i])
                    {
                        bool triggered = gamepadState.digitalButton[i] != 0;
                        auto button = (JoystickButton)i;
                        if (triggered)
                            window->OnJoystickDown(gamepadState.index, button);
                        else
                            window->OnJoystickUp(gamepadState.index, button);
                    }
                }
                prevGamepadState = gamepadState;
            }
        }
    }

    static int64_t s_touchId = 0;
    void EmscriptenWindow::HandleTouchUpEvent()
    {
        //SDL_FINGERUP event does not work. This is just a simulation
        EmscriptenWindow* window = static_cast<EmscriptenWindow*>(Window::mainWindow_);
        const int MaxFingers = 5;
        TouchFingerEvent touchEvent;
        touchEvent.type = TouchFingerEvent::Type::UP;
        touchEvent.touchId = s_touchId;
        touchEvent.x = 0;
        touchEvent.y = 0;
        touchEvent.dx = 0;
        touchEvent.dy = 0;
        touchEvent.pressure = 0;
        for (auto i = 0; i < MaxFingers; i++)
        {
            if (!SDL_GetTouchFinger(s_touchId, i))
            {
                touchEvent.fingerId = i;
                window->SigTouchFinger()->Run(touchEvent);
            }
        }
    }

    void EmscriptenWindow::HandleEvents()
    {
        EmscriptenWindow::HandleGamepad();
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_WINDOWEVENT)
            {
                EmscriptenWindow* window = GetWindowFromID(event.window.windowID);
                if (!window) continue;
                switch (event.window.event)
                {
                    case SDL_WINDOWEVENT_CLOSE:
                        LOGI("SDL_WINDOWEVENT_CLOSE");
                        window->Close();
                        break;
                    case SDL_WINDOWEVENT_MINIMIZED:
                        LOGI("SDL_WINDOWEVENT_MINIMIZED");
                        window->EnterBackground();
                        break;
                    case SDL_WINDOWEVENT_MAXIMIZED:
                        LOGI("SDL_WINDOWEVENT_MAXIMIZED");
                        window->EnterForeground();
                        break;
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                        LOGI("SDL_WINDOWEVENT_SIZE_CHANGED");
                        window->ViewChanged(event.window.data1, event.window.data2);
                        break;
                    case SDL_WINDOWEVENT_RESTORED:
                        LOGI("SDL_WINDOWEVENT_RESTORED");
                        window->EnterForeground();
                        break;
                    default:
                        break;
                }
            }
            else if (event.type == SDL_KEYDOWN)
            {
                EmscriptenWindow* window = GetWindowFromID(event.key.windowID);
                if (!window) continue;
                int key = event.key.keysym.sym & ~SDLK_SCANCODE_MASK;
                int action = NSG_KEY_PRESS;
                int modifier = event.key.keysym.mod;
                //int scancode = event.key.keysym.scancode;
                window->OnKey(key, action, modifier);
            }
            else if (event.type == SDL_KEYUP)
            {
                EmscriptenWindow* window = GetWindowFromID(event.key.windowID);
                if (!window) continue;
                int key = event.key.keysym.sym & ~SDLK_SCANCODE_MASK;
                //int scancode = event.key.keysym.scancode;
                int action = NSG_KEY_RELEASE;
                int modifier = event.key.keysym.mod;
                window->OnKey(key, action, modifier);
            }
            else if (event.type == SDL_TEXTINPUT)
            {
                EmscriptenWindow* window = GetWindowFromID(event.text.windowID);
                if (!window) continue;
                window->OnText(event.text.text);
                UTF8String utf8(event.text.text);
                unsigned unicode = utf8.AtUTF8(0);
                if (unicode)
                    window->OnChar(unicode);
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                EmscriptenWindow* window = GetWindowFromID(event.button.windowID);
                if (!window) continue;
                window->OnMouseDown(event.button.button, event.button.x, event.button.y);
            }
            else if (event.type == SDL_MOUSEBUTTONUP)
            {
                EmscriptenWindow* window = GetWindowFromID(event.button.windowID);
                if (!window) continue;
                window->OnMouseUp(event.button.button, event.button.x, event.button.y);
            }
            else if (event.type == SDL_MOUSEMOTION)
            {
                EmscriptenWindow* window = GetWindowFromID(event.button.windowID);
                if (!window) continue;
                window->OnMouseMove(event.motion.x, event.motion.y);
            }
            else if (event.type == SDL_MOUSEWHEEL)
            {
                EmscriptenWindow* window = GetWindowFromID(event.wheel.windowID);
                if (!window) continue;
                window->OnMouseWheel((float)event.wheel.x, (float)event.wheel.y);
            }
            else if (event.type == SDL_FINGERDOWN || event.type == SDL_FINGERDOWN || event.type == SDL_FINGERMOTION)
            {
                EmscriptenWindow* window = static_cast<EmscriptenWindow*>(Window::mainWindow_);
                if (!window) continue;
                auto x = (int)(event.tfinger.x * window->GetWidth());
                auto y = (int)(event.tfinger.y * window->GetHeight());
                TouchFingerEvent touchEvent;
                touchEvent.type = TouchFingerEvent::Type::MOTION;
                if (event.type == SDL_FINGERDOWN)
                {
                    window->OnMouseDown(NSG_BUTTON_LEFT, x, y);
                    touchEvent.type = TouchFingerEvent::Type::DOWN;
                }
                else if (event.type == SDL_FINGERUP)
                {
                    window->OnMouseUp(NSG_BUTTON_LEFT, x, y);
                    touchEvent.type = TouchFingerEvent::Type::UP;
                }
                else
                    window->OnMouseMove(x, y);

                s_touchId = touchEvent.touchId = event.tfinger.touchId;
                touchEvent.fingerId = event.tfinger.fingerId;
                touchEvent.x = event.tfinger.x;
                touchEvent.y = event.tfinger.y;
                touchEvent.dx = event.tfinger.dx;
                touchEvent.dy = event.tfinger.dy;
                touchEvent.pressure = event.tfinger.pressure;
                window->SigTouchFinger()->Run(touchEvent);
            }
            else if (event.type == SDL_JOYBUTTONDOWN)
            {
                EmscriptenWindow* window = static_cast<EmscriptenWindow*>(Window::mainWindow_);
                if (!window) continue;
                auto& state = window->joysticks_.find(event.jbutton.which)->second;
                if (!state.pad_)
                {
                    auto button = ConvertButton(event.jbutton.button);
                    window->OnJoystickDown(state.instanceID_, button);
                }
            }
            else if (event.type == SDL_JOYBUTTONUP)
            {
                EmscriptenWindow* window = static_cast<EmscriptenWindow*>(Window::mainWindow_);
                if (!window) continue;
                auto& state = window->joysticks_.find(event.jbutton.which)->second;
                if (!state.pad_)
                {
                    auto button = ConvertButton(event.jbutton.button);
                    window->OnJoystickUp(state.instanceID_, button);
                }
            }
            else if (event.type == SDL_JOYAXISMOTION)
            {
                EmscriptenWindow* window = static_cast<EmscriptenWindow*>(Window::mainWindow_);
                if (!window) continue;
                auto& state = window->joysticks_.find(event.jaxis.which)->second;
                if (!state.pad_)
                {
                    auto axis = ConvertAxis(event.jaxis.axis);
                    float value = (float)event.jaxis.value;
                    if (std::abs(value) < 5000) value = 0;
                    auto position = Clamp(value / 32767.0f, -1.0f, 1.0f);
                    window->OnJoystickAxisMotion(state.instanceID_, axis, position);
                }
            }
        }
    }

    void EmscriptenWindow::SetupImgui()
    {
        Window::SetupImgui();
    }

    void EmscriptenWindow::BeginImguiRender()
    {
        ImGuiIO& io = ImGui::GetIO();
        // Hide OS mouse cursor if ImGui is drawing it
        SDL_ShowCursor(io.MouseDrawCursor ? 0 : 1);
    }
}
#endif
