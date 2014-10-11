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
#include "Path.h"
#include <regex>
#include <cctype>

namespace NSG
{
    Path::Path(const std::string& filePath)
        : filePath_(filePath)
    {
        ReplaceChar(filePath_, '\\', '/');
        path_ = ExtractPath(filePath_);
        filename_ = ExtractFilename(filePath_);
        ext_ = GetLowercaseFileExtension(filePath_);

        isAbsolutePath_ = filePath_.size() && (filePath_[0] == '/');

        #if WIN32
        {
            if (!isAbsolutePath_ && filePath_.size() > 2)
                isAbsolutePath_ = std::isalpha(filePath_[0]) && filePath_[1] == ':' && filePath_[2] == '/';
        }
        #endif
    }

    Path::Path(const Path& obj)
        : filePath_(obj.filePath_),
          path_(obj.path_),
          filename_(obj.filename_),
          ext_(obj.ext_),
          isAbsolutePath_(obj.isAbsolutePath_)
    {

    }

    Path::~Path()
    {

    }

    void Path::ReplaceChar(std::string& filePath, char from, char to)
    {
        for (;;)
        {
            const size_t last_slash_idx = filePath.find_last_of(from);
            if (std::string::npos == last_slash_idx) break;
            filePath.replace(last_slash_idx, 1, 1, to);
        }
    }

    std::string Path::ExtractPath(const std::string& filePath)
    {
        const size_t idx = filePath.find_last_of('/');
        if (idx != std::string::npos)
            return filePath.substr(0, idx);

        return "";
    }

    std::string Path::ExtractFilename(const std::string& filePath)
    {
        const size_t idx = filePath.find_last_of('/');
        if (idx != std::string::npos)
            return filePath.substr(idx + 1);
        else
            return filePath;
    }

    std::string Path::GetLowercaseFileExtension(const std::string& filePath)
    {
        std::string extension;
        std::string::size_type pos = filePath.find_last_of(".");
        if (pos != std::string::npos)
        {
            std::copy(filePath.begin() + pos + 1, filePath.end(), std::back_inserter(extension));
            for (auto& ch : extension)
                ch = std::tolower(ch);
        }

        return extension;
    }
}