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
#include "NSG.h"
using namespace NSG;
using namespace NSG::Task;

static void TimedTaskTest0()
{
    struct TaskBase : NSG::Task::Task
    {
        int counter_;

        TaskBase() 
            : counter_(0) 
        {
        }
        void Run() 
        {
            ++counter_;
        }
    };

    struct Task0 : TaskBase {};

    struct Task1 : TaskBase {};

    struct Task2 : TaskBase {};

    struct Task3 : TaskBase {};

    struct Task4 : TaskBase {};

    TimedTask tasks("tasks", Milliseconds(10));

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


    TimePoint t0 = Clock::now() + Milliseconds(50);
    TimePoint t1 = Clock::now() + Milliseconds(80);
    TimePoint t2 = Clock::now() + Milliseconds(100);
    TimePoint t3 = Clock::now() + Milliseconds(150);
    TimePoint t4 = Clock::now() + Milliseconds(400);

    int id0 = tasks.AddLoopTask(pTask0, t0, Milliseconds(100));
    tasks.AddTask(pTask1, t1);
    tasks.AddTask(pTask2, t2);
    tasks.AddRepeatTask(pTask3, t3, Milliseconds(200), 2);
    tasks.AddLoopTask(pTask4, t4, Milliseconds(400));

    std::this_thread::sleep_for(Milliseconds(700));

    CHECK_ASSERT(p0->counter_ > 1, __FILE__, __LINE__);
    CHECK_ASSERT(p1->counter_ == 1, __FILE__, __LINE__);
    CHECK_ASSERT(p2->counter_ == 1, __FILE__, __LINE__);
    CHECK_ASSERT(p3->counter_ == 2, __FILE__, __LINE__);
    CHECK_ASSERT(p4->counter_ == 1, __FILE__, __LINE__);

    CHECK_ASSERT(tasks.CancelTask(id0), __FILE__, __LINE__);

    std::this_thread::sleep_for(Milliseconds(600));

    CHECK_ASSERT(p0->counter_ > 1, __FILE__, __LINE__);
    CHECK_ASSERT(p4->counter_ == 3, __FILE__, __LINE__);
}

static void TimedTaskTest1()
{
    struct TaskBase : NSG::Task::Task
    {
        int counter_;
        bool overdue_;
        bool hasException_;

        TaskBase() 
            : counter_(0), overdue_(false), hasException_(false) 
        {
        }
        
        void Run() 
        {
            ++counter_;
        }
        
        bool OverDue(Milliseconds overDueTime) 
        {
            overdue_ = true;
            return true;
        }
        
        void Exception(const std::exception& e) 
        {
            hasException_ = true;
        }
    };

    struct Task0 : TaskBase 
    {
        void Run() 
        {
            TaskBase::Run();
            std::this_thread::sleep_for(Milliseconds(100));
        }
    };

    struct Task1 : TaskBase 
    {
        bool OverDue(Milliseconds overDueTime) 
        {
            TaskBase::OverDue(overDueTime);
            return false;
        }
    };

    struct Task2 : TaskBase 
    {
        void Run() 
        {
            TaskBase::Run();
            //throw std::runtime_error("Error in task2");
        }
    };

    TimedTask tasks("tasks", Milliseconds(10));

    Task0* p0(new Task0);
    Task1* p1(new Task1);
    Task2* p2(new Task2);

    PTask pTask0(p0);
    PTask pTask1(p1);
    PTask pTask2(p2);

    TimePoint t = Clock::now() + Milliseconds(100);

    tasks.AddLoopTask(pTask0, t, Milliseconds(150));
    tasks.AddTask(pTask1, t);
    tasks.AddTask(pTask2, t);

    std::this_thread::sleep_for(Milliseconds(250));

    CHECK_ASSERT(p0->counter_ == 1, __FILE__, __LINE__);
    //CHECK_ASSERT(p1->overdue_, __FILE__, __LINE__);
    //CHECK_ASSERT(p1->counter_ == 0, __FILE__, __LINE__);
    //CHECK_ASSERT(p2->overdue_, __FILE__, __LINE__);
    //CHECK_ASSERT(p2->counter_ == 1, __FILE__, __LINE__);
    //CHECK_ASSERT(p2->hasException_, __FILE__, __LINE__);
}

void TimedTaskTest()
{
    TimedTaskTest0();
    TimedTaskTest1();
}
