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
#include "Context.h"
#include "Check.h"
#include "GPUObject.h"
#include "FontAtlasTextureManager.h"
#include "TextureMemory.h"
#include "Keyboard.h"
#include "Resource.h"
#include "IMGUIContext.h"
#include "Graphics.h"
#include "BufferManager.h"
#include "Audio.h"

namespace NSG
{
	template<> Context* Singleton<Context>::this_ = nullptr;

    Context::Context()
        : atlasManager_(new FontAtlasTextureManager),
        imgui_(new IMGUI::Context),
        keyboard_(new Keyboard),
        bufferManager_(new BufferManager)
    {
    }

    Context::~Context()
    {
		Context::this_ = nullptr;
        atlasManager_ = nullptr;
    }

	void Context::AddObject(GPUObject* object)
    {
		if (Context::this_)
			Context::this_->Add(object);
    }

	void Context::RemoveObject(GPUObject* object)
    {
		object->Release();

		if (Context::this_)
			Context::this_->Remove(object);
	}
	
	void Context::Add(GPUObject* object)
	{
		auto result = objects_.insert(object);

		CHECK_ASSERT(result.second && "Context::Add has failed in file", __FILE__, __LINE__);
	}

	void Context::Remove(GPUObject* object)
	{
		auto result = objects_.erase(object);
		CHECK_ASSERT(result && "Context::Remove has failed in file", __FILE__, __LINE__);
	}

    void Context::InvalidateGPUResources()
    {
        TRACE_LOG("Context::InvalidateGPUResources...");

        auto it = objects_.begin();
        while (it != objects_.end())
        {
            (*it)->Invalidate();
            ++it;
        }

        bufferManager_->Invalidate();

        ResetCachedState();

        TRACE_LOG("Context::InvalidateGPUResources done");
    }

	void Context::AddResource(Resource* object)
	{
		if (Context::this_)
			Context::this_->Add(object);
	}

	void Context::RemoveResource(Resource* object)
	{
		if (Context::this_)
			Context::this_->Remove(object);
	}


    void Context::Add(Resource* object)
    {
        auto result = resources_.insert(object);

        CHECK_ASSERT(result.second && "Context::Add has failed in file", __FILE__, __LINE__);
    }

    void Context::Remove(Resource* object)
    {
        auto result = resources_.erase(object);
        CHECK_ASSERT(result && "Context::Remove has failed in file", __FILE__, __LINE__);
    }

    void Context::ReleaseResourcesFromMemory()
    {
        TRACE_LOG("Context::ReleaseResourcesFromMemory...");

        auto it = resources_.begin();
        while (it != resources_.end())
        {
            (*it)->Invalidate();
            ++it;
        }

        TRACE_LOG("Context::ReleaseResourcesFromMemory done");
    }

    PTexture Context::GetWhiteTexture()
    {
        if (!whiteTexture_)
        {
            const int WIDTH = 1;
            const int HEIGHT = 1;
            // Creates 1x1 white texture
            static unsigned char img[WIDTH * HEIGHT * 3];
            memset(&img[0], 0xFF, sizeof(img));
            whiteTexture_ = PTexture(new TextureMemory(GL_RGB, WIDTH, HEIGHT, (char*)&img[0]));
            TRACE_LOG("White texture has been generated.")
        }

        return whiteTexture_;
    }

    void Context::Initialize()
    {
        audio_ = PAudio(new Audio);
    }
}
