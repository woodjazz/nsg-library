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
#include "Worker.h"
#include "Types.h"
#include <cassert>
#include <chrono>

#if defined(EMSCRIPTEN)
#include "emscripten.h"
#endif

namespace NSG
{
    Worker::Worker(const std::string& name)
        : worker_(nullptr)
    {
        #if defined(EMSCRIPTEN)
        finish_ = false;
        std::string path = "./data/NSGWorker.js";
        //handle_ = emscripten_create_worker(path.c_str());
        #endif
    }

    Worker::~Worker()
    {
        #if defined(EMSCRIPTEN)
        //emscripten_destroy_worker(handle_);
        #endif
    }

    void Worker::Start(Worker* worker)
    {
        assert(worker);
        worker_ = worker;
        #if defined(EMSCRIPTEN)
        finish_ = false;
        //emscripten_call_worker(handle_, "NSGWorkerEntryPoint", (char*)this, sizeof(Worker*), nullptr, nullptr);
        //emscripten_run_script("new Worker('data/NSGWorker.js');");
        //emscripten_run_script("document.write(5 + 6);");
        
        #else
        thread_ = std::thread([this]() {WorkerEntryPoint();});
        #endif

    }

    void Worker::Join()
    {
        #if defined(EMSCRIPTEN)
        while (!finish_)
            std::this_thread::sleep_for(Milliseconds(50));
        #else
        thread_.join();
        #endif
    }

    #if !defined(EMSCRIPTEN)
    void Worker::WorkerEntryPoint()
    {
        worker_->RunWorker();
    }
    #endif
}