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
#include <chrono>
#include <condition_variable>
#include <deque>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
using namespace std;

namespace utils {
typedef std::chrono::steady_clock::time_point TimePoint;
typedef std::chrono::milliseconds Milliseconds;

class ActiveObject {
    struct BasicTask {
        virtual ~BasicTask() {}
        virtual void Execute() = 0;
    };
    typedef std::unique_ptr<BasicTask> PTask;
    template <typename Res, typename... Params> struct Task : public BasicTask {
        std::packaged_task<Res(Params... params)> task_;
        Task(std::function<Res(Params... params)> func) : task_(func) {}
        void Execute() override { task_(); }
    };
    struct PriorityTask {
        TimePoint timePoint_;
        Milliseconds duration_;
        std::shared_ptr<BasicTask> task_;
        PriorityTask(const TimePoint& timePoint, Milliseconds duration,
                     const std::shared_ptr<BasicTask>& task)
            : timePoint_(timePoint), duration_(duration), task_(task) {}
        bool operator>(const PriorityTask& obj) const {
            Milliseconds diff = std::chrono::duration_cast<Milliseconds>(
                timePoint_ - obj.timePoint_);
            return diff.count() > 0;
        }
    };

public:
    ActiveObject();
    virtual ~ActiveObject();
    void Join();
    template <typename Res, typename... Args, typename Fun>
    std::future<Res> Enqueue(Fun func, Args... args) {
        std::lock_guard<std::mutex> guard(mtx_);
        auto task =
            std::unique_ptr<Task<Res>>(new Task<Res>(std::bind(func, args...)));
        auto result = task->task_.get_future();
        queue_.push_back(std::move(task));
        condition_.notify_one();
        return result;
    }
    template <typename Res, typename... Args, typename Fun>
    std::future<Res> Schedule(const TimePoint& timePoint, Milliseconds duration,
                              Fun func, Args... args) {
        std::lock_guard<std::mutex> guard(mtx_);
        auto task = std::make_shared<Task<Res>>(std::bind(func, args...));
        auto result = task->task_.get_future();
        PriorityTask priorityTask(timePoint, duration, task);
        priorityQueue_.push(priorityTask);
        condition_.notify_one();
        return result;
    }

private:
    void Run();
    std::condition_variable condition_;
    std::deque<PTask> queue_;
    mutable std::mutex mtx_;
    std::thread thread_;
    std::atomic<bool> alive_;
    std::priority_queue<PriorityTask, std::vector<PriorityTask>,
                        std::greater<PriorityTask>>
        priorityQueue_;
};
}
