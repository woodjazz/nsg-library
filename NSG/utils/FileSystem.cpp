/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

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
#include "FileSystem.h"
#include "Path.h"
#include "Check.h"
#include "AppConfiguration.h"
#include "Engine.h"
#include "Util.h"
#include "pugixml.hpp"
#if EMSCRIPTEN
#include "SDL.h"
#include <emscripten.h>
#include <emscripten/bind.h>
#include <html5.h>
#endif

#ifndef WIN32
#include <unistd.h>
#include <cerrno>
#else
#include <windows.h>
#endif

#include <thread>
#include <fstream>

namespace NSG
{
    #if EMSCRIPTEN
    static bool isFSReady = false;
    static bool isFSInitializing = false;
    static void FSReady()
    {
        TRACE_PRINTF("Ready...\n");
        isFSReady = true;
        FileSystem::SigReady()->Run();
    }

    EMSCRIPTEN_BINDINGS(FSReady)
    {
        emscripten::function("FSReady", &FSReady);
    }

    static bool isFSSaved = false;
    static void FSSaved()
    {
        TRACE_PRINTF("Saved...\n");
        isFSSaved = true;
        FileSystem::SigSaved()->Run();
    }

    EMSCRIPTEN_BINDINGS(FSSaved)
    {
        emscripten::function("FSSaved", &FSSaved);
    }
    #endif

    std::string FileSystem::GetPreferencesPath()
    {
        auto& conf = Engine::GetPtr()->GetAppConfiguration();
        return FileSystem::GetPreferencesPath(conf.orgName_, conf.appName_);
    }

    std::string FileSystem::GetPreferencesPath(const std::string& org, const std::string& app)
    {
        Path path(org + "/" + app + "/");
        #if defined(SDL) && !defined(EMSCRIPTEN)
        {
            char* prefsPath = SDL_GetPrefPath(org.c_str(), app.c_str());
            CHECK_CONDITION(prefsPath && "Preferences path not supported!!!", __FILE__, __LINE__);
            path = Path(prefsPath);
            SDL_free(prefsPath);
        }
        #endif
        return path.GetAbsolutePath();
    }

    void FileSystem::Initialize()
    {
        #if EMSCRIPTEN
        if (!isFSReady)
        {
            if (!isFSInitializing)
            {
                isFSInitializing = true;
                //TRACE_PRINTF("Begin InitializeFS\n");
                auto& conf = Engine::GetPtr()->GetAppConfiguration();
                std::string org = conf.orgName_;
                std::string app = conf.appName_;
                emscripten_run_script(std::string("FS.mkdir('" + org + "')").c_str());
                emscripten_run_script(std::string("FS.mkdir('" + org + "/" + app + "')").c_str());
                emscripten_run_script(std::string("FS.mount(IDBFS, {}, '/" + org + "/" + app + "')").c_str());
                EM_ASM(FS.syncfs(true, function (err)
                {
                    assert(!err);
                    Module.FSReady();
                }));
            }
        }
        #else
        FileSystem::SigReady()->Run();
        #endif
    }

    bool FileSystem::Save()
    {
        #if EMSCRIPTEN
        {
            if (!isFSReady)
                return false;
            EM_ASM(FS.syncfs(false, function (err)
            {
                assert(!err);
                Module.FSSaved();
            }));
        }
        #else
        {
            FileSystem::SigSaved()->Run();
        }
        #endif
        return true;
    }

    SignalEmpty::PSignal FileSystem::SigReady()
    {
        static SignalEmpty::PSignal signalReady(new SignalEmpty);
        return signalReady;
    }

    SignalEmpty::PSignal FileSystem::SigSaved()
    {
        static SignalEmpty::PSignal signalSaved(new SignalEmpty);
        return signalSaved;
    }

    bool FileSystem::SetCurrentDir(const std::string& path)
    {
        if (!path.empty())
        {
            #ifdef WIN32
            {
                if (::SetCurrentDirectory(path.c_str()) == FALSE)
                {
                    TRACE_PRINTF("Failed to change directory to %s with error = %d", path.c_str(), GetLastError());
                    return false;
                }
            }
            #else
            {
                if (chdir(path.c_str()) != 0)
                {
                    TRACE_PRINTF("Failed to change directory to %s with error = %d", path.c_str(), errno);
                    return false;
                }
            }
            #endif
        }
        return true;
    }

    bool FileSystem::Copy(const Path& source, const Path& target)
    {
        std::ifstream is(source.GetFullAbsoluteFilePath(), std::ios::binary);
        if (is.is_open())
        {
            std::ofstream os(target.GetFullAbsoluteFilePath(), std::ios::binary);
            if (os.is_open())
            {
                os << is.rdbuf();
                return true;
            }
        }
        return false;
    }

    bool FileSystem::SaveDocument(const Path& path, const pugi::xml_document& doc, bool compress)
    {
        if (compress)
        {
            Path filename(path);
            filename.AddExtension("lz4");
            TRACE_PRINTF("Saving file: %s", filename.GetFullAbsoluteFilePath().c_str());
            struct XMLWriter : pugi::xml_writer
            {
                std::string buffer_;
                void write(const void* data, size_t size) override
                {
                    const char* m = (const char*)data;
                    buffer_.insert(buffer_.end(), &m[0], &m[size]);
                }
            } writer;
            doc.save(writer);
            auto compressedBuffer = CompressBuffer(writer.buffer_);
            std::ofstream os(filename.GetFullAbsoluteFilePath(), std::ios::binary);
            if (os.is_open())
            {
                os.write(&compressedBuffer[0], compressedBuffer.size());
                return true;
            }
            return false;
        }
        else
            return doc.save_file(path.GetFullAbsoluteFilePath().c_str());
    }


}
