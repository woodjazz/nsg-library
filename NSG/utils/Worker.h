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
#include <thread>

namespace NSG
{
    class Worker
    {
    public:
        Worker(const std::string& name);
        ~Worker();
        void Start(Worker* worker);
        void Join();
        virtual void RunWorker() = 0;
        #if defined(EMSCRIPTEN)
        void SetAsFinished() { finish_ = true; }
        Worker* Get() { return worker_; }
        #else
        void WorkerEntryPoint();
        #endif
    private:
        #if defined(EMSCRIPTEN)
        int handle_;
        bool finish_;
        #else
        std::thread thread_;
        #endif
        Worker* worker_;
    };
}

