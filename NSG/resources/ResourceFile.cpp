/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2016 Néstor Silveira Gorski

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
#include "Util.h"
#include <fstream>

namespace NSG
{
    ResourceFile::ResourceFile(const Path& path)
        : Resource(path.GetFullAbsoluteFilePath()),
          path_(path)
    {
		EnableInvalidation();
        #if defined(EMSCRIPTEN)
        isLocal_ = false;
        #else
        isLocal_ = true;
        #endif

        onLoad_ = [this](const std::string & data)
        {
            LOGI("HTTP Loaded %s with size = %u", name_.c_str(), (unsigned)data.size());
            buffer_ = data;
            isLocal_ = true;
        };

        onError_ = [this](int httpError, const std::string & description)
        {
            LOGI("HTTP Failed loading %s: %d. %s", name_.c_str(), httpError, description.c_str());
        };

        onProgress_ = [this](unsigned percentage)
        {
            LOGI("HTTP Progress for %s: %d", name_.c_str(), percentage);
        };
    }

    ResourceFile::~ResourceFile()
    {
    }

    bool ResourceFile::IsValid()
    {
        if (!get_ && !isLocal_)
        {
            if (path_.IsPathRelative())
            {
                #if defined(EMSCRIPTEN)
                std::string url = emscripten_run_script_string("window.location.href");
                #else
                std::string url; //@@@ TO DO
                CHECK_ASSERT(false);
                #endif
                auto filename = path_.GetFilePath();
                get_ = std::make_shared<HTTPRequest>(Path(url).GetPath() + "/" + filename, onLoad_, onError_, onProgress_);
            }
            else
            {
                get_ = std::make_shared<HTTPRequest>(path_.GetFullAbsoluteFilePath(), onLoad_, onError_, onProgress_);
            }

            get_->StartRequest();
        }
        return isLocal_;
    }

    void ResourceFile::AllocateResources()
    {
        if (!get_)
        {
            #if defined(EMSCRIPTEN) || !defined(SDL)
            {
                auto filename = path_.GetFilePath();
                std::ifstream file(filename.c_str(), std::ios::binary);
                if (file.is_open())
                {
                    file.seekg(0, std::ios::end);
                    std::streampos filelength = file.tellg();
                    file.seekg(0, std::ios::beg);
                    buffer_.resize((int)filelength);
                    file.read(&buffer_[0], filelength);
                    CHECK_ASSERT(file.gcount() == filelength);
                    file.close();
                    LOGI("%s has been loaded with size=%u", filename.c_str(), (unsigned)buffer_.size());
                }
                else
                {
                    LOGE("Cannot load %s", filename.c_str());
                }
            }
            #else
            {
                CHECK_ASSERT(isLocal_);
                auto filename = path_.GetFilePath();
                SDL_RWops* context = SDL_RWFromFile(filename.c_str(), "rb");
                if (context)
                {
                    SDL_RWseek(context, 0, RW_SEEK_END);
                    Sint64 filelength = SDL_RWtell(context);
                    SDL_RWseek(context, 0, RW_SEEK_SET);
                    buffer_.resize((int)filelength);
                    SDL_RWread(context, &buffer_[0], buffer_.size(), 1);
                    SDL_RWclose(context);
                    LOGI("%s has been loaded with size=%u", filename.c_str(), (unsigned)buffer_.size());
                }
                else
                {
                    LOGE("Cannot load %s with error %s", filename.c_str(), SDL_GetError());
                }
            }
            #endif
        }

        if (path_.GetExtension() == "lz4")
            buffer_ = DecompressBuffer(buffer_);
    }

    void ResourceFile::ReleaseResources()
    {
        LOGI("Releasing memory for file: %s", name_.c_str());
        Resource::ReleaseResources();
        get_ = nullptr;
    }
}