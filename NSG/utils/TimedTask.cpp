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
#include "TimedTask.h"
#include <assert.h>

namespace NSG
{
    namespace Task
    {
        bool operator > (const TimedTask::PData& left , const TimedTask::PData& right)
        {
            Milliseconds diff = std::chrono::duration_cast<Milliseconds>(left->timePoint_ - right->timePoint_);
            return diff.count() > 0;
        }

        TimedTask::Data::Data(int id, PTask pTask, TimePoint timePoint, Type type, Milliseconds repeatStep, size_t repeatTimes)
            : id_(id), pTask_(pTask), timePoint_(timePoint), type_(type), repeatStep_(repeatStep), repeatTimes_(repeatTimes), canceled_(false)
        {
        }

        TimedTask::TimedTask(const std::string& name, Milliseconds precision)
            : Worker(name),
              taskAlive_(true),
              precision_(precision)
        {
            Worker::Start(this);
        }

        TimedTask::~TimedTask()
        {
            taskAlive_ = false;
            condition_.notify_one();
            Join();
        }

        void TimedTask::RunWorker()
        {
            InternalTask();
        }

        bool TimedTask::IsEmpty() const
        {
            std::lock_guard<Mutex> guard(mtx_);
            return queue_.empty();
        }

        TimedTask::Data& TimedTask::GetTop()
        {
            std::lock_guard<Mutex> guard(mtx_);
            return *queue_.top().get();
        }

        void TimedTask::Run()
        {
            PData pData;
            {
                std::lock_guard<Mutex> guard(mtx_);
                pData = queue_.top();
                queue_.pop();
            }

            if (pData->canceled_)
                return;

            try
            {
                pData->pTask_->Run();
            }
            catch (std::exception& e)
            {
                pData->pTask_->Exception(e);
            }

            if (taskAlive_ && pData->type_ != Data::ONCE)
            {
                if (Data::REPEAT_TIMES == pData->type_)
                {
                    if (pData->repeatTimes_ == 0)
                    {
                        return;
                    }
                    else
                    {
                        --pData->repeatTimes_;
                    }
                }
                pData->timePoint_ = Clock::now();
                pData->timePoint_ += pData->repeatStep_;
                std::lock_guard<Mutex> guard(mtx_);
                queue_.push(pData);
            }
        }

        void TimedTask::InternalTask()
        {
            bool isEmpty = true;

            while (taskAlive_ || !isEmpty)
            {
                isEmpty = IsEmpty();

                Milliseconds duration(Milliseconds::max());

                if (!isEmpty)
                {
                    Data data(GetTop());
                    duration = std::chrono::duration_cast<Milliseconds>(data.timePoint_ - Clock::now());
                    if (duration <= precision_)
                    {
                        if (duration < -precision_ && !data.pTask_->OverDue(duration))
                        {
                            std::lock_guard<Mutex> guard(mtx_);
                            queue_.pop();
                            continue;
                        }
                        Run();
                        continue;
                    }
                }
                bool run = false;
                {
                    std::unique_lock<Mutex> lck(mtx_);
                    if (duration != Milliseconds::max())
                    {
                        run = std::cv_status::timeout == condition_.wait_for(lck, duration);
                    }
                    else
                    {
                        while (queue_.empty() && taskAlive_)
                        {
                            condition_.wait(lck);
                        }
                    }
                }
                if (run && taskAlive_)
                {
                    Run();
                }
            }
        }

        static int s_id(0);

        int TimedTask::AddTask(PTask pTask, TimePoint timePoint)
        {
            std::lock_guard<Mutex> guard(mtx_);
            int id = ++s_id;
            PData pData(new Data(id, pTask, timePoint, Data::ONCE, Milliseconds::zero(), 0));
            queue_.push(pData);
            keyDataMap_.insert(MAP_ID_DATA::value_type(id, pData));
            condition_.notify_one();
            return id;
        }

        int TimedTask::AddLoopTask(PTask pTask, TimePoint timePoint, Milliseconds repeat)
        {
            std::lock_guard<Mutex> guard(mtx_);
            int id = ++s_id;
            PData pData(new Data(id, pTask, timePoint, Data::REPEAT_LOOP, repeat, 0));
            queue_.push(PData(pData));
            keyDataMap_.insert(MAP_ID_DATA::value_type(id, pData));
            condition_.notify_one();
            return id;
        }

        int TimedTask::AddRepeatTask(PTask pTask, TimePoint timePoint, Milliseconds repeat, size_t times)
        {
            assert(times > 1);
            std::lock_guard<Mutex> guard(mtx_);
            int id = ++s_id;
            PData pData(new Data(id, pTask, timePoint, Data::REPEAT_TIMES, repeat, times - 1));
            queue_.push(PData(pData));
            keyDataMap_.insert(MAP_ID_DATA::value_type(id, pData));
            condition_.notify_one();
            return id;
        }

        bool TimedTask::CancelTask(int id)
        {
            std::lock_guard<Mutex> guard(mtx_);
            auto it = keyDataMap_.find(id);
            if (it != keyDataMap_.end())
            {
                it->second->canceled_ = true;
                keyDataMap_.erase(it);
                return true;
            }
            return false;
        }
    }
}