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

#include "NSG.h"

using namespace NSG;

static void* LockFile(const char* filename)
{
    #if _WIN32
    auto handle = CreateFileA(filename, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (INVALID_HANDLE_VALUE != handle)
        return handle;
    return nullptr;
    #else
    return fopen(filename, "r+");
    #endif
}

static void UnlockFile(void* handle)
{
    #if _WIN32
    CloseHandle(handle);
    #else
    fclose((FILE*)handle);
    #endif
}

int NSG_MAIN(int argc, char* argv[])
{
    auto window = Window::Create("Viewer", 0, 0, 600, 600);
    //auto window = Window::Create();
    PAppData data;
    PCamera camera;
    PCameraControl control;
    Path inputFile;
    std::string lastModification;

    auto ReadFile = [&]()
    {
        control = nullptr;
        camera = nullptr;
        data = nullptr;
        auto extension = inputFile.GetExtension();

        if (extension == "xml" || extension == "lz4")
        {
            data = std::make_shared<AppData>(Resource::GetOrCreate<ResourceFile>(inputFile.GetFullAbsoluteFilePath()));
        }

        if (!data->scenes_.empty())
        {
            auto scene = data->scenes_.at(0);
            camera = scene->GetMainCamera();
            if (!camera)
                camera = scene->CreateChild<Camera>();
            control = std::make_shared<CameraControl>(camera);
        }
    };

    auto slotButtonClicked = window->SigDropFile()->Connect([&](const std::string & file)
    {
        inputFile = Path(file);
        lastModification.clear();
    });

    auto engine = Engine::Create();
    float totalTime = 0;
    auto updateSlot = engine->SigUpdate()->Connect([&](float deltaTime)
    {
        totalTime += deltaTime;
        const float FREQUENCY = 1; // seconds
        if (inputFile.HasName() && totalTime > FREQUENCY)
        {
            if (lastModification.empty() || lastModification != inputFile.GetModificationTime())
            {
                auto lock = LockFile(inputFile.GetFullAbsoluteFilePath().c_str());
                if (lock)
                {
                    totalTime = 0;
                    ReadFile();
                    lastModification = inputFile.GetModificationTime();
                    UnlockFile(lock);
                }
            }
        }
    });

    return engine->Run();

}