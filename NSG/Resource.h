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
#include <string>
#include <memory>
#include "SharedPointers.h"

class AAssetManager;
namespace NSG
{
	struct IProceduralResource
	{
		virtual void Build() = 0;
		virtual const char* GetName() const = 0;
	};

	class Resource
	{
	public:
		Resource(IProceduralResource* obj);
		Resource(const char* filename);
		Resource(const char* buffer, size_t bytes);
		~Resource();
		bool IsLoaded();
		const char* const GetData() const;
		size_t GetBytes() const;
        std::string GetFilename() const { return filename_; }
        void Invalidate();
	private:
		bool loaded_;
		std::string filename_;
		const char* pData_;
		std::string buffer_;
		const char* staticBuffer_;
		size_t bytes_;
		NaCl::PNaClURLLoader pLoader_;
		AAssetManager* pAAssetManager_;	
		IProceduralResource* proceduralObj_;
	};
}