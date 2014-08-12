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
#include "SharedPointers.h"
#include "Singleton.h"
#include <set>

namespace NSG
{
	class GPUObject;
	class Resource;
	struct Context : public Singleton<Context>
	{
		std::set<GPUObject*> objects_;
		std::set<Resource*> resources_;
		PFontAtlasTextureManager atlasManager_;
		IMGUI::PContext imgui_;
		PKeyboard keyboard_;
		PBufferManager bufferManager_;
		PAudio audio_;
		PGraphics graphics_;

		Context();
		~Context();
		void Initialize();
		static void AddObject(GPUObject* object);
		static void RemoveObject(GPUObject* object);
		void InvalidateGPUResources();
		static void AddResource(Resource* object);
		static void RemoveResource(Resource* object);
		void ReleaseResourcesFromMemory();
		PTexture GetWhiteTexture();

	private:
		void Add(GPUObject* object);
		void Remove(GPUObject* object);
		void Add(Resource* object);
		void Remove(Resource* object);

		PTexture whiteTexture_;
	};
}
