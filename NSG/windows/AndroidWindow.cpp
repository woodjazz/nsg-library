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
#if defined(IS_TARGET_ANDROID)
#include "AndroidWindow.h"
#include "Engine.h"
#include "Tick.h"
#include "Keys.h"
#include "Log.h"
#include "Check.h"
#include "UTF8String.h"
#include "AppConfiguration.h"
#include "imgui.h"
#include "RenderingContext.h"
#include <memory>
#include <string>
#include <locale>
#include <map>
#ifndef __GNUC__
#include <codecvt>
#endif
#include <jni.h>
#include <android/input.h>
#include <android/keycodes.h>

namespace NSG
{
    extern android_app* androidApp;
    AndroidWindow::AndroidWindow(const std::string& name, WindowFlags flags)
        : Window(name),
          flags_(0),
          sensorManager_(nullptr),
          accelerometerSensor_(nullptr),
          sensorEventQueue_(nullptr),
          display_(EGL_NO_DISPLAY),
          surface_(EGL_NO_SURFACE),
          eglContext_(EGL_NO_CONTEXT)

    {
        const AppConfiguration& conf = Engine::GetPtr()->GetAppConfiguration();
        Initialize(conf.x_, conf.y_, conf.width_, conf.height_, flags);
        LOGI("Window %s created.", name_.c_str());
    }

    AndroidWindow::AndroidWindow(const std::string& name, int x, int y, int width, int height, WindowFlags flags)
        : Window(name),
          flags_(0),
          sensorManager_(nullptr),
          accelerometerSensor_(nullptr),
          sensorEventQueue_(nullptr),
          display_(EGL_NO_DISPLAY),
          surface_(EGL_NO_SURFACE),
          eglContext_(EGL_NO_CONTEXT)
    {
        Initialize(x, y, width, height, flags);
        LOGI("Window %s created.", name_.c_str());
    }

    AndroidWindow::~AndroidWindow()
    {
        Close();
    }

    static int32_t GetTranslatedModifier(int32_t modifiers)
    {
        int32_t translated = 0;

        if (modifiers & AMETA_SHIFT_ON)
            translated |= NSG_KEY_MOD_SHIFT;

        if (modifiers & AMETA_ALT_ON)
            translated |= NSG_KEY_MOD_ALT;

        if (modifiers & AMETA_SYM_ON)
            translated |= NSG_KEY_MOD_CONTROL;

        return translated;
    }

    static int GetUnicodeChar(ANativeActivity* pActivity, int eventType, int keyCode, int metaState)
    {
        JavaVM* javaVM = pActivity->vm;
        JNIEnv* jniEnv = pActivity->env;

        JavaVMAttachArgs attachArgs;
        attachArgs.version = JNI_VERSION_1_6;
        attachArgs.name = "NativeThread";
        attachArgs.group = nullptr;

        jint result = javaVM->AttachCurrentThread(&jniEnv, &attachArgs);
        if (result == JNI_ERR)
        {
            LOGE("AttachCurrentThread:JNI_ERR");
            return 0;
        }

        jclass class_key_event = jniEnv->FindClass("android/view/KeyEvent");
        int unicodeKey;

        if (metaState == 0)
        {
            jmethodID method_get_unicode_char = jniEnv->GetMethodID(class_key_event, "getUnicodeChar", "()I");
            jmethodID eventConstructor = jniEnv->GetMethodID(class_key_event, "<init>", "(II)V");
            jobject eventObj = jniEnv->NewObject(class_key_event, eventConstructor, eventType, keyCode);

            unicodeKey = jniEnv->CallIntMethod(eventObj, method_get_unicode_char);
        }

        else
        {
            jmethodID method_get_unicode_char = jniEnv->GetMethodID(class_key_event, "getUnicodeChar", "(I)I");
            jmethodID eventConstructor = jniEnv->GetMethodID(class_key_event, "<init>", "(II)V");
            jobject eventObj = jniEnv->NewObject(class_key_event, eventConstructor, eventType, keyCode);

            unicodeKey = jniEnv->CallIntMethod(eventObj, method_get_unicode_char, metaState);
        }

        javaVM->DetachCurrentThread();

        return unicodeKey;
    }

    static uint32_t GetTranslatedKeyCode(uint32_t keyCode)
    {
        switch (keyCode)
        {
            case AKEYCODE_DEL:
                return NSG_KEY_BACKSPACE;
            case AKEYCODE_TAB:
                return NSG_KEY_TAB;
            case AKEYCODE_ENTER:
                return NSG_KEY_ENTER;
            default:
                return keyCode;
        }
    }

    int32_t AndroidWindow::HandleInput(struct android_app* app, AInputEvent* event)
    {
        AndroidWindow* window = static_cast<AndroidWindow*>(androidApp->userData);

        const int32_t type       = AInputEvent_getType(event);
        const int32_t source     = AInputEvent_getSource(event);
        const int32_t actionBits = AMotionEvent_getAction(event);

        switch (type)
        {
            case AINPUT_EVENT_TYPE_MOTION:
                {
                    float mx = AMotionEvent_getX(event, 0);
                    float my = AMotionEvent_getY(event, 0);
                    int32_t count = AMotionEvent_getPointerCount(event);

                    int32_t action = (actionBits & AMOTION_EVENT_ACTION_MASK);
                    int32_t index  = (actionBits & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;

                    // Simulate left mouse click with 1st touch and right mouse click with 2nd touch. ignore other touchs
                    if (count < 2)
                    {
                        switch (action)
                        {
                            case AMOTION_EVENT_ACTION_DOWN:
                            case AMOTION_EVENT_ACTION_POINTER_DOWN:
                                window->OnMouseDown(action == AMOTION_EVENT_ACTION_DOWN ? NSG_BUTTON_LEFT : NSG_BUTTON_RIGHT
                                                    , (int32_t)mx
                                                    , (int32_t)my);
                                break;

                            case AMOTION_EVENT_ACTION_UP:
                            case AMOTION_EVENT_ACTION_POINTER_UP:
                                window->OnMouseUp(action == AMOTION_EVENT_ACTION_UP ? NSG_BUTTON_LEFT : NSG_BUTTON_RIGHT
                                                  , (int32_t)mx
                                                  , (int32_t)my);
                                break;

                            default:
                                break;
                        }
                    }

                    switch (action)
                    {
                        case AMOTION_EVENT_ACTION_MOVE:
                            if (0 == index)
                            {
                                window->OnMouseMove((int32_t)mx, (int32_t)my);
                            }
                            break;

                        default:
                            break;
                    }
                }
                break;

            case AINPUT_EVENT_TYPE_KEY:
                {
                    int32_t action = AKeyEvent_getAction(event);
                    int32_t modifiers = AKeyEvent_getMetaState(event);
                    int32_t keyCode = AKeyEvent_getKeyCode(event);

                    window->OnKey(GetTranslatedKeyCode(keyCode), action, GetTranslatedModifier(modifiers));

                    if (NSG_KEY_PRESS == action)
                    {
                        if (keyCode == AKEYCODE_BACK)
                        {
                            RenderingContext::GetPtr()->ResetCachedState();
                            window->Close();
                            exit(0);
                        }
                        else if (keyCode != AKEYCODE_SHIFT_LEFT && keyCode != AKEYCODE_SHIFT_RIGHT)
                        {
                            window->OnChar(GetUnicodeChar(app->activity, action, keyCode, modifiers));
                        }
                    }
                    return 1;
                }
                break;

            default:
                break;
        }

        return 0;
    }

    void AndroidWindow::HandleCmd(struct android_app* app, int32_t cmd)
    {
        AndroidWindow* window = static_cast<AndroidWindow*>(androidApp->userData);

        switch (cmd)
        {
            case APP_CMD_SAVE_STATE:
                LOGI("APP_CMD_SAVE_STATE");
                break;

            case APP_CMD_INIT_WINDOW:
                LOGI("APP_CMD_INIT_WINDOW");
                // The window is being shown, get it ready.
                window->InitDisplay();
                break;

            case APP_CMD_TERM_WINDOW:
                LOGI("APP_CMD_TERM_WINDOW");
                // The window is being hidden or closed, clean it up.
                //window->DestroyDisplay();
                window->Invalidate();
                break;

            case APP_CMD_GAINED_FOCUS:
                LOGI("APP_CMD_GAINED_FOCUS");
                // When our app gains focus, we start monitoring the accelerometer.
                if (window->accelerometerSensor_)
                {
                    ASensorEventQueue_enableSensor(window->sensorEventQueue_, window->accelerometerSensor_);
                    // We'd like to get 60 events per second (in us).
                    ASensorEventQueue_setEventRate(window->sensorEventQueue_, window->accelerometerSensor_, (1000L / 60) * 1000);
                }
                break;

            case APP_CMD_LOST_FOCUS:
                LOGI("APP_CMD_LOST_FOCUS");
                // When our app loses focus, we stop monitoring the accelerometer.
                // This is to avoid consuming battery while not being used.
                if (window->accelerometerSensor_)
                    ASensorEventQueue_disableSensor(window->sensorEventQueue_, window->accelerometerSensor_);
                break;

            case APP_CMD_CONTENT_RECT_CHANGED:
                LOGI("APP_CMD_CONTENT_RECT_CHANGED");
                break;

            case APP_CMD_WINDOW_REDRAW_NEEDED:
                LOGI("APP_CMD_WINDOW_REDRAW_NEEDED");
                break;

            case APP_CMD_WINDOW_RESIZED:
                LOGI("APP_CMD_WINDOW_RESIZED");
                break;

            case APP_CMD_INPUT_CHANGED:
                LOGI("APP_CMD_INPUT_CHANGED");
                break;

            case APP_CMD_LOW_MEMORY:
                LOGI("APP_CMD_LOW_MEMORY");
                break;
            case APP_CMD_START:
                LOGI("APP_CMD_START");
                break;

            case APP_CMD_RESUME:
                LOGI("APP_CMD_RESUME");
                window->RestoreContext();
                window->EnterForeground();
                break;

            case APP_CMD_PAUSE:
                LOGI("APP_CMD_PAUSE");
                window->EnterBackground();
                break;

            case APP_CMD_STOP:
                LOGI("APP_CMD_STOP");
                break;

            case APP_CMD_DESTROY:
                LOGI("APP_CMD_DESTROY");
                window->Close();
                break;
        }
    }

    bool AndroidWindow::IsValid()
    {
        return surface_ != EGL_NO_SURFACE;
    }

    void AndroidWindow::Initialize(int x, int y, int width, int height, WindowFlags flags)
    {
        app_dummy(); // Make sure glue isn't stripped.

        androidApp->userData = this;
        androidApp->onAppCmd = AndroidWindow::HandleCmd;
        androidApp->onInputEvent = AndroidWindow::HandleInput;

        // Prepare to monitor accelerometer
        sensorManager_ = ASensorManager_getInstance();
        accelerometerSensor_ = ASensorManager_getDefaultSensor(sensorManager_, ASENSOR_TYPE_ACCELEROMETER);
        sensorEventQueue_ = ASensorManager_createEventQueue(sensorManager_, androidApp->looper, LOOPER_ID_USER, nullptr, nullptr);
        Window::SetMainWindow(this);
    }

    void AndroidWindow::Close()
    {
        Window::Close();
    }

    void AndroidWindow::RestoreContext()
    {
        if (eglGetCurrentContext() == EGL_NO_CONTEXT)
        {
            // On Android the context may be lost behind the scenes as the application is minimized
            LOGI("OpenGL context has been lost. Restoring!!!");
            eglContext_ = eglGetCurrentContext();
            SetContext();
        }

    }

    void AndroidWindow::SetContext()
    {
        if (display_ != EGL_NO_DISPLAY && eglMakeCurrent(display_, surface_, surface_, eglContext_) == EGL_FALSE)
        {
            LOGE("Unable to eglMakeCurrent");
        }
    }

    //Initialize an EGL context for the current display.
    void AndroidWindow::InitDisplay()
    {
        if (androidApp->window && isMainWindow_ && surface_ == EGL_NO_SURFACE)
        {
            CHECK_ASSERT(display_ == EGL_NO_DISPLAY);
            LOGI("InitDisplay");
            // initialize OpenGL ES and EGL
            const EGLint attribs[] =
            {
                EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                EGL_STENCIL_SIZE, 8,
                EGL_BLUE_SIZE, 8,
                EGL_GREEN_SIZE, 8,
                EGL_RED_SIZE, 8,
                EGL_ALPHA_SIZE, 8,
                EGL_DEPTH_SIZE, 24,
                EGL_NONE
            };

            EGLint format;
            EGLint numConfigs;
            EGLConfig config;
            EGLSurface surface;
            EGLContext context;

            EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

            eglInitialize(display, 0, 0);

            eglChooseConfig(display, attribs, &config, 1, &numConfigs);

            eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

            ANativeWindow_setBuffersGeometry(androidApp->window, 0, 0, format);

            surface = eglCreateWindowSurface(display, config, androidApp->window, nullptr);
            EGLint attrib_list[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
            context = eglCreateContext(display, config, nullptr, attrib_list);

            display_ = display;
            eglContext_ = context;
            surface_ = surface;

            SetContext();

            EGLint w, h;
            eglQuerySurface(display, surface, EGL_WIDTH, &w);
            eglQuerySurface(display, surface, EGL_HEIGHT, &h);

            ViewChanged(w, h);
        }
    }

    void AndroidWindow::DestroyDisplay()
    {
        if (isMainWindow_)
        {
            LOGI("DestroyDisplay");
            CHECK_ASSERT(display_ != EGL_NO_DISPLAY);
            eglMakeCurrent(display_, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
            if (eglContext_ != EGL_NO_CONTEXT)
                eglDestroyContext(display_, eglContext_);
            if (surface_ != EGL_NO_SURFACE)
                eglDestroySurface(display_, surface_);
            eglTerminate(display_);
            display_ = EGL_NO_DISPLAY;
            surface_ = EGL_NO_SURFACE;
            eglContext_ = EGL_NO_CONTEXT;
        }
    }

    void AndroidWindow::Destroy()
    {
        if (!isClosed_)
        {
            isClosed_ = true;
            Window::NotifyOneWindow2Remove();
            if (isMainWindow_)
            {
                Window::SetMainWindow(nullptr);
                DestroyDisplay();
            }
        }
    }

    void AndroidWindow::SwapWindowBuffers()
    {
        eglSwapBuffers(display_, surface_);
    }

    void AndroidWindow::HandleEvents()
    {
        // Read all pending events.
        AndroidWindow* window = static_cast<AndroidWindow*>(androidApp->userData);
        int ident;
        int events;
        struct android_poll_source* source;

        ident = ALooper_pollOnce(0, nullptr, &events, (void**)&source);
        if (ident >= 0)
        {
            // Process this event.
            if (source)
                source->process(androidApp, source);

            // If a sensor has data, process it now.
            if (ident == LOOPER_ID_USER)
            {
                if (window->accelerometerSensor_)
                {
                    ASensorEvent event;
                    while (ASensorEventQueue_getEvents(window->sensorEventQueue_, &event, 1) > 0)
                    {
                        /*LOGI("accelerometer: x=%f y=%f z=%f",
                                event.acceleration.x, event.acceleration.y,
                                event.acceleration.z);*/
                    }
                }
            }

            // Check if we are exiting.
            if (androidApp->destroyRequested)
            {
                LOGI("Destroy requested (exiting loop)");
                window->Close();
            }
        }
    }

    void AndroidWindow::Show()
    {
    }

    void AndroidWindow::Hide()
    {
    }

    void AndroidWindow::Raise()
    {
    }

    void AndroidWindow::SetupImgui()
    {
        Window::SetupImgui();
    }

    void AndroidWindow::BeginImguiRender()
    {
        ImGuiIO& io = ImGui::GetIO();
        //io.ImeWindowHandle = (void*)hwnd_;
        // Hide OS mouse cursor if ImGui is drawing it
        //ShowCursor(io.MouseDrawCursor ? FALSE : TRUE);
    }
}
#endif
