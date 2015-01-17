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
#include "Check.h"
#include "App.h"
#if WIN32
#include <Windows.h>
#elif defined(EMSCRIPTEN)
#include <emscripten.h>
#include <unistd.h>
#elif defined(__APPLE__) && defined(SDL)
#include "SDL.h"
#undef main
#else
#include <stdio.h>
#include <unistd.h>
#include <cerrno>
#endif

#include <regex>
#include <mutex>
#include <cctype>

namespace NSG
{
    std::string Path::basePath_;

    Path::Path()
        : isAbsolutePath_(false)
    {
    }

    Path::Path(const char* filePath)
        : Path(std::string(filePath))
    {
    }

    Path::Path(const std::string& filePath)
        : filePath_(filePath)
    {
        ReDoState();
    }

    Path::Path(const Path& obj)
        : filePath_(obj.filePath_),
          fullFilePath_(obj.fullFilePath_),
          pathName_(obj.pathName_),
          path_(obj.path_),
          absolutePath_(obj.absolutePath_),
          name_(obj.name_),
          filename_(obj.filename_),
          ext_(obj.ext_),
          isAbsolutePath_(obj.isAbsolutePath_)
    {

    }

    Path::~Path()
    {

    }

    void Path::SetPath(const std::string& path)
    {
        filePath_ = path + "/";
        if(HasName())
        {
            filePath_ += name_;
            if(HasExtension())
                filePath_ += "." + ext_;
        }
        ReDoState();
 
    }

    void Path::SetName(const std::string& name)
    {
        filePath_ = path_ + "/" + name + "." + ext_;
        ReDoState();
    }

    void Path::SetExtension(const std::string& ext)
    {
        filePath_ = pathName_ + "." + ext;
        ReDoState();
    }

    void Path::SetFileName(const std::string& fileName)
    {
        filePath_ = path_ + "/" + Path::ExtractFilename(fileName, true);
        ReDoState();
    }

    void Path::ReDoState()
    {
        Path::RemoveCurrentDirSymbol(filePath_);
        Path::ReplaceChar(filePath_, '\\', '/');
        path_ = Path::ExtractPath(filePath_);
        filename_ = Path::ExtractFilename(filePath_, true);
        name_ = Path::ExtractFilename(filePath_, false);
        if (!path_.empty() || !name_.empty())
            pathName_ = path_ + "/" + name_;
        ext_ = Path::GetLowercaseFileExtension(filePath_);

        isAbsolutePath_ = filePath_.size() && (filePath_[0] == '/');

        #if WIN32
        {
            if (!isAbsolutePath_ && filePath_.size() > 2)
                isAbsolutePath_ = std::isalpha(filePath_[0]) && filePath_[1] == ':' && filePath_[2] == '/';
        }
        #endif

        if (isAbsolutePath_)
            absolutePath_ = path_;
        else
            absolutePath_ = Path::GetBasePath() + path_;

        fullFilePath_ = absolutePath_ + "/" + filename_;
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
        std::string path;
        const size_t idx = filePath.find_last_of('/');
        if (idx != std::string::npos)
        {
            path = filePath.substr(0, idx);
            Path::RemoveCurrentDirSymbol(path);
        }

        return path;
    }

    void Path::RemoveCurrentDirSymbol(std::string& path)
    {
        std::string::size_type pos = path.find("./");
        if(pos != std::string::npos)
            Path::RemoveCurrentDirSymbol(path.erase(pos, pos+2));
    }

    std::string Path::ExtractFilename(const std::string& filePath, bool extension)
    {
        std::string filename;
        const size_t idx = filePath.find_last_of('/');
        if (idx != std::string::npos)
            filename = filePath.substr(idx + 1);
        else
            filename = filePath;

        if (!extension)
        {
            std::string::size_type pos = filename.find_last_of(".");
            if (pos != std::string::npos)
                return filename.substr(0, pos);
        }

        return filename;
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

    std::vector<std::string> Path::GetDirs(const std::string& path)
    {
        std::vector<std::string> dirs;
        std::string tmp(path);
        std::string::size_type pos = tmp.find("/");
        while (pos != std::string::npos)
        {
            dirs.push_back(tmp.substr(0, pos));
            tmp = tmp.substr(pos + 1);
            pos = tmp.find("/");
        }

        if (!tmp.empty() && tmp != ".")
            dirs.push_back(tmp);

        return dirs;
    }

    const Path& Path::GetEmpty()
    {
        static Path path;
        return path;
    }

    const std::string& Path::GetBasePath()
    {
        static std::once_flag flag;
        std::call_once(flag, [&]()
        {
            basePath_ = Path::GetCurrentDir();
        });
        return basePath_;
    }

    std::string Path::GetCurrentDir()
    {
        #if WIN32
        {
            char buffer[MAX_PATH_SIZE];
            DWORD n = ::GetCurrentDirectoryA(sizeof(buffer), buffer);
            CHECK_CONDITION(n > 0 && n < sizeof(buffer), __FILE__, __LINE__);
            std::string result(buffer, n);
            if (result[n - 1] != '\\')
                result.append("/");

            Path::ReplaceChar(result, '\\', '/');
            return result;
        }
        #elif (defined(ANDROID) || defined(__APPLE__)) && defined(SDL)
        {
            std::string path;
            char* base_path = SDL_GetBasePath();
            if (base_path)
            {
                path = base_path;
                SDL_free(base_path);
            }
            else
            {
                path = "./";
            }
            return path;
        }
        #else
        {
            std::string path;
            char cwd[MAX_PATH_SIZE];
            getcwd(cwd, sizeof(cwd));
            CHECK_CONDITION(getcwd(cwd, sizeof(cwd)), __FILE__, __LINE__);
            path = cwd;
            std::string::size_type n = path.size();
            if (n > 0 && path[n - 1] != '/') path.append("/");
            return path;
        }
        #endif
    }

    bool Path::operator < (const Path& obj) const
    {
        return fullFilePath_ < obj.fullFilePath_;
    };

    std::ostream& operator << (std::ostream& s , const Path& obj)
    {
        s << "IsRelativePath=" << std::boolalpha << obj.IsPathRelative() << "\n";
        s << "FilePath=" << obj.GetFilePath() << "\n";
        s << "FullAbsoluteFilePath=" << obj.GetFullAbsoluteFilePath() << "\n";
        s << "Path=" << obj.GetPath() << "\n";
        s << "AbsolutePath=" << obj.GetAbsolutePath() << "\n";
        s << "PathAndName=" << obj.GetPathAndName() << "\n";
        s << "Filename=" << obj.GetFilename() << "\n";
        s << "Extension=" << obj.GetExtension() << "\n";
        return s;
    }

    bool Path::ContainsDir(const std::string& path, const std::string& dirName)
    {
        std::vector<std::string> dirs = Path::GetDirs(path);
        for (auto& dir : dirs)
            if (dir == dirName)
                return true;
        return false;
    }

    bool Path::AppendDirIfDoesNotExist(const std::string& dirName)
    {
        std::vector<std::string> dirs = Path::GetDirs(absolutePath_);
        if (dirs.empty())
        {
            filePath_ = dirName + "/" + filename_;
            ReDoState();
            return true;
        }
        else
        {
            if (dirs.back() != dirName)
            {
                if (path_.empty())
                    filePath_ = dirName + "/" + filename_;
                else
                    filePath_ = path_ + "/" + dirName + "/" + filename_;
                ReDoState();
                return true;
            }
        }
        return false;
    }
}