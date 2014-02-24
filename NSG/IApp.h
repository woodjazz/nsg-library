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
#endif

#include "Tick.h"

namespace NSG 
{
	class IApp 
	{
	public:
        IApp();
		virtual ~IApp();
		virtual void Start() {}
		virtual void Update(float delta) {}
		virtual void LateUpdate() {}
		virtual void RenderFrame() = 0;
		virtual void ViewChanged(int32_t width, int32_t height) = 0;
        virtual void OnMouseMove(double x, double y) {}
        virtual void OnMouseDown(double x, double y) {}
        virtual void OnMouseUp() {}
        static IApp* GetPtrInstance();
        void SetViewSize(int32_t width, int32_t height);
        std::pair<int32_t, int32_t> GetViewSize() const;

	#if NACL
		virtual void HandleMessage(const pp::Var& var_message);
	#elif ANDROID
		void SetAssetManager(AAssetManager* pAAssetManager) { pAAssetManager_ = pAAssetManager; }
        AAssetManager* GetAssetManager() { return pAAssetManager_; }
	#endif
    private:
    #if ANDROID        
        AAssetManager* pAAssetManager_;
    #endif
    int32_t width_;
    int32_t height_;
	};

	typedef std::shared_ptr<IApp> PApp;

    struct InternalApp : public Tick
    {
        NSG::PApp pApp_;

        InternalApp(NSG::PApp pApp);
        void Initialize(int fps);
        void BeginTick();
        void DoTick(float delta);
        void EndTick();
        void ViewChanged(int32_t width, int32_t height);
        void OnMouseMove(double x, double y);
        void OnMouseDown(double x, double y);
        void OnMouseUp();
        void RenderFrame();
	#if NACL
		void HandleMessage(const pp::Var& var_message);
	#elif ANDROID
		void SetAssetManager(AAssetManager* pAAssetManager);
	#endif
    };

    typedef std::shared_ptr<InternalApp> PInternalApp;
}	
