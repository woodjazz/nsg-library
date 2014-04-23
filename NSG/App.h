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

#include <memory>
#if NACL
#include "ppapi/cpp/var.h"
#elif ANDROID
#include <android/asset_manager.h>
#include <android/native_activity.h>
#endif

#include "Tick.h"
#include "GLES2FrameColorSelection.h"


namespace NSG 
{
    struct InternalApp;
	class App 
	{
	public:
        App();
		virtual ~App();
        virtual int GetFPS() const;
		virtual void Start() {}
		virtual void Update() {}
		virtual void RenderFrame() = 0;
        virtual void RenderGUIFrame() {}
		virtual void ViewChanged(int32_t width, int32_t height) = 0;
        virtual void OnMouseMove(float x, float y) {}
        virtual void OnMouseDown(float x, float y) {}
        virtual void OnMouseUp() {}
        virtual void OnKey(int key, int action, int modifier) {}
        virtual void OnChar(unsigned int character) {}
        void Initialize();
        void Release();
        void DoTick(float delta);
        bool ShowKeyboard();
        bool HideKeyboard();
        bool IsKeyboardVisible() { return isKeyboardVisible_; }
        static App* GetPtrInstance();
        void SetViewSize(int32_t width, int32_t height);
        std::pair<int32_t, int32_t> GetViewSize() const;
        void BeginSelection(float screenX, float screenY);
        void EndSelection();
        PGLES2FrameColorSelection GetSelection() const { return pFrameColorSelection_; }
        GLushort GetSelectedNode() const { return selectedIndex_; }
        float GetDeltaTime() const { return deltaTime_; }

#if NACL
		virtual void HandleMessage(const pp::Var& var_message);
#elif ANDROID
		void SetAssetManager(AAssetManager* pAAssetManager) { pAAssetManager_ = pAAssetManager; }
        AAssetManager* GetAssetManager() { return pAAssetManager_; }
        void SetActivity(ANativeActivity* pActivity) { pActivity_ = pActivity; }
#endif
    private:
#if ANDROID        
        AAssetManager* pAAssetManager_;
        ANativeActivity* pActivity_;
#endif
        int32_t width_;
        int32_t height_;
        PGLES2FrameColorSelection pFrameColorSelection_;
        GLushort selectedIndex_;
        bool isKeyboardVisible_;
        // The time in seconds it took to complete the last frame
        float deltaTime_;
        friend struct InternalApp;
	};

	typedef std::shared_ptr<App> PApp;

    struct InternalApp : public Tick
    {
        PApp pApp_;
        float screenX_;
        float screenY_;

        InternalApp(App* pApp);
        ~InternalApp();
        void Initialize();
        void Release();
        void BeginTick();
        void DoTick(float delta);
        void EndTick();
        void ViewChanged(int32_t width, int32_t height);
        void OnMouseMove(float x, float y);
        void OnMouseDown(float x, float y);
        void OnMouseUp();
        void OnKey(int key, int action, int modifier);
        void OnChar(unsigned int character);
        void RenderFrame();
        bool HideKeyboard();
	#if NACL
		void HandleMessage(const pp::Var& var_message);
	#elif ANDROID
		void SetAssetManager(AAssetManager* pAAssetManager);
        void SetActivity(ANativeActivity* pActivity);
	#endif
    };

    typedef std::shared_ptr<InternalApp> PInternalApp;
}	
