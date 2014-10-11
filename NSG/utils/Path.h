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

namespace NSG
{
    class Path
    {
    public:
        Path(const std::string& filePath);
		Path(const Path& obj);
        ~Path();
        const std::string& GetFilePath() const { return filePath_; }
		const std::string& GetPath() const { return path_; }
        const std::string& GetFilename() const { return filename_; }
		const std::string& GetExtension() const { return ext_;  }
        bool HasName() const { return !filename_.empty(); }
        bool HasPath() const { return !path_.empty(); }
        bool IsPathRelative() const { return !isAbsolutePath_; }
    private:
        void ReplaceChar(std::string& filePath, char from, char to);
        std::string ExtractPath(const std::string& filePath);
        std::string ExtractFilename(const std::string& filePath);
        std::string GetLowercaseFileExtension(const std::string& filePath);
    private:
        std::string filePath_;
        std::string path_; // only path (without filename)
        std::string filename_; // name + extension (without path)
        std::string ext_; // extension in lowercase
        bool isAbsolutePath_;
    };
}