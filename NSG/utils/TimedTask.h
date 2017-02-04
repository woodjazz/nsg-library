/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2017 Néstor Silveira Gorski

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
#include "Types.h"
#include "Task.h"
#include "Worker.h"
#include "NonCopyable.h"
#include <queue>
#include <map>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <string>

namespace NSG 
{
    namespace Task 
    {
        class TimedTask : Worker, NonCopyable 
        {
        public:
	        TimedTask(const std::string& name, Milliseconds precision);
	        ~TimedTask();
            int AddTask(PTask pTask, TimePoint timePoint);
            int AddLoopTask(PTask pTask, TimePoint timePoint, Milliseconds repeat);
            int AddRepeatTask(PTask pTask, TimePoint timePoint, Milliseconds repeat, size_t times);
            bool CancelTask(int id);
        private:
            void RunWorker() override;
            bool IsEmpty() const;
            void InternalTask();
            struct Data {
                enum Type {ONCE, REPEAT_LOOP, REPEAT_TIMES};
                int id_;
                PTask pTask_;
                TimePoint timePoint_;
                Type type_;
                Milliseconds repeatStep_;
                size_t repeatTimes_;
                bool canceled_;

                Data(int id, PTask pTask, TimePoint timePoint, Type type, Milliseconds repeatStep, size_t repeatTimes);
            };
            typedef std::shared_ptr<Data> PData;
            Data& GetTop();
            void Run();
            typedef std::priority_queue<PData, std::vector<PData>, std::greater<PData>> QUEUE;
            typedef std::map<int, PData> MAP_ID_DATA;
            typedef std::mutex Mutex;
            typedef std::condition_variable Condition;
            typedef std::thread Thread;
            
            bool taskAlive_;
            mutable Mutex mtx_;
            QUEUE queue_;
            MAP_ID_DATA keyDataMap_;
            Condition condition_;
            Milliseconds precision_;
            friend bool operator > (const NSG::Task::TimedTask::PData& a , const NSG::Task::TimedTask::PData& b);
        };

        bool operator > (const TimedTask::PData& a , const TimedTask::PData& b);
    }
}
