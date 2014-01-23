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
#include "NonCopyable.h"
#include <memory>
#include <deque>
#include <map>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>
#include "Task.h"

namespace NSG {

    namespace Task {

        class QueuedTask : NonCopyable {
        public:
	        QueuedTask();
	        ~QueuedTask();
            int AddTask(PTask pTask);
            bool CancelTask(int id);
            void CancelAllTasks();
        private:
            void InternalTask();
            struct Data {
                int id_;
                PTask pTask_;
                bool canceled_;

                Data(int id, PTask pTask);
            };
            typedef std::shared_ptr<Data> PData;
            PData Pop();
            typedef std::deque<PData> QUEUE;
            typedef std::map<int, PData> MAP_ID_DATA;
            typedef std::mutex Mutex;
            typedef std::condition_variable Condition;
            typedef std::thread Thread;
            
            std::atomic<bool> taskAlive_;
            std::atomic<int> pendingTasks_;
            mutable Mutex mtx_;
            QUEUE queue_;
            MAP_ID_DATA keyDataMap_;
            Condition condition_;
            Thread thread_;
        };
    }
}






