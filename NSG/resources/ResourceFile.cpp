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
#include "ResourceFile.h"
#include "Check.h"
#include "App.h"
#include "Util.h"
#include <fstream>

namespace NSG
{
    ResourceFile::ResourceFile(const Path& path)
        : path_(path)
    {
    }

    ResourceFile::~ResourceFile()
    {
        Invalidate();
    }

    bool ResourceFile::IsValid()
    {
        return path_.HasName();
    }

    void ResourceFile::AllocateResources()
    {
        #if SDL
        {
            SDL_RWops* context = SDL_RWFromFile(path_.GetFilePath().c_str(), "rb");

            if (!context)
            {
                if (path_.AppendDirIfDoesNotExist("data"))
                    context = SDL_RWFromFile(path_.GetFilePath().c_str(), "rb");
            }

            if (context)
            {
                SDL_RWseek(context, 0, RW_SEEK_END);
                Sint64 filelength = SDL_RWtell(context);
                SDL_RWseek(context, 0, RW_SEEK_SET);
                buffer_.resize((int)filelength);
				SDL_RWread(context, &buffer_[0], buffer_.size(), 1);
                SDL_RWclose(context);
                TRACE_LOG(path_.GetFilePath() << " has been loaded with size=" << buffer_.size());
            }
            else
            {
                TRACE_LOG("Cannot load " << path_.GetFilePath() << " with error " << SDL_GetError());
            }
        }
        #else
        {
            std::ifstream file(path_.GetFilePath().c_str(), std::ios::binary);

            if (!file.is_open())
            {
                if (path_.AppendDirIfDoesNotExist("data"))
                    file.open(path_.GetFilePath().c_str(), std::ios::binary);
            }

            if (file.is_open())
            {
                file.seekg(0, std::ios::end);
                std::streampos filelength = file.tellg();
                file.seekg(0, std::ios::beg);
                buffer_.resize((int)filelength);
                file.read(&buffer_[0], filelength);
                CHECK_ASSERT(file.gcount() == filelength, __FILE__, __LINE__);
                file.close();
                TRACE_LOG(path_.GetFilePath() << " has been loaded with size=" << buffer_.size());
            }
            else
            {
                TRACE_LOG("Cannot load " << path_.GetFilePath() << " with error " << SDL_GetError());
            }
        }
        #endif
    }

    void ResourceFile::ReleaseResources()
    {
        TRACE_PRINTF("Releasing memory for file: %s\n", path_.GetFilePath().c_str());
        Resource::ReleaseResources();
    }
}