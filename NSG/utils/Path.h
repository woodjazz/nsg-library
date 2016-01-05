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
#pragma once
#include <string>
#include <fstream>
#include <vector>

namespace NSG
{
    class Path
    {
    public:
        static const int MAX_PATH_SIZE = 1024;
        Path();
        Path(const char* filePath);
        Path(const std::string& filePath);
        Path(const Path& obj);
        ~Path();
        const std::string& GetFilePath() const { return filePath_; }
        const std::string& GetFullAbsoluteFilePath() const { return fullFilePath_; }
        const std::string& GetPath() const { return path_; }
        const std::string& GetAbsolutePath() const { return absolutePath_; }
        const std::string& GetPathAndName() const { return pathName_; }
        const std::string& GetFilename() const { return filename_; }
        const std::string& GetName() const { return name_; }
        const std::string& GetExtension() const { return ext_;  }
        void SetPath(const std::string& path);
        void SetName(const std::string& name);
        void SetExtension(const std::string& ext);
		void AddExtension(const std::string& ext);
        void SetFileName(const std::string& fileName);
        bool HasName() const { return !filename_.empty(); }
        bool HasExtension() const { return !ext_.empty(); }
        bool HasPath() const { return !path_.empty(); }
        bool IsPathRelative() const { return !isAbsolutePath_; }
        static const Path& GetEmpty();
        bool IsEmpty() const { return filePath_.empty(); }
        static std::string GetCurrentDir();
        static const std::string& GetBasePath();
        static void ReplaceChar(std::string& filePath, char from, char to);
        static void ReplaceString(std::string& filePath, const std::string& from, const std::string& to);
        static std::string ExtractPath(const std::string& filePath);
        static std::string ExtractFilename(const std::string& filePath, bool extension);
        static std::string GetLowercaseFileExtension(const std::string& filePath);
		static void RemoveChar(std::string& path, char ch);
        static std::vector<std::string> GetDirs(const std::string& path);
        static bool ContainsDir(const std::string& path, const std::string& dirName);
        bool operator < (const Path& obj) const;
        bool AppendDirIfDoesNotExist(const std::string& dirName);
		bool Exists() const;
        std::string GetModificationTime() const;
    private:
        void ReDoState();
        std::string filePath_; // path + filename (could be absolute or relative)
        std::string fullFilePath_; // absolute path + filename
        std::string pathName_; // path + name (without extension)
        std::string path_; // only path (without filename)
        std::string absolutePath_; // only absolute path (without filename)
        std::string name_; // name (name only without extension)
        std::string filename_; // name + extension (without path)
        std::string ext_; // extension in lowercase
        bool isAbsolutePath_;
        static std::string basePath_;
    };

    std::ostream& operator << (std::ostream& s , const Path& obj);

}