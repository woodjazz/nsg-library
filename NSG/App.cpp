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
#include "App.h"
#include "Log.h"
#include "GLES2Camera.h"
#include "GLES2IMGUI.h"
#include "GLES2Render2Texture.h"
#include "GLES2Text.h"
#include <cassert>

namespace NSG
{
    App* s_pApp = nullptr;

    App::App() 
    : width_(0),
    height_(0),
    selectedIndex_(0),
    isKeyboardVisible_(false)
    {
	    assert(s_pApp == nullptr);

	    s_pApp = this;
    }

    App::~App()
    {
        TRACE_LOG("Terminating App");
	    assert(s_pApp != nullptr);

	    s_pApp = nullptr;
    }

    int App::GetFPS() const
    {
        return 24;
    }


    App* App::GetPtrInstance()
    {
	    return s_pApp;
    }

    void App::SetViewSize(int32_t width, int32_t height)
    {
        width_ = width;
        height_ = height;
    }

    std::pair<int32_t, int32_t> App::GetViewSize() const
    {
        return std::pair<int32_t, int32_t>(width_, height_);
    }

#if NACL
    void App::HandleMessage(const pp::Var& var_message)
    {
        TRACE_LOG("App::HandleMessage");

        if(var_message.is_string())
        {
            std::string message = var_message.AsString();
        }
    }
#endif    

#if ANDROID
static bool displayKeyboard(ANativeActivity* pActivity, bool pShow) 
{ 
    // Attaches the current thread to the JVM. 
    jint lResult; 
    jint lFlags = 0; 

    JavaVM* lJavaVM = pActivity->vm; 
    JNIEnv* lJNIEnv = pActivity->env; 

    JavaVMAttachArgs lJavaVMAttachArgs; 
    lJavaVMAttachArgs.version = JNI_VERSION_1_6; 
    lJavaVMAttachArgs.name = "NativeThread"; 
    lJavaVMAttachArgs.group = NULL; 

    lResult=lJavaVM->AttachCurrentThread(&lJNIEnv, &lJavaVMAttachArgs); 
    
    if (lResult == JNI_ERR) 
    { 
        TRACE_LOG("AttachCurrentThread:JNI_ERR");
        return false; 
    } 

    // Retrieves NativeActivity. 
    jobject lNativeActivity = pActivity->clazz; 
    jclass ClassNativeActivity = lJNIEnv->GetObjectClass(lNativeActivity); 

    // Retrieves Context.INPUT_METHOD_SERVICE. 
    jclass ClassContext = lJNIEnv->FindClass("android/content/Context"); 
    jfieldID FieldINPUT_METHOD_SERVICE = lJNIEnv->GetStaticFieldID(ClassContext, "INPUT_METHOD_SERVICE", "Ljava/lang/String;"); 
    jobject INPUT_METHOD_SERVICE = lJNIEnv->GetStaticObjectField(ClassContext, FieldINPUT_METHOD_SERVICE); 
    //jniCheck(INPUT_METHOD_SERVICE); 

    // Runs getSystemService(Context.INPUT_METHOD_SERVICE). 
    jclass ClassInputMethodManager = lJNIEnv->FindClass( "android/view/inputmethod/InputMethodManager"); 
    jmethodID MethodGetSystemService = lJNIEnv->GetMethodID( ClassNativeActivity, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;"); 
    jobject lInputMethodManager = lJNIEnv->CallObjectMethod(lNativeActivity, MethodGetSystemService, INPUT_METHOD_SERVICE); 

    // Runs getWindow().getDecorView(). 
    jmethodID MethodGetWindow = lJNIEnv->GetMethodID(ClassNativeActivity, "getWindow", "()Landroid/view/Window;"); 
    jobject lWindow = lJNIEnv->CallObjectMethod(lNativeActivity, MethodGetWindow); 
    jclass ClassWindow = lJNIEnv->FindClass( "android/view/Window"); 
    jmethodID MethodGetDecorView = lJNIEnv->GetMethodID( ClassWindow, "getDecorView", "()Landroid/view/View;"); 
    jobject lDecorView = lJNIEnv->CallObjectMethod(lWindow, MethodGetDecorView); 

    if(pShow) 
    { 
        // Runs lInputMethodManager.showSoftInput(...). 
        jmethodID MethodShowSoftInput = lJNIEnv->GetMethodID( ClassInputMethodManager, "showSoftInput", "(Landroid/view/View;I)Z"); 
        jboolean lResult = lJNIEnv->CallBooleanMethod( lInputMethodManager, MethodShowSoftInput, lDecorView, lFlags);
    } 
    else 
    { 
        // Runs lWindow.getViewToken() 
        jclass ClassView = lJNIEnv->FindClass( "android/view/View"); 
        jmethodID MethodGetWindowToken = lJNIEnv->GetMethodID( ClassView, "getWindowToken", "()Landroid/os/IBinder;"); 
        jobject lBinder = lJNIEnv->CallObjectMethod(lDecorView,MethodGetWindowToken); 

        // lInputMethodManager.hideSoftInput(...). 
        jmethodID MethodHideSoftInput = lJNIEnv->GetMethodID( ClassInputMethodManager, "hideSoftInputFromWindow", "(Landroid/os/IBinder;I)Z"); 
        jboolean lRes = lJNIEnv->CallBooleanMethod( lInputMethodManager, MethodHideSoftInput, lBinder, lFlags); 
    } 

    // Finished with the JVM. 
    lJavaVM->DetachCurrentThread(); 

    return true;
}    
#endif

    bool App::ShowKeyboard()
    {
#if ANDROID
        TRACE_LOG("Showing keyboard")
        if(displayKeyboard(pActivity_, true))
        {
            isKeyboardVisible_ = true;
        }
        return true;
#else
        return false;        
#endif        

    }

    bool App::HideKeyboard()
    {
#if ANDROID        
        TRACE_LOG("Hiding keyboard")
        if(displayKeyboard(pActivity_, false))
        {
            isKeyboardVisible_ = false;
        }
        return true;
#else
        return false;        
       
#endif        
    }


    void App::BeginSelection(PGLES2FrameColorSelection pFrameColorSelection) 
    { 
        pFrameColorSelection_ = pFrameColorSelection; 
    }

    void App::EndSelection()
    {
        selectedIndex_ = pFrameColorSelection_->GetSelected();

        pFrameColorSelection_ = nullptr;
    }


    InternalApp::InternalApp(App* pApp) 
    : pApp_(pApp),
    screenX_(0),
    screenY_(0)
    {
    }

    InternalApp::~InternalApp()
    {
        pApp_ = nullptr;
    }

    void InternalApp::Initialize()
    {
        IMGUI::AllocateResources();
        Tick::Initialize(pApp_->GetFPS());
    }

    void InternalApp::Release()
    {
        IMGUI::ReleaseResources();
        GLES2Text::ReleaseAtlasCollection();
    }

    void InternalApp::BeginTick()
    {
        pApp_->Start();
    }
    
    void InternalApp::DoTick(float delta)
    {
        pApp_->deltaTime_ = delta;
        IMGUI::DoTick();
        pApp_->Update();
    }
    
    void InternalApp::EndTick()
    {
        pApp_->LateUpdate();
    }

    void InternalApp::ViewChanged(int32_t width, int32_t height)
    {
        if(width > 0 && height > 0)
        {
            TRACE_LOG("ViewChanged: width=" << width << " height=" << height);
            
            glViewport(0, 0, width, height);

            pApp_->SetViewSize(width, height);

            if(!pFrameColorSelection_)
                pFrameColorSelection_ = PGLES2FrameColorSelection(new GLES2FrameColorSelection());

            pFrameColorSelection_->ViewChanged(width, height);

            GLES2Render2Texture::Renderers& renderers = GLES2Render2Texture::GetRenderers();
            for(auto &renderer : renderers)
            {
                renderer->ViewChanged(width, height);
            }

            IMGUI::ViewChanged(width, height);

            GLES2Camera::Cameras& cameras = GLES2Camera::GetCameras();

            for(auto &camera : cameras)
            {
                camera->ViewChanged(width, height);
            }

            pApp_->ViewChanged(width, height);
        }
    }

    void InternalApp::OnMouseMove(float x, float y) 
    {
        screenX_ = x;
        screenY_ = y;

        IMGUI::OnMouseMove(x, y);
        pApp_->OnMouseMove(x, y);
    }

    void InternalApp::OnMouseDown(float x, float y) 
    {
        TRACE_LOG("Mouse Down");
        screenX_ = x;
        screenY_ = y;

        IMGUI::OnMouseDown(x, y);
        pApp_->OnMouseDown(x, y);
    }

    void InternalApp::OnMouseUp() 
    {
        TRACE_LOG("Mouse Up");
        IMGUI::OnMouseUp();
        pApp_->OnMouseUp();
    }

    void InternalApp::OnKey(int key, int action, int modifier)
    {
        TRACE_LOG("key=" << key << " action=" << action << " modifier=" << modifier);
        IMGUI::OnKey(key, action, modifier);
        pApp_->OnKey(key, action, modifier);
    }

    void InternalApp::OnChar(unsigned int character)
    {
        TRACE_LOG("character=" << character);
        IMGUI::OnChar(character);
        pApp_->OnChar(character);
        
    }

    void InternalApp::RenderFrame()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glClearColor(0, 0, 0, 1);
        glClearDepth(1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        //glEnable(GL_BLEND);
        //glEnable(GL_CULL_FACE);
        //glCullFace(GL_FRONT);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        pApp_->RenderFrame();

        pApp_->BeginSelection(pFrameColorSelection_);
        pFrameColorSelection_->Begin(screenX_, screenY_);
        pApp_->RenderFrame();
        pFrameColorSelection_->End();
        pApp_->EndSelection();

        IMGUI::Begin();
        pApp_->RenderGUIFrame();
        IMGUI::End();
    }
    bool InternalApp::HideKeyboard()
    {
        return pApp_->HideKeyboard();
    }

#if NACL
    void InternalApp::HandleMessage(const pp::Var& var_message)
    {
        pApp_->HandleMessage(var_message);
    }
#elif ANDROID
    void InternalApp::SetAssetManager(AAssetManager* pAAssetManager)
    {
        pApp_->SetAssetManager(pAAssetManager);
    }

    void InternalApp::SetActivity(ANativeActivity* pActivity)
    {
        pApp_->SetActivity(pActivity);
    }
#endif

}