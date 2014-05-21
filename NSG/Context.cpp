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
#include "GLES2GPUObject.h"
#include "GLES2FontAtlasTextureManager.h"
#include "GLES2FrameColorSelection.h"
#include "GLES2Texture.h"
#include "Keyboard.h"

namespace NSG
{
	Context::Context()
	: pFrameColorSelection_(new GLES2FrameColorSelection(false)),
	atlasManager_(new GLES2FontAtlasTextureManager),
	imgui_(new IMGUI::Context),
	keyboard_(new Keyboard)
	{
	        // Creates 1x1 white texture 
			char img[3];
			memset(&img[0], 0xFF, sizeof(unsigned char)*3);
	        whiteTexture_ = PGLES2Texture(new GLES2Texture(GL_RGB, 1, 1, &img[0]));
	}
		
	Context::~Context()
	{
		atlasManager_ = nullptr;
	}

	void Context::Add(GLES2GPUObject* object)
	{
		auto result = objects_.insert(object);

		CHECK_ASSERT(result.second && "Context::Add has failed in file", __FILE__, __LINE__);
	}

	void Context::Remove(GLES2GPUObject* object)
	{
		object->Release();
		auto result = objects_.erase(object);
		CHECK_ASSERT(result && "Context::Remove has failed in file", __FILE__, __LINE__);
	}

	void Context::InvalidateGPUResources()
	{
        TRACE_LOG("Context::InvalidateGPUResources...");

		auto it = objects_.begin();
		while(it != objects_.end())
		{
			(*it)->Invalidate();
			++it;
		}

        TRACE_LOG("Context::InvalidateGPUResources done");
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

	void Context::InvalidateResources()
	{
        TRACE_LOG("Context::InvalidateResources...");

		auto it = resources_.begin();
		while(it != resources_.end())
		{
			(*it)->Invalidate();
			++it;
		}

        TRACE_LOG("Context::InvalidateResources done");
	}
}
