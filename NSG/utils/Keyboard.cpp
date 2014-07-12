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
#include "Keyboard.h"
#if SDL
#include "SDL.h"
#undef main
#endif

#if ANDROID
#include <android/asset_manager.h>
#include <android/native_activity.h>
#endif

namespace NSG
{
#if ANDROID	
    bool DisplayKeyboard(ANativeActivity* pActivity, bool pShow) 
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

	Keyboard::Keyboard()
	{

	}

	Keyboard::~Keyboard()
	{

	}

	bool Keyboard::Enable()
	{
#if ANDROID	
		return DisplayKeyboard(activity_, true);
#elif SDL
		SDL_StartTextInput();
#endif
		return true;
	}

	bool Keyboard::Disable()
	{
#if ANDROID	
		return DisplayKeyboard(activity_, false);
#elif SDL
		SDL_StopTextInput();
#endif
		return true;
    }

}
