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
        : path_(path),
          trySecondTime_(true)
    {
    }

    ResourceFile::~ResourceFile()
    {
    }

    bool ResourceFile::IsValid()
    {
        return path_.HasName();
    }

    void ResourceFile::AllocateResources()
    {
        bool loaded = false;

        #if ANDROID
        {
            SDL_RWops* assetHandle = SDL_RWFromFile(path_.GetFilePath().c_str(), "rb");

            if (!assetHandle)
            {
                if (trySecondTime_)
                {
                    trySecondTime_ = false;
                    if (path_.AppendDirIfDoesNotExist("data"))
                        assetHandle = SDL_RWFromFile(path_.GetFilePath().c_str(), "rb");
                }
            }

            if (assetHandle)
            {
                off_t filelength = assetHandle->hidden.androidio.size;
                buffer_.resize((int)filelength);
                SDL_RWread(assetHandle, &buffer_[0], filelength, 1);
                SDL_RWclose(assetHandle);
                loaded = true;
            }
        }
        #else
        {
            SetCurrentDir(path_.GetAbsolutePath());

            std::ifstream file(path_.GetFilename(), std::ios::binary);

            if (!file.is_open())
            {
                if (path_.AppendDirIfDoesNotExist("data"))
                    file.open(path_.GetFilePath().c_str(), std::ios::binary);
            }

            #if defined(__APPLE__) && defined(SDL)
            {
                if (!file.is_open())
                {
                    char* base_path = SDL_GetBasePath();
                    CHECK_CONDITION(base_path, __FILE__, __LINE__);
                    std::string path(base_path);
                    SDL_free(base_path);
                    if (path_.IsPathRelative())
                    {
                        path += "data/";
                    }
                    path += path_.GetFilename();
                    file.open(path.c_str(), std::ios::binary);
                }
            }
            #endif

            if (file.is_open())
            {
                file.seekg(0, std::ios::end);
                std::streampos filelength = file.tellg();
                file.seekg(0, std::ios::beg);
                buffer_.resize((int)filelength);
                file.read(&buffer_[0], filelength);
                CHECK_ASSERT(file.gcount() == filelength, __FILE__, __LINE__);
                file.close();
                loaded = true;
            }
        }
        #endif

        if (loaded)
        {
            TRACE_LOG("Resource::File " << path_.GetFilePath() << " has been loaded with size=" << buffer_.size());
            signalLoaded_->Run();
        }
        else
        {
            TRACE_LOG("Resource::Cannot load file " << path_);
        }
    }
}