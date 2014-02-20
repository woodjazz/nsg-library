/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#ifdef ANDROID
#include "IApp.h"
#include "Tick.h"
#include "Log.h"
#include <jni.h>
#include <errno.h>

#include <EGL/egl.h>
#include "GLES2Includes.h"

#include <android/sensor.h>
#include <android_native_app_glue.h>

#include <cmath>
#include <limits>
 
int AndroidPrintMessage(const char* format, ...) 
{
    va_list args;
    const int maxBuffer = 1024;
    char buffer[maxBuffer];
    va_start(args, format);
    int ret_status = vsnprintf(buffer, maxBuffer, format, args);
    va_end(args);

    __android_log_print(ANDROID_LOG_INFO, "nsg-library", buffer);

    return ret_status;  
}


static NSG::PInternalApp s_pApp = nullptr;

struct engine 
{
    struct android_app* app;

    ASensorManager* sensorManager;
    const ASensor* accelerometerSensor;
    ASensorEventQueue* sensorEventQueue;

    int animating;
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    int32_t width;
    int32_t height;
    ANativeWindow* window;
    bool configurationChanged;
};

static engine* s_engine = nullptr;

/**
 * Initialize an EGL context for the current display.
 */
static int engine_init_display(struct engine* engine) 
{
    TRACE_LOG("engine_init_display");
    // initialize OpenGL ES and EGL

    /*
     * Here specify the attributes of the desired configuration.
     * Below, we select an EGLConfig with at least 8 bits per color
     * component compatible with on-screen windows
     */
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

    EGLint w, h, dummy, format;
    EGLint numConfigs;
    EGLConfig config;
    EGLSurface surface;
    EGLContext context;

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, 0, 0);

    /* Here, the application chooses the configuration it desires. In this
     * sample, we have a very simplified selection process, where we pick
     * the first EGLConfig that matches our criteria */
    eglChooseConfig(display, attribs, &config, 1, &numConfigs);

    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
     * As soon as we picked a EGLConfig, we can safely reconfigure the
     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

    ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);

    surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);
    EGLint attrib_list[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
    context = eglCreateContext(display, config, NULL, attrib_list);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        LOGW("Unable to eglMakeCurrent");
        return -1;
    }

    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

    engine->display = display;
    engine->context = context;
    engine->surface = surface;
    engine->width = w;
    engine->height = h;

    s_pApp->Initialize(30);

    s_pApp->ViewChanged(w, h);

    return 0;
}

/**
 * Just the current frame in the display.
 */
static void engine_draw_frame(struct engine* engine) 
{
    if (engine->display == NULL) 
    {
        // No display.
        return;
    }

    engine->animating = true;

    if(engine->configurationChanged)
    {
        EGLint w, h;
        
        eglQuerySurface(engine->display, engine->surface, EGL_WIDTH, &w);
        
        eglQuerySurface(engine->display, engine->surface, EGL_HEIGHT, &h);

        if(w == engine->width && h == engine->height)
        {
            engine->configurationChanged = false;

            s_pApp->ViewChanged(engine->width, engine->height);
        }
        else
        {
            glClearColor(0, 0, 0, 1);
            glClearDepth(1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);            
        }
    }
    else
    {
        s_pApp->PerformTick();

        s_pApp->RenderFrame();
    }

    eglSwapBuffers(engine->display, engine->surface);
}

/**
 * Tear down the EGL context currently associated with the display.
 */
static void engine_term_display(struct engine* engine) 
{
    TRACE_LOG("engine_term_display");

    if (engine->display != EGL_NO_DISPLAY) 
    {
        eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    
        if (engine->context != EGL_NO_CONTEXT) 
        {
            eglDestroyContext(engine->display, engine->context);
        }

        if (engine->surface != EGL_NO_SURFACE) 
        {
            eglDestroySurface(engine->display, engine->surface);
        }
        
        eglTerminate(engine->display);
    }
    engine->animating = 0;
    engine->display = EGL_NO_DISPLAY;
    engine->context = EGL_NO_CONTEXT;
    engine->surface = EGL_NO_SURFACE;
}

/**
 * Process the next input event.
 */
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) 
{
    //TRACE_LOG("engine_handle_input");

    if(AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) 
    {
        if(AMotionEvent_getAction(event) == AMOTION_EVENT_ACTION_DOWN)
        {
            if(s_engine->width > 0 && s_engine->height > 0)
            {
                s_pApp->OnMouseDown(-1 + 2*AMotionEvent_getX(event, 0)/s_engine->width, 1 - 2*AMotionEvent_getY(event, 0)/s_engine->height);
            }
        }
        else if(AMotionEvent_getAction(event) == AMOTION_EVENT_ACTION_UP)
        {
            s_pApp->OnMouseUp();
        }
        else if(AMotionEvent_getAction(event) == AMOTION_EVENT_ACTION_MOVE)
        {
            if(s_engine->width > 0 && s_engine->height > 0)
            {
                s_pApp->OnMouseMove(-1 + 2*AMotionEvent_getX(event, 0)/s_engine->width, 1 - 2*AMotionEvent_getY(event, 0)/s_engine->height);
            }
        }
        return 1;
    }

    return 0;
}

static void engine_handle_configuration_changed(ANativeActivity *pActivity)
{
    TRACE_LOG("engine_handle_configuration_changed");

    s_engine->configurationChanged = true;

    AConfiguration* pConf = AConfiguration_new();

    AConfiguration_fromAssetManager(pConf, pActivity->assetManager);

    int32_t w = AConfiguration_getScreenWidthDp(pConf);
    int32_t h = AConfiguration_getScreenHeightDp(pConf);
    int32_t density = AConfiguration_getDensity(pConf);
    int32_t screenSize = AConfiguration_getScreenSize(pConf);
    int32_t screenlong = AConfiguration_getScreenLong(pConf);

    AConfiguration_delete(pConf);

    float current_factor = s_engine->width / s_engine->height;
    float new_factor = w / h;

    if(std::abs(current_factor - new_factor) > std::numeric_limits<float>::epsilon())
    {
        std::swap(s_engine->width, s_engine->height);

        //s_pApp->ViewChanged cannot be called here since the real screen rotation has not been performed yet
    }

    TRACE_LOG("screenlong=" << screenlong << " screenSize=" << screenSize << " density=" << density << " w=" << w << " h=" << h);
}

static void engine_handle_window_resized(ANativeActivity* activity, ANativeWindow* window)
{
    TRACE_LOG("engine_handle_window_resized"); 

    s_engine->window = window;

    int32_t w = ANativeWindow_getWidth(window);
    
    int32_t h = ANativeWindow_getHeight(window);

    //s_pApp->ViewChanged(w, h);
}

/**
 * Process the next main command.
 */
static void engine_handle_cmd(struct android_app* app, int32_t cmd) 
{
    struct engine* engine = (struct engine*)app->userData;

    switch (cmd) 
    {
        case APP_CMD_SAVE_STATE:
            TRACE_LOG("APP_CMD_SAVE_STATE"); 
            break;
        case APP_CMD_INIT_WINDOW:
            TRACE_LOG("APP_CMD_INIT_WINDOW"); 
            // The window is being shown, get it ready.
            if (engine->app->window != NULL) 
            {
                engine_init_display(engine);
                engine_draw_frame(engine);
            }
            break;
        case APP_CMD_TERM_WINDOW:
            TRACE_LOG("APP_CMD_TERM_WINDOW"); 
            // The window is being hidden or closed, clean it up.
            engine_term_display(engine);
            break;
        case APP_CMD_GAINED_FOCUS:
            TRACE_LOG("APP_CMD_GAINED_FOCUS"); 
            // When our app gains focus, we start monitoring the accelerometer.
            if (engine->accelerometerSensor != NULL) 
            {
                ASensorEventQueue_enableSensor(engine->sensorEventQueue, engine->accelerometerSensor);
                // We'd like to get 60 events per second (in us).
                ASensorEventQueue_setEventRate(engine->sensorEventQueue, engine->accelerometerSensor, (1000L/60)*1000);
            }
            break;
        case APP_CMD_LOST_FOCUS:
            TRACE_LOG("APP_CMD_LOST_FOCUS"); 
            // When our app loses focus, we stop monitoring the accelerometer.
            // This is to avoid consuming battery while not being used.
            if (engine->accelerometerSensor != NULL) 
            {
                ASensorEventQueue_disableSensor(engine->sensorEventQueue, engine->accelerometerSensor);
            }
            // Also stop animating.
            engine->animating = 0;
            engine_draw_frame(engine);
            break;
    }
}

namespace NSG
{
    /**
     * This is the main entry point of a native application that is using
     * android_native_app_glue.  It runs in its own thread, with its own
     * event loop for receiving input events and doing other things.
     */
    void CreateModule(struct android_app* state, NSG::PApp pApp)
    {
        TRACE_LOG("CreateModule"); 

        pApp->SetAssetManager(state->activity->assetManager);

        s_pApp = PInternalApp(new InternalApp(pApp));

        struct engine engine;

        // Make sure glue isn't stripped.
        app_dummy();

        memset(&engine, 0, sizeof(engine));
        s_engine = &engine;
        state->userData = &engine;
        state->onAppCmd = engine_handle_cmd;
        state->onInputEvent = engine_handle_input;
        engine.app = state;
        state->activity->callbacks->onNativeWindowResized = engine_handle_window_resized;
        state->activity->callbacks->onConfigurationChanged = engine_handle_configuration_changed;

        // Prepare to monitor accelerometer
        engine.sensorManager = ASensorManager_getInstance();
        engine.accelerometerSensor = ASensorManager_getDefaultSensor(engine.sensorManager, ASENSOR_TYPE_ACCELEROMETER);
        engine.sensorEventQueue = ASensorManager_createEventQueue(engine.sensorManager, state->looper, LOOPER_ID_USER, NULL, NULL);

        while (1) 
        {
            // Read all pending events.
            int ident;
            int events;
            struct android_poll_source* source;

            // If not animating, we will block forever waiting for events.
            // If animating, we loop until all events are read, then continue
            // to draw the next frame of animation.
            while ((ident=ALooper_pollAll(engine.animating ? 0 : -1, NULL, &events, (void**)&source)) >= 0) 
            {
                // Process this event.
                if (source != NULL) 
                {
                    source->process(state, source);
                }

                // If a sensor has data, process it now.
                if (ident == LOOPER_ID_USER) 
                {
                    if (engine.accelerometerSensor != NULL) 
                    {
                        ASensorEvent event;
                        while (ASensorEventQueue_getEvents(engine.sensorEventQueue, &event, 1) > 0) 
                        {
                            /*LOGI("accelerometer: x=%f y=%f z=%f",
                                    event.acceleration.x, event.acceleration.y,
                                    event.acceleration.z);*/
                        }
                    }
                }

                // Check if we are exiting.
                if (state->destroyRequested != 0) 
                {
                    engine_term_display(&engine);
                    return;
                }
            }

            if (engine.animating) 
                engine_draw_frame(&engine);
        }
    }
}

#endif