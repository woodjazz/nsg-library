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
#include "ActiveObject.h"
namespace utils
{
    ActiveObject::ActiveObject()
        : alive_(true)
    {
        thread_ = std::thread([this]() { Run(); });
    }

    ActiveObject::~ActiveObject()
    {
        if (alive_)
            Join();
    }

    void ActiveObject::Join()
    {
        alive_ = false;
        condition_.notify_one();
        thread_.join();
    }

    void ActiveObject::Run()
    {
        while (alive_)
        {
            PTask task;
            {
                std::unique_lock<std::mutex> lck(mtx_);
                Milliseconds duration(Milliseconds::max()); //default timeout when the priority queue is empty
                if(!priorityQueue_.empty())
                {
                    duration = std::chrono::duration_cast<Milliseconds>(priorityQueue_.top().timePoint_ - std::chrono::steady_clock::now());
                    const Milliseconds toleranceError(50);
                    if(duration < toleranceError)
                    {
                        if(duration < -toleranceError)
                        {
                            // We have a time violation. 
                            // This is very bad because we could not meet the time constraint.
                            // We can reach this point if some callback was too slow
                            // or because we scheduled too many messages to be dispatched in less than toleranceError
                            // or because we executed a normal task that took too long
                            priorityQueue_.pop(); //the user will get an exception (see std::packaged_task::~packaged_task)
                        }
                        else
                        {
                            // less than toleranceError => no wait needed
                            // Execute object from priority queue
                            auto task = priorityQueue_.top().task_;
                            auto duration = priorityQueue_.top().duration_;
                            priorityQueue_.pop();
                            if(task)
                            {
                                auto start = std::chrono::steady_clock::now();
                                task->Execute();
                                auto end = std::chrono::steady_clock::now();
                                auto diff = std::chrono::duration_cast<Milliseconds>(end - start);
                                if(diff > duration)
                                    fprintf(stderr, "The execution of the task took %ld ms. The expected duration was %ld ms.\n", diff.count(), duration.count());
                            }
                            continue;
                        }
                    }
                }
                bool noTimeout = condition_.wait_for(lck, duration, [this]() 
                { 
                    return !queue_.empty() || !alive_; 
                });
                if(noTimeout && !queue_.empty())
                {
                    //execute a normal task (without priority)
                    task = std::move(queue_.front());
                    queue_.pop_front();
                }
            }
            if(task)
                task->Execute();
        }
    }
}
