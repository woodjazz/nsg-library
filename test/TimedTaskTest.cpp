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
#undef NDEBUG
#include <assert.h>
#include <iostream>
#include <string>

using namespace NSG::Task;

static void TimedTaskTest0()
{
    struct TaskBase : Task {
        int counter_;

        TaskBase() 
            : counter_(0) {
        }
        void Run() {
            ++counter_;
        }
    };

    struct Task0 : TaskBase {
    };

    struct Task1 : TaskBase {
    };

    struct Task2 : TaskBase {
    };

    struct Task3 : TaskBase {
    };

    struct Task4 : TaskBase {
    };


    TimedTask tasks(Milliseconds(100));

    Task0* p0(new Task0);
    Task1* p1(new Task1);
    Task2* p2(new Task2);
    Task3* p3(new Task3);
    Task4* p4(new Task4);

    PTask pTask0(p0);
    PTask pTask1(p1);
    PTask pTask2(p2);
    PTask pTask3(p3);
    PTask pTask4(p4);


    TimePoint t0 = Clock::now() + Milliseconds(500);
    TimePoint t1 = Clock::now() + Milliseconds(800);
    TimePoint t2 = Clock::now() + Milliseconds(1000);
    TimePoint t3 = Clock::now() + Milliseconds(1500);
    TimePoint t4 = Clock::now() + Milliseconds(4000);

    int id0 = tasks.AddLoopTask(pTask0, t0, Seconds(1));
    int id1 = tasks.AddTask(pTask1, t1);
    int id2 = tasks.AddTask(pTask2, t2);
    int id3 = tasks.AddRepeatTask(pTask3, t3, Seconds(2), 2);
    int id4 = tasks.AddLoopTask(pTask4, t4, Seconds(4));

    std::this_thread::sleep_for(Seconds(7));

    assert(p0->counter_ == 7);
    assert(p1->counter_ == 1);
    assert(p2->counter_ == 1);
    assert(p3->counter_ == 2);
    assert(p4->counter_ == 1);

    assert(tasks.CancelTask(id0));

    std::this_thread::sleep_for(Seconds(6));

    assert(p0->counter_ == 7);
    assert(p4->counter_ == 3);
}

static void TimedTaskTest1()
{
    struct TaskBase : Task {
        int counter_;
        bool overdue_;
        bool hasException_;
        std::string e_;

        TaskBase() 
            : counter_(0), overdue_(false), hasException_(false) {
        }
        void Run() {
            ++counter_;
        }
        bool OverDue(Milliseconds overDueTime) {
            overdue_ = true;
            return true;
        }
        void Exception(const std::exception& e) {
            hasException_ = true;
            e_ = e.what();
        }
    };

    struct Task0 : TaskBase {
        void Run() {
            TaskBase::Run();
            std::this_thread::sleep_for(Seconds(1));
        }
    };

    struct Task1 : TaskBase {
        bool OverDue(Milliseconds overDueTime) {
            TaskBase::OverDue(overDueTime);
            return false;
        }
    };

    struct Task2 : TaskBase {
        void Run() {
            TaskBase::Run();
            throw std::runtime_error("Error in task2");
        }
    };

    TimedTask tasks(Milliseconds(100));

    Task0* p0(new Task0);
    Task1* p1(new Task1);
    Task2* p2(new Task2);

    PTask pTask0(p0);
    PTask pTask1(p1);
    PTask pTask2(p2);

    TimePoint t = Clock::now() + Seconds(1);

    tasks.AddLoopTask(pTask0, t, Milliseconds(1500));
    tasks.AddTask(pTask1, t);
    tasks.AddTask(pTask2, t);

    std::this_thread::sleep_for(Milliseconds(2500));

    assert(p0->counter_ == 1);
    assert(p1->overdue_);
    assert(p1->counter_ == 0);
    assert(p2->overdue_);
    assert(p2->counter_ == 1);
    assert(p2->hasException_);
    assert(std::string("Error in task2") == p2->e_);
}

void TimedTaskTest()
{
    TimedTaskTest0();
    TimedTaskTest1();
}
